// thread-pc-peterson.cpp - a simple producer consumer threading model
// https://en.wikipedia.org/wiki/Peterson%27s_algorithm
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on Peterson flags for buffer empty or available

// prefer nomenclature from https://bartoszmilewski.com/2008/11/05/who-ordered-memory-fences-on-an-x86/e

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;

/*
The algorithm satisfies the three essential criteria to solve the critical 
section problem, provided that changes to the variables victim, interested[0], and 
interested[1] propagate immediately and atomically. The while condition works even 
with preemption.[1]

The three criteria are mutual exclusion, progress, and bounded waiting.[3]

Since victim can take on one of two values, it can be replaced by a single bit, 
meaning that the algorithm requires only three bits of memory.[4]:22

+ Mutual exclusion +
P0 and P1 can never be in the critical section at the same time: If P0 is in 
its critical section, then interested[0] is true. In addition, either interested[1] is 
false (meaning P1 has left its critical section), or victim is 0 (meaning P1 
is just now trying to enter the critical section, but graciously waiting), 
or P1 is at label P1_gate (trying to enter its critical section, after 
setting interested[1] to true but before setting victim to 0 and busy waiting). So 
if both processes are in their critical sections then we conclude that the 
state must satisfy interested[0] and interested[1] and victim = 0 and victim = 1. No state 
can satisfy both victim = 0 and victim = 1, so there can be no state where both 
processes are in their critical sections. (This recounts an argument that 
is made rigorous in.[5])

+ Progress +
Progress is defined as the following: if no process is executing in its 
critical section and some processes wish to enter their critical sections, 
then only those processes that are not executing in their remainder sections 
can participate in making the decision as to which process will enter its 
critical section next. Note that for a process or thread, the remainder 
sections are parts of the code that are not related to the critical section. 
This selection cannot be postponed indefinitely.[3] A process cannot 
immediately re-enter the critical section if the other process has set its 
interested to say that it would like to enter its critical section.

+ Bounded waiting +
Bounded waiting, or bounded bypass means that the number of times a process 
is bypassed by another process after it has indicated its desire to enter 
the critical section is bounded by a function of the number of processes 
in the system.[3][4]:11 In Peterson's algorithm, a process will never wait 
longer than one victim for entrance to the critical section: After giving 
priority to the other process, this process will run to completion and set 
its interested to 1, thereby never allowing the other process to enter the critical 
section.
*/

class PC {
  volatile bool      interested[2] = {false, false};
  volatile int       victim;
           int       buf          = 0;
           thread    pTid;
           thread    cTid;
  volatile bool      pAlive       = false;
  volatile bool      cAlive       = false;
           uint64_t  dataWaitTSC  = 0;
           uint64_t  emptyWaitTSC = 0;
  volatile bool      stop         = false;

  uint64_t tscTimedFunction (function<void(void)> f) {
    auto RDTSC = [] {uint32_t a,d; asm volatile("rdtsc" : "=a"(a),"=d"(d)); return (uint64_t(d)<<32)|uint64_t(a);}; // 22 clocks
    uint64_t start = RDTSC(); 
    f();
    uint64_t stop  = RDTSC(); 
    return start < stop? stop - start: start - stop;  // RDTSC wraps
  }  

  void producer() { // write
    pAlive = true;
    do {
      // lock
      interested[0] = true;
      victim        = 1;
      auto waitForEmptyBuffer = [this] {while(interested[1] and victim==1 and not stop);};
      emptyWaitTSC += tscTimedFunction(waitForEmptyBuffer);
      if(stop) break;

      // critical section
      buf++; // Produce buf.
      // unlock
      interested[0] = false;
    } while(not stop);
    pAlive = false;
  }

  void consumer() { // read
    cAlive = true;
    do {
      // lock
      interested[0] = true;
      victim        = 0;
      auto waitForData = [this] {while(interested[0] and victim==0 and not stop);};
      dataWaitTSC += tscTimedFunction(waitForData);
      if(stop) break;

      // critical section
      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      // unlock
    } while(not stop);
    cAlive = false;
  }

public:

  void run(int ms) {                            // run for 'ms' milliseconds
    pTid = thread(&PC::producer, this);
    cTid = thread(&PC::consumer, this);

    while(pAlive == false or cAlive == false);  // wait for pipeline to come up

    this_thread::sleep_for(chrono::milliseconds(ms));
    stop = true;

    pTid.join();
    cTid.join();

    cout << "buf=" << buf << ", " << 1. * buf << "\n";  // prints 6-8 Million on 2016 era I5 laptop
    cout << "producer wasted clocks  = " << emptyWaitTSC << ", " << 1. * emptyWaitTSC << "\n";
    cout << "consumer wasted clocks  = " <<  dataWaitTSC << ", " << 1. *  dataWaitTSC << "\n";
    cout << "producer wasted clock per critical region access = " << emptyWaitTSC               / double(buf) << "\n";
    cout << "consumer wasted clock per critical region access = " << dataWaitTSC                / double(buf) << "\n";
    cout << "TOTAL    wasted clock per critical region access = " << (emptyWaitTSC+dataWaitTSC) / double(buf) << "\n";
  }
}; // class PC

int main(int argc, char*argv[]) {
    PC pc;

    pc.run(1000);
    return 0;
}
