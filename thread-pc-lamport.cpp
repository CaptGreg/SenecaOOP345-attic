// thread-pc-lamport.cpp - a simple producer consumer threading model
// https://en.wikipedia.org/wiki/Lamport%27s_bakery_algorithm
// a single producer thread produces data for a single consumer thread.
// mutex free, Lamports Baker algorithm

// g++ -std=c++17 -Wa,-adhln -g thread-pc-lamport.cpp -o thread-pc-lamport -pthread > thread-pc-lamport.s
//   thread-pc-lamport.s  <-- mixed C++ assembler listing

// definitive reference:
// A New Solution of Dijkstra's Concurrent Programming Problem
// Leslie Lamport Massachusetts Computer Associates, Inc.
// Communications of the ACM August 1974 Volume 17 Number 8

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;

// static constexpr int NUM_THREADS = 2;
static const     int NUM_THREADS = 2;
class PC {
  volatile bool      Choosing[NUM_THREADS] = {false};
  // Processor i is said to be in the doorway while Choosing[i] is true
  // It is said to be in the bakery from the time it resets Choosing[i] to false 
  // until it either fails or leaves its critical section.
  volatile int       Number  [NUM_THREADS] = {0};

           int       buf                   = 0;
           thread    pTid;
           thread    cTid;
  volatile bool      pAlive                = false;
  volatile bool      cAlive                = false;
           uint64_t  dataWaitTSC           = 0;
           uint64_t  emptyWaitTSC          = 0;
  volatile bool      stop                  = false;

  inline void lock(int i)
  {
    Choosing[i] = true; // we want access to critical region (at doorway)
    // Number[i]   = 1 + *std::max_element(begin(Number), end(Number));
    Number[i]   = 1 + max(Number[0], Number[1]);
    Choosing[i] = false;
    for (int j = 0; j < NUM_THREADS; j++) {
      // Wait until thread j receives its number:
      while (not stop and Choosing[j]) ; // wait for Choosing[j] to be true
      auto lessThan=[](int a, int b, int c, int d) { // Lamport paper
        // (a,b) < (c,d) is true if, if a<c or if(a==c) b<d
        if(a < c) return true;
        if(a==c)  return b < d;
        return false;
      };

      // Wait until all threads with smaller numbers or with the same
      // number, but with higher priority, finish their work:
      while (not stop  and  Number[j]  and  lessThan(Number[j], j, Number[i], i)) ; // spin
    }
  }
  inline void unlock(int i)
  {
    Number[i] = 0;
  }

  inline uint64_t tscTimedFunctionX (function<void(void)> f) {
    auto RDTSC = [] {uint32_t a,d; asm volatile("rdtsc" : "=a"(a),"=d"(d)); return (uint64_t(d)<<32)|uint64_t(a);}; // 22 clocks
    uint64_t start = RDTSC(); 
    f();
    uint64_t stop  = RDTSC(); 
    return start < stop? stop - start: start - stop;  // RDTSC wraps
  }  

  inline uint64_t tscTimedFunction (function<void(void)> f) {
    union{uint64_t u64; struct{uint32_t lo, hi;};} start, stop;
    asm volatile("rdtsc" : "=a"(start.lo),"=d"(start.hi)); 
    f();
    asm volatile("rdtsc" : "=a"(stop.lo),"=d"(stop.hi)); 
    return start.u64 < stop.u64? stop.u64 - start.u64: start.u64 - stop.u64;  // RDTSC wraps
  }  

  void producer() { // write
    pAlive = true;
    do {

      auto waitForEmptyBuffer = [this] { lock(0); };
      emptyWaitTSC += tscTimedFunction(waitForEmptyBuffer);
      if(stop) break;

      // critical section
      buf++; // Produce buf.

      unlock(0);
    } while(not stop);
    pAlive = false;
  }

  void consumer() { // read
    cAlive = true;
    do {
      auto waitForData = [this] { lock(1); };
      dataWaitTSC += tscTimedFunction(waitForData);
      if(stop) break;

      // critical section
      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      unlock(1);
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
