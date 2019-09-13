// thread-pc-spinlock.cpp - a simple producer consumer threading model
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on bufValid flag for buffer empty or available
// Algorithm and author Greg Blair


// check out MPMC http://moodycamel.com/blog/2014/a-fast-general-purpose-lock-free-queue-for-c++
// check out MPMC attic/queuelockfree/concurrentqueue-master.zip
// check out SPSC attic/queuelockfree/readerwriterqueue-master.zip

// MPMC multiple processor multiple consumer
// SPSC single   processor single   consumer

#include <atomic>
#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;

class SpinLock { // https://codereview.stackexchange.com/questions/84148/spinlock-for-c-kernel-with-x86-asm
public:
  SpinLock() noexcept : lock_(ATOMIC_FLAG_INIT) {}

  SpinLock(const SpinLock &) = delete;
  SpinLock &operator=(const SpinLock &) = delete;


  void Lock() noexcept {
    while (lock_.test_and_set(std::memory_order_acquire))
      asm volatile("pause");
  }

  void Unlock() noexcept { lock_.clear(std::memory_order_release); }

private:
  std::atomic_flag lock_;
};

void spin_lock(int volatile *p) // https://stackoverflow.com/questions/6935442/x86-spinlock-using-cmpxchg
{
    while(!__sync_bool_compare_and_swap(p, 0, 1))
    {
        // spin read-only until a cmpxchg might succeed
        while(*p)asm volatile("pause");  // or maybe do{}while(*p) to pause first
    }
}

class PC {

  void  pause() {asm volatile("pause");}  // see Intel docs w.r.t. spinloops usage
  // The PAUSE instruction pauses speculative execution and yields execution resources to the other (hw) thread (if present)
  // https://software.intel.com/en-us/articles/benefitting-power-and-performance-sleep-loops
  // https://software.intel.com/en-us/articles/long-duration-spin-wait-loops-on-hyper-threading-technology-enabled-intel-processors

#if 1
  // buf=4521981, 4.52198e+06
  // producer wasted clocks  = 1312837972, 1.31284e+09
  // consumer wasted clocks  = 1314973446, 1.31497e+09
  // producer wasted clock per critical region access = 290.324
  // consumer wasted clock per critical region access = 290.796
  // TOTAL    wasted clock per critical region access = 581.12

  volatile bool alignas(128) bufValid     = false; // true data available for consumer, false producer needs to create data
  void clr(volatile bool*p)     { *p = false;          }
  void set(volatile bool*p)     { *p = true;           }
  void waitUntilSet(volatile bool*p) { while(*p != true  and not stop) pause(); } 
  void waitUntilClr(volatile bool*p) { while(*p != false and not stop) pause(); } 
#endif

#if 0
  // buf=3664400, 3.6644e+06
  // producer wasted clocks  = 950011684, 9.50012e+08
  // consumer wasted clocks  = 1095202834, 1.0952e+09
  // producer wasted clock per critical region access = 259.254
  // consumer wasted clock per critical region access = 298.876
  // TOTAL    wasted clock per critical region access = 558.131

  volatile atomic<bool>  alignas(128)    bufValid     = false; // true data available for consumer, false producer needs to create data
  void clr(volatile atomic<bool>*p)     { p->store(false);          }
  void set(volatile atomic<bool>*p)     { p->store(true);           }
  void waitUntilSet(volatile atomic<bool>*p) { while(p->load() != true  and not stop) pause(); } 
  void waitUntilClr(volatile atomic<bool>*p) { while(p->load() != false and not stop) pause(); } 
#endif

#if 0
  // std::atomic_flag

  // atomic_flag_test_and_set          *** atomically sets the flag to true and returns its previous value
  // atomic_flag_test_and_set_explicit

  // atomic_flag_clear                 *** atomically sets the value of the flag to false
  // atomic_flag_clear_explicit

  // ATOMIC_FLAG_INIT                  *** initializes an std::atomic_flag to false

  // http://en.cppreference.com/w/cpp/atomic/atomic_flag
  // std::atomic_flag lock = ATOMIC_FLAG_INIT;
  // while (lock.test_and_set(std::memory_order_acquire))  // acquire lock
  //   ; // spin
  // lock.clear(std::memory_order_release);                // release lock

  atomic_flag        bufValid     = ATOMIC_FLAG_INIT; // true data available for consumer, false producer needs to create data
  void clr(auto*p) { atomic_flag_clear(p);                                                      } // set false
  void set(auto*p) { atomic_flag_test_and_set_explicit(p, std::memory_order_acquire);           } // set true
  void waitUntilSet(auto*p) {while(atomic_flag_test_and_set_explicit(p, std::memory_order_acquire)); } // returns previous value
  void waitUntilClr(auto*p) {while(not atomic_flag_test_and_set_explicit(p, std::memory_order_acquire)); }
#endif

#ifdef USE_VOLATILE
  volatile bool      pAlive       = false;
  volatile bool      cAlive       = false;
  volatile bool  alignas(128)    stop         = false;
#else
  atomic<bool>       cAlive       = false;
  atomic<bool>       pAlive       = false;
  atomic<bool>   alignas(128)    stop         = false;
#endif
           int       buf          = 0;
           thread    pTid;
           thread    cTid;
           uint64_t  dataWaitTSC  = 0;
           uint64_t  emptyWaitTSC = 0;

  uint64_t tscTimedFunction (function<void(void)> f) {
    auto RDTSC = [] {uint32_t a,d; asm volatile("rdtsc" : "=a"(a),"=d"(d)); return (uint64_t(d)<<32)|uint64_t(a);}; // 22 clocks
    uint64_t start = RDTSC(); 
    f();
    uint64_t stop  = RDTSC(); 
    return start < stop? stop - start: start - stop;  // RDTSC wraps
  }  

  void producer() { // write
    moveToCore(thread::hardware_concurrency() == 4? 3: 1, pTid.native_handle()); // different cores for spinlock not to be pathological
    pAlive = true;
    do {
      // auto waitForEmptyBuffer = [this] {while(bufValid and not stop);};
      auto pWait = [this] {waitUntilClr(&bufValid);};
      emptyWaitTSC += tscTimedFunction(pWait);
      if(stop) break;

      buf++; // Produce buf.
      set(&bufValid);
    } while(not stop);
    pAlive = false;
  }

  void consumer() { // read
    moveToCore(thread::hardware_concurrency() == 4? 2: 0, cTid.native_handle()); // different cores for spinlock not to be pathological
    cAlive = true;
    do {
      // auto waitForData = [this] {while(not bufValid and not stop);};
      auto cWait = [this] {waitUntilSet(&bufValid);};
      dataWaitTSC += tscTimedFunction(cWait);
      if(stop) break;

      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      clr(&bufValid);
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
  std::cout << argv[0] << ":\n";
  PC pc;

  pc.run(1000);
  return 0;
}
