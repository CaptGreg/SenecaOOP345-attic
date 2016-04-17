// NOTE clock_gettime must link with librt, the real time library
// compile with -ltr real time library
// g++ clocks.cpp -o clocks -lrt

#include <iostream>           // cout ...
using namespace std;

#include <cstdio>           // pipe
#include <ctime>            // clock_gettime
#include <sys/time.h>       // gettimeofday
#include <sys/utsname.h>    // uname
#include <unistd.h>         // usleep, getpid
// #include <rtl_time.h>        // hrtime_t gethrtime(void);

// typedef long long          int64_t;  Not needed.  It is defined by <inttypes.h>

#include <thread> // usleep needs thread
#include <chrono>
class Timer { // use C++11 std::chrono features to create a stop-watch timer class
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop () { stop  = std::chrono::high_resolution_clock::now(); }
  // basic form to calculate time differences, illustrate with microseconds
  uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t us = duration_get.count();
    return us;
  }
  // Now use a macro to return milli, micro, and nano seconds 
  #define RET(UNITS) uint64_t UNITS##secs() { \
    typedef std::chrono::duration<int,std::UNITS> UNITS##secs_t ; \
    UNITS##secs_t duration_get( std::chrono::duration_cast<UNITS##secs_t>(stop-start) ) ; \
    uint64_t us = duration_get.count(); \
    return us; \
  }
  RET(milli) // creates member function 'uint64_t millisecs()' - which returns 'stop-start' in millisecs
  RET(micro) // creates member function 'uint64_t microsecs()' - which returns 'stop-start' in microsecs
  RET(nano)  // creates member function 'uint64_t nanosecs()'  - which returns 'stop-start' in nanosecs
};

int usleep(unsigned usec) // C++11 std::chrono platform independent usleep()
{
  // NAME
  //      usleep - suspend execution for microsecond intervals
  // SYNOPSIS
  //      #include <unistd.h>
  //      int usleep(useconds_t usec);
  // RETURN VALUE
  //      The usleep() function returns 0 on success.  On error, -1 is returned, with errno set to indicate the cause of the error.
  // ERRORS
  //      EINTR  Interrupted by a signal; see signal(7).
  //      EINVAL usec is not smaller than 1000000.  (On systems where that is considered an error.)
  // NOTES
  //     The  type  useconds_t  is  an  unsigned  integer  type capable of holding 
  //     integers in the range [0,1000000].  Programs will be more portable if 
  //     they never mention this type explicitly.  Use
  //        #include <unistd.h>
  //        ...
  //            unsigned int usecs;
  //        ...
  //            usleep(usecs);

  std::this_thread::sleep_for (std::chrono::microseconds(usec));  // void
}

/**
 * CPUID assembler instruction
 */
