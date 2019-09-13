// thread-pc-simple.cpp - a simple producer consumer threading model
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on bufValid flag for buffer empty or available
// Algorithm and author Greg Blair

#include <atomic>
#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;
#include <sched.h>

class PC {
  // volatile is deprecated
  // the only case where you get guaranteed multithreaded behavior by the 
  // C++11 memory model is if you use a proper mutex or std::atomic<int> 
  // x with the proper atomic load/store calls. 

#ifdef USE_VOLATILE
  volatile bool      bufValid     = false; // true data available for consumer, false producer needs to create data
  volatile bool      pAlive       = false;
  volatile bool      cAlive       = false;
  volatile bool      stop         = false;
#else
  atomic<bool>       bufValid     = false; // true data available for consumer, false producer needs to create data
  atomic<bool>       cAlive       = false;
  atomic<bool>       pAlive       = false;
  atomic<bool>       stop         = false;
#endif
  int                buf          = 0;
  thread             pTid;
  thread             cTid;
  uint64_t           cWastedTSC   = 0;
  uint64_t           pWastedTSC   = 0;

  inline uint64_t tscTimedFunction (function<void(void)> f) {
    auto RDTSC = [] {uint32_t a,d; asm volatile("rdtsc" : "=a"(a),"=d"(d)); return (uint64_t(d)<<32)|uint64_t(a);}; // 22 clocks
    uint64_t start = RDTSC(); 
    f();
    uint64_t stop  = RDTSC(); 
    return start < stop? stop - start: start - stop;  // RDTSC wraps
  }  

  void  pause() {asm volatile("pause");}  // see Intel docs w.r.t. spinloops usage
  // The PAUSE instruction pauses speculative execution and yields execution resources to the other (hw) thread (if present)
  // https://software.intel.com/en-us/articles/benefitting-power-and-performance-sleep-loops
  // https://software.intel.com/en-us/articles/long-duration-spin-wait-loops-on-hyper-threading-technology-enabled-intel-processors

  inline void producer() { // write
    moveToCore(thread::hardware_concurrency() == 4? 3: 1, pTid.native_handle()); // different cores for spinlock not to be pathological
    pAlive = true;
    do {
      auto pWait = [this] {while(bufValid and not stop) pause();}; // wait for buffer empty
      pWastedTSC += tscTimedFunction(pWait);
      if(stop) break;

      buf++; // Produce buf.
      bufValid = true;
    } while(not stop);
    pAlive = false;
  }

  inline void consumer() { // read
    moveToCore(thread::hardware_concurrency() == 4? 2: 0, cTid.native_handle()); // different cores for spinlock not to be pathological
    cAlive = true;
    do {
      auto cWait = [this] {while(not bufValid and not stop) pause();}; // wait for data
      cWastedTSC += tscTimedFunction(cWait);
      if(stop) break;

      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      bufValid = false;
    } while(not stop);
    cAlive = false;
  }

 void moveToCore(int core, pthread_t ptid)
  {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    // for (int core = 0; core < 4; core++) CPU_SET(core, &cpuset); // let thread run on any of these CPU cores.
    CPU_SET(core, &cpuset);

    if(pthread_setaffinity_np(ptid, sizeof(cpu_set_t), &cpuset)) // move
      cerr <<  "ERROR: pthread_setaffinity_np\n";
  }

  void moveToCore(int core, pthread_t ptid, string label)
  {
    moveToCore(core, ptid);
    printCoreAffinity(ptid, label);
  }

  void printCoreAffinity(pthread_t ptid, string label)
  {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    if(pthread_getaffinity_np(ptid, sizeof(cpu_set_t), &cpuset))
      cerr << "ERROR: pthread_getaffinity_np\n";

    string p = label + " set returned by pthread_getaffinity_np() contained:\n";
    for (int i = 0; i < CPU_SETSIZE; i++) 
      if(CPU_ISSET(i, &cpuset)) 
        p += "    core " + to_string(i) + "\n";
    cout << p;
  }

public:

  inline void run(int ms) {                            // run for 'ms' milliseconds
    pTid = thread(&PC::producer, this);
    cTid = thread(&PC::consumer, this);

    while(pAlive == false or cAlive == false) pause();  // wait for pipeline to come up

    this_thread::sleep_for(chrono::milliseconds(ms));
    stop = true;

    pTid.join();
    cTid.join();

    cout << "buf=" << buf << ", " << 1. * buf << "\n";  // prints 6-8 Million on 2016 era I5 laptop
    cout << "producer wasted clocks  = " << pWastedTSC << ", " << 1. * pWastedTSC << "\n";
    cout << "consumer wasted clocks  = " <<  cWastedTSC << ", " << 1. *  cWastedTSC << "\n";
    cout << "producer wasted clock per critical region access = " << pWastedTSC              / double(buf) << "\n";
    cout << "consumer wasted clock per critical region access = " << cWastedTSC              / double(buf) << "\n";
    cout << "TOTAL    wasted clock per critical region access = " << (pWastedTSC+cWastedTSC) / double(buf) << "\n";
  }
}; // class PC

int main(int argc, char*argv[]) {
    PC pc;

    pc.run(1000);
    return 0;
}
