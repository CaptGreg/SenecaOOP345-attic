// thread-pc-double-buf.cpp - a simple producer consumer threading model
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait
// Algorithm : http://www.scs.stanford.edu/07au-cs140/notes/l4.pdf
// Author    : Greg Blair

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;

static const int      BUFFER_SIZE  = 2;
static const uint32_t UNINITILIZED = ~0u;
class PC {
           int       in           = 0;
           int       out          = 0;
           uint32_t  buffer[BUFFER_SIZE] = {UNINITILIZED};
           uint32_t  count        = 0;
           uint32_t  nextProduced = 0;
           uint32_t  nextConsumed = 0;
           thread    pTid;
           thread    cTid;
  volatile bool      pAlive       = false;
  volatile bool      cAlive       = false;
           uint64_t  pWaitTSC     = 0;
           uint64_t  cWaitTSC     = 0;
  volatile bool      stop         = false;

  uint64_t tscTimedFunction (function<void(void)> f) {
    auto RDTSC = [] {uint32_t a,d; asm volatile("rdtsc" : "=a"(a),"=d"(d)); return (uint64_t(d)<<32)|uint64_t(a);}; // 22 clocks
    uint64_t start = RDTSC(); 
    f();
    uint64_t stop  = RDTSC(); 
    return start < stop? stop - start: start - stop;  // RDTSC wraps
  }  

  void producer () { // write
    pAlive = true;
    while( not stop ) {
      /* produce an item and put in nextProduced */
      auto pWait = [this] {while (((in + 1) % BUFFER_SIZE) == out);};
      pWaitTSC += tscTimedFunction(pWait);
      buffer [in] = nextProduced++;
      in = (in + 1) % BUFFER_SIZE;
      count++;
    }
    pAlive = false;
  }
  void consumer () { // read
    cAlive = true;
    while( not stop ) {
      auto cWait = [this] {while (in == out);};
      cWaitTSC += tscTimedFunction(cWait);
      nextConsumed =  buffer[out];
      if(nextConsumed == UNINITILIZED) 
        cout << "consumed bad value\n";
      out = (out + 1) % BUFFER_SIZE;
        /*  consume the item in nextConsumed */
    }
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

    cout << "count=" << count << ", " << 1. * count << "\n";  // prints 6-8 Million on 2016 era I5 laptop
    cout << "producer wasted clocks  = " << pWaitTSC << ", " << 1. * pWaitTSC << "\n";
    cout << "consumer wasted clocks  = " << cWaitTSC << ", " << 1. * cWaitTSC << "\n";
    cout << "producer wasted clock per critical region access = " << pWaitTSC            / double(count) << "\n";
    cout << "consumer wasted clock per critical region access = " << cWaitTSC            / double(count) << "\n";
    cout << "TOTAL    wasted clock per critical region access = " << (pWaitTSC+cWaitTSC) / double(count) << "\n";
  }
};

int main(int argc, char*argv[]) {
    PC pc;

    pc.run(1000);
    return 0;
}
