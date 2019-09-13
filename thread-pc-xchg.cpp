// thread-pc-simple.cpp - a simple producer consumer threading model
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on bufValid flag for buffer empty or available

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for + chrono time units
using namespace std;

// http://lxr.linux.no/#linux+v2.6.38/arch/x86/include/asm/cmpxchg_64.h
int32_t atomic_xchg(uint32_t *ptr, uint32_t val)
{
    uint32_t tmp = val;
    asm volatile (
        "xchgl %0, %1;\n"  // b,w,l,q instruction suffix for 8, 16, 32, 64 bit data operand
        : "=r"(tmp), "+m"(*ptr)
        : "0"(tmp)
        :"memory");
    return tmp;
}

void atomic_inc(int32_t *ptr)
{
    // lock turns next instruction into atomic
    asm volatile ("lock; incl %0;\n"
        : "+m"(*ptr));
}

// http://lxr.linux.no/#linux+v2.6.38/arch/x86/include/asm/cmpxchg_64.h
// Note: no "lock" prefix even on SMP: xchg always implies lock anyway.
// Since this is generally used to protect other memory information, we
// use "asm volatile" and "memory" clobbers to prevent gcc from moving
// information around.
uint32_t xchg(uint32_t *ptr, uint32_t x)
{
  volatile uint32_t *__ptr = (volatile uint32_t *)(ptr);
  __typeof(*(ptr)) __x = (x);
  asm volatile("xchgl %0,%1"
    : "=r" (__x), "+m" (*__ptr)
    : "0" (__x)
    : "memory");         
  return __x;
}

// http://lxr.linux.no/#linux+v2.6.38/arch/x86/include/asm/cmpxchg_64.h
// Atomic compare and exchange.  Compare OLD with MEM, if identical,
// store NEW in MEM.  Return the initial value in MEM.  Success is
// indicated by comparing RETURN with OLD.
int32_t raw_cmpxchg(uint32_t *ptr, uint32_t OLD, uint32_t NEW) // use 'NEW' since new is c++ keyword
{
  __typeof__(*(ptr)) __ret;                                       \
  __typeof__(*(ptr)) __old = (OLD);                               \
  __typeof__(*(ptr)) __new = (NEW);

   volatile uint32_t *__ptr = (volatile uint32_t *)(ptr);
   asm volatile("lock;" "cmpxchgl %2,%1"
     : "=a" (__ret), "+m" (*__ptr)
     : "r" (__new), "0" (__old)
     : "memory");      
   return __ret;
}


class PC {
  volatile bool      bufValid     = false; // true data available for consumer, false producer needs to create data
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

  void producer() {
    pAlive = true;
    do {
      auto waitForEmptyBuffer = [this] {while(bufValid and not stop);};
      emptyWaitTSC += tscTimedFunction(waitForEmptyBuffer);
      if(stop) break;

      buf++; // Produce buf.
      bufValid = true;
    } while(not stop);
    pAlive = false;
  }

  void consumer() {
    cAlive = true;
    do {
      auto waitForData = [this] {while(not bufValid and not stop);};
      dataWaitTSC += tscTimedFunction(waitForData);
      if(stop) break;

      // There is data in the buffer.  Do something with it.
      // cout << buf << "\n"; // print it 
      bufValid = false;
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
