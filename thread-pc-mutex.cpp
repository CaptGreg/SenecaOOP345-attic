// thread-pc-simple.cpp - a simple producer consumer threading model
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on bufValid flag for buffer empty or available
// Algorithm and author Greg Blair

#warning comment("TODO: test")

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
#include <mutex>
using namespace std;

class PC {
           mutex     m;
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
      auto waitForEmptyBuffer = [this] {m.lock();};
      emptyWaitTSC += tscTimedFunction(waitForEmptyBuffer);
      if(stop) break;

      buf++; // Produce buf.
      m.unlock();
    } while(not stop);
    pAlive = false;
  }

  void consumer() { // read
    cAlive = true;
    do {
      auto waitForData = [this] {m.lock();};
      dataWaitTSC += tscTimedFunction(waitForData);
      if(stop) break;

      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      m.unlock();
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