static inline void cpuid(uint32_t op, uint32_t *eax, uint32_t *ebx,
  uint32_t *ecx, uint32_t *edx)
{
  asm("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (op) : "cc");
}
static inline uint32_t cpuid_string(uint32_t op, uint32_t r[4]) 
{
  asm volatile("cpuid":"=a"(*r),"=b"(*(r+1)), "=c"(*(r+2)),"=d"(*(r+3)):"a"(op));
  return (uint32_t)r[0];
}

/**
 * retrieve CPU Vender from CPU hardware
 */
char *CPUVendor(void)
{
   uint32_t eax, ebx, ecx, edx;
   static uint32_t Vendor[4];

   eax = 0; cpuid(eax, &eax, &ebx, &ecx, &edx);
   Vendor[0] = ebx; Vendor[1] = edx; Vendor[2] = ecx; Vendor[3] = 0;
   return (char *) Vendor;
}

/**
 * retrieve CPU Name from CPU hardware
 */
char *CPUName(void)
{
  uint32_t eax;
  static uint32_t Name[13];
  //                           &eax      &ebx      &ecx      &edx
  eax = 0x80000002; cpuid(eax, Name + 0, Name + 1, Name + 2, Name + 3);
  eax = 0x80000003; cpuid(eax, Name + 4, Name + 5, Name + 6, Name + 7);
  eax = 0x80000004; cpuid(eax, Name + 8, Name + 9, Name +10, Name +11);
  Name[12] = 0;

  return (char *) Name;
}


/**
 * print machine information
 */
void myMachineInfo()
{
  cout << "Vender=" << CPUVendor() << " Model=" << CPUName() << "\n";

  char *cmd = (char*) "/usr/bin/lsb_release -d"; // -i ID, -d description, -r release, -c code name, or -a for all
  FILE *pipe = popen(cmd, "r");
  if(pipe) {
    char b[1000];
    while(fgets(b, sizeof(b)-1, pipe)) cout << b;
    pclose(pipe);
  }

  struct utsname uname_pointer;
  uname(&uname_pointer);
  cout << uname_pointer.sysname << " " 
       << uname_pointer.machine 
       << " kernel:" << uname_pointer.release
       << " (" << uname_pointer.version << ")"
       << "\n";

  cout << "application (file " << __FILE__ << ") "
       <<  "compiled: " << __DATE__<< " "
       << __TIME__
       << "\n";
}


class timers {
// This class uses 
//   RDTSC (CPU instruction count)
//   clock_gettime (nsec)
//   gettimeofday (usec)
// There are other Linux timers rtc, hwclock, gethrtime
// for Windows, see Windows doc's on QueryPerformanceCounter() 
private:
    static const clockid_t clk_id = CLOCK_MONOTONIC;
    static const long BILLION = 1000000000L;
    static const long MILLION = 1000000L;
public:
    uint64_t u64RDTSC() { // time in processor clock cycles since power up, NOTE value wraps
       // RDTSC 'Read Time Stamp Counter' instruction - google it!
       unsigned a, d; 
       asm volatile("rdtsc" : "=a" (a), "=d" (d)); 
       return ((uint64_t)a) | (((uint64_t)d) << 32); 
    }

    // uint64_t u64TimeHRNS() { // hr timer (gethrtime) nsec since power up
	  // real-time Linux function:- can't locate get_high_resolution_time function 
    //   return (uint64_t) gethrtime(); 
    // }

    uint64_t u64TimeNS() { // clock_gettime time in nsec
      struct timespec tp;
      clock_gettime(0, &tp); // nanosec's since power up
      return BILLION * tp.tv_sec + tp.tv_nsec; // nsec
    }

    uint64_t u64CpuTimeNS() { // clock_getcpuclockid time in nsec
      struct timespec tp;
      clockid_t clockid;

      if (clock_getcpuclockid(getpid(), &clockid) != 0) {
        perror("clock_getcpuclockid");
        exit(EXIT_FAILURE);
      }
      
      clock_gettime(clk_id, &tp); // nanosec's since power up for this CPU
      return BILLION * tp.tv_sec + tp.tv_nsec; // nsec
    }

    uint64_t u64getResNS() { // clock_getres timer resolution in nsec
        struct timespec res;
        clock_getres(clk_id, &res);
        return BILLION * res.tv_sec + res.tv_nsec; // nsec
    }
    
    uint64_t u64TimeUS() { // gettimeofday time in usec
      struct timeval tv;
      gettimeofday(&tv, 0);
      return MILLION * tv.tv_sec + tv.tv_usec; // nsec
    }
};

int main(int argc, char **argv) 
{
    myMachineInfo();
    cout << "\n";

    timers t;
    cout << "time resolution = " << t.u64getResNS() << " nanosec." << "\n"; // nsec

    cout << "'sleep(n)' guarantees to sleep for at least 'n' seconds\n";
    cout << "'usleep(n)' guarantees to sleep for at least 'n' micro-seconds\n";
    cout << "\n";


    uint64_t startRDTSC;
    uint64_t startNS;
    uint64_t startCPU_NS;
    uint64_t startUS;
    uint64_t stopRDTSC;
    uint64_t stopNS;
    uint64_t stopCPU_NS;
    uint64_t stopUS;

#define SECSLEEP  1
    startRDTSC  = t.u64RDTSC();
    startNS     = t.u64TimeNS();
    startUS     = t.u64TimeUS();
    startCPU_NS = t.u64CpuTimeNS();

    sleep(SECSLEEP);  // sleep (sec)

    stopRDTSC  = t.u64RDTSC();
    stopNS     = t.u64TimeNS();
    stopUS     = t.u64TimeUS();
    stopCPU_NS = t.u64CpuTimeNS();

    cout << "time for sleep("<< SECSLEEP << ") = " << stopRDTSC  - startRDTSC  << " TSC"  << "\n";
    cout << "time for sleep("<< SECSLEEP << ") = " << stopNS     - startNS     << " nsec" << "\n";
    cout << "time for sleep("<< SECSLEEP << ") = " << stopCPU_NS - startCPU_NS << " nsec - CPU clock" << "\n";
    cout << "time for sleep("<< SECSLEEP << ") = " << stopUS     - startUS     << " usec, " 
         << stopUS - startUS - 1000000 * SECSLEEP  << " usec overhead" << "\n";
    cout << "\n";

    for(int napTime = 5; napTime <= 1000000; napTime *=10) {
        startRDTSC  = t.u64RDTSC();
        startNS     = t.u64TimeNS();
        startUS     = t.u64TimeUS();
        startCPU_NS = t.u64CpuTimeNS();

        usleep(napTime);  // microsecond sleep (usec)
    
        stopRDTSC  = t.u64RDTSC();
        stopNS     = t.u64TimeNS();
        stopUS     = t.u64TimeUS();
        stopCPU_NS = t.u64CpuTimeNS();

        cout << "time for usleep("<< napTime << ") = " << stopRDTSC - startRDTSC  << " TSC"  << "\n";
        cout << "time for usleep("<< napTime << ") = " << stopNS    - startNS     << " nsec" << "\n";
        cout << "time for usleep("<< napTime << ") = " << stopCPU_NS- startCPU_NS << " nsec - CPU clock" << "\n";
        cout << "time for usleep("<< napTime << ") = " << stopUS    - startUS     << " usec, "
             << stopUS - startUS - napTime  << " usec overhead" << "\n";

        cout << "\n";
    }
}

/* vim: set cin et ts=4 sw=4 cino=>1s,e0,n0,f0,{0,}0,^0,\:1s,=0,g1s,h0,t0,+1s,c3,(0,u0 : */
