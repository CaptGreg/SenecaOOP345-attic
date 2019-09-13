// thread-pc-dekekr.cpp - a simple producer consumer threading model
// https:https://en.wikipedia.org/wiki/Dekker%27s_algorithm
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on Deker flags for buffer empty or available

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;

/*
Dekker's algorithm is the first known correct solution to the mutual 
exclusion problem in concurrent programming. The solution is attributed 
to Dutch mathematician Th. J. Dekker by Edsger W. Dijkstra in an 
unpublished paper on sequential process descriptions[1] and his manuscript 
on cooperating sequential processes.[2] It allows two threads to share a 
single-use resource without conflict, using only shared memory for 
communication.

It avoids the strict alternation of a naïve turn-taking algorithm, and 
was one of the first mutual exclusion algorithms to be invented.

If two processes attempt to enter a critical section at the same time, 
the algorithm will allow only one process in, based on whose turn it is. 
If one process is already in the critical section, the other process will 
busy wait for the first process to exit. This is done by the use of two 
flags, wants_to_enter[0] and wants_to_enter[1], which indicate an 
intention to enter the critical section on the part of processes 0 and 1, 
respectively, and a variable turn that indicates who has priority 
between the two processes.

Processes indicate an intention to enter the critical section which is 
tested by the outer while loop. If the other process has not flagged 
intent, the critical section can be entered safely irrespective of the 
current turn. Mutual exclusion will still be guaranteed as neither 
process can become critical before setting their flag (implying at 
least one process will enter the while loop). This also guarantees 
progress as waiting will not occur on a process which has withdrawn 
intent to become critical. Alternatively, if the other process's variable 
was set the while loop is entered and the turn variable will establish who 
is permitted to become critical. Processes without priority will withdraw 
their intention to enter the critical section until they are given priority 
again (the inner while loop). Processes with priority will break from the
while loop and enter their critical section.

Dekker's algorithm guarantees mutual exclusion, freedom from deadlock, and 
freedom from starvation. Let us see why the last property holds. Suppose p0 
is stuck inside the "while wants_to_enter[1]" loop forever. There is freedom 
from deadlock, so eventually p1 will proceed to its critical section and set 
turn = 0 (and the value of turn will remain unchanged as long as p0 doesn't 
progress). Eventually p0 will break out of the inner "while turn ≠ 0" loop (if 
it was ever stuck on it). After that it will set wants_to_enter[0] to true and 
settle down to waiting for wants_to_enter[1] to become false (since turn = 0, 
it will never do the actions in the while loop). The next time p1 tries to enter 
its critical section, it will be forced to execute the actions in its "while 
wants_to_enter[0]" loop. In particular, it will eventually set wants_to_enter[1] 
to false and get stuck in the "while turn ≠ 1" loop (since turn remains 0). The
next time control passes to p0, it will exit the "while wants_to_enter[1]" loop 
and enter its critical section.

If the algorithm were modified by performing the actions in the "while wants_to_enter[1]" 
loop without checking if turn = 0, then there is a possibility of starvation. 
Thus all the steps in the algorithm are necessary.
*/
class PC {
  volatile bool      wants_to_enter[2] = {false, false};
  volatile int       turn         = 0; // or 1
  volatile int       buf          = 0;
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

      auto waitForEmptyBuffer = [this] {
        wants_to_enter[0] = true;
        while(wants_to_enter[1] and not stop){
          if(turn != 0) {
            wants_to_enter[0] = false;
            while(turn!=0)
              ;
            wants_to_enter[0] = true;
          }
        }
      };
      emptyWaitTSC += tscTimedFunction(waitForEmptyBuffer);
      if(stop) break;

      // critical section
      buf++; // Produce buf.
      turn = 1;
      wants_to_enter[0] = false;
    } while(not stop);
    pAlive = false;
  }

  void consumer() { // read
    cAlive = true;
    do {
      auto waitForData = [this] {
        wants_to_enter[1] = true;
        while(wants_to_enter[0] and not stop){
          if(turn != 1) {
            wants_to_enter[1] = false;
            while(turn!=1)
              ;
            wants_to_enter[1] = true;
          }
        }
      };
      dataWaitTSC += tscTimedFunction(waitForData);
      if(stop) break;

      // critical section
      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      turn = 0;
      wants_to_enter[1] = false;
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
