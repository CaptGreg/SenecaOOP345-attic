// https://bartoszmilewski.com/2008/11/05/who-ordered-memory-fences-on-an-x86/e
// GB broken - hangs

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;


thread_local int threadID = 0;
class Peterson
{
private:
    // indexed by thread ID, 0 or 1
    bool _interested[2] ={ false, false};
    // who's yielding priority?
    int _victim = 0;
public:
    void lock()
    {
        // threadID is thread local,
        // initialized either to zero or one
        int me = threadID;
        int he = 1 - me;
        _interested[me] = true;
        _victim = me;
        while (_interested[he] && _victim == me)
            continue;
    }
    void unlock()
    {
        int me = threadID;
        _interested[me] = false;
    }
};

class PC {
  Peterson peterson;
  volatile bool flag[2] = {false, false};
  volatile int turn;
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
    threadID = 0;
    pAlive = true;
    do {

      auto pWait = [this] {peterson.lock();};
      emptyWaitTSC += tscTimedFunction(pWait);
      if(stop) break;

      // critical section
      buf++; // Produce buf.
    } while(not stop);
    pAlive = false;
  }

  void consumer() { // read
    threadID = 1;
    cAlive = true;
    do {
      auto cWait = [this] {peterson.lock();};
      dataWaitTSC += tscTimedFunction(cWait);
      if(stop) break;

      // critical section
      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
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
