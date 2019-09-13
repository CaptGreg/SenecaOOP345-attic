// thread-pc-simple-128-pause.cpp - a simple producer consumer threading model
//   128   <-- aligns(128) align on 128byte boundary
//   pause <-- use pause 
//     * run producer on CORE-3
//     * run consumer on CORE-2
//     * affinity code from man pthread_setaffinity_np
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on bufValid flag for buffer empty or available
// Algorithm and author Greg Blair

// https://software.intel.com/en-us/articles/long-duration-spin-wait-loops-on-hyper-threading-technology-enabled-intel-processors

// Application Note AP-949 "Using Spin-Loops on Intel® Pentium® 4 Processor and Intel® Xeon® processor".
// https://software.intel.com/sites/default/files/m/d/4/1/d/8/17689_w_spinlock.pdf

// Intel® 64 and IA-32 Architectures Optimization Reference Manual
// http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-optimization-manual.html

#include <functional>  // function
#include <iostream>
#include <string>
#include <thread>      // thread, join, sleep_for + chrono time units
#include <typeinfo>
using namespace std;
#include <sched.h>

#define TYPEID(X) cout << "typeid(" #X ").name()=" << typeid(X).name() << "\n";
#define SIZEOF(X) cout << "sizeof(" #X ")=" << sizeof(X) << "\n";

class PC { // sizeof = 48/384 
//volatile bool      bufValid     = false; // true data available for consumer, false producer needs to create data
//volatile bool      stop         = false;
  struct alignas(128) {volatile bool bufValid;};
  struct alignas(128) {volatile bool stop;};         
           int       buf          = 0;
           thread    pTid;
           thread    cTid;
  volatile bool      pAlive       = false;
  volatile bool      cAlive       = false;
           uint64_t  dataWaitTSC  = 0;
           uint64_t  emptyWaitTSC = 0;

  uint64_t tscTimedFunction (function<void(void)> f) {
    auto RDTSC = [] {uint32_t a,d; asm volatile("rdtsc" : "=a"(a),"=d"(d)); return (uint64_t(d)<<32)|uint64_t(a);}; // 22 clocks
    uint64_t start = RDTSC(); 
    f();
    uint64_t stop  = RDTSC(); 
    return start < stop? stop - start: start - stop;  // RDTSC wraps
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

  void producer() { // write
    moveToCore(3, pTid.native_handle(),  "Producer"); // different cores for spinlock not to be pathological
    pAlive = true;

    do {
      // auto waitForEmptyBuffer = [this] { while(bufValid and not stop); };
      auto waitForEmptyBuffer = [this] {
        while(bufValid and not stop);
        // while(bufValid);
      };
      emptyWaitTSC += tscTimedFunction(waitForEmptyBuffer);
      if(stop) break;

      buf++; // Produce buf.
      bufValid = true;
    } while(not stop);
    pAlive = false;
  }

  void consumer() { // read
    moveToCore(2, cTid.native_handle(),  "Consumer"); // different cores for spinlock not to be pathological
    cAlive = true;

    do {
      // auto waitForData = [this] { while(not bufValid and not stop); };
      auto waitForData = [this] {
        while(not bufValid and not stop);
        // while(not bufValid);
      };
      dataWaitTSC += tscTimedFunction(waitForData);
      if(stop) break;

      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      bufValid = false;
    } while(not stop);
    cAlive = false;
  }

public:
  PC() : bufValid(false), stop(false) {cout << "sizeof(PC)=" << sizeof(PC) << "\n"; }

  void run(int ms) {                            // run for 'ms' milliseconds
    pTid = thread(&PC::producer, this);
    cTid = thread(&PC::consumer, this);

    // cout << "p   tid=" << pTid.get_id() << "\n";
    // cout << "p   nh =" << pTid.native_handle() << "\n";
    // cout << "c   tid=" << cTid.get_id() << "\n";
    // cout << "c   nh =" << cTid.native_handle() << "\n";
    // cout << "run tid=" << this_thread::get_id() << "\n";
    // cout << "run tid=" << this_thread::native_handle() << "\n";

    // TYPEID(pTid.get_id())
    // SIZEOF(pTid.get_id())
    // TYPEID(pTid.native_handle())
    // SIZEOF(pTid.native_handle())
    // auto rTid =  this_thread::get_id();
    // cout << "rTid   =" << rTid << "\n";
    // TYPEID(rTid)
    // SIZEOF(rTid)
    // printCoreAffinity(reinterpret_cast<pthread_t> (rTid), "run");

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
