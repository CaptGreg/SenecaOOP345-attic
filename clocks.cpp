// NOTE clock_gettime must link with librt, the real time library
// compile with -ltr real time library
// g++ clocks.cpp -o clocks -lrt

#include <iostream>           // cout ...
using namespace std;

#include <cstdio>            // pipe
#include <inttypes.h>        // uint8_t ...
#include <time.h>            // clock_gettime
#include <sys/time.h>        // gettimeofday
#include <sys/utsname.h>     // uname
#include <stdlib.h>          // exit
#include <unistd.h>          // usleep
// #include <rtl_time.h>        // hrtime_t gethrtime(void);

// typedef long long          int64_t;  Not needed.  It is defined by <inttypes.h>

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
  cout << "Vender=" << CPUVendor() << " Model=" << CPUName() << endl;

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
       << endl;

  cout << "application (file " << __FILE__ << ") "
       <<  "compiled: " << __DATE__<< " "
       << __TIME__
       << endl;
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
    /**
     * time in processor clock cycles since power up, NOTE value wraps
     */
    uint64_t u64RDTSC() { 
       // RDTSC 'Read Time Stamp Counter' instruction - google it!
       unsigned a, d; 
       asm volatile("rdtsc" : "=a" (a), "=d" (d)); 
       return ((uint64_t)a) | (((uint64_t)d) << 32); 
    }
    /**
     * hr timer (gethrtime) nsec since power up
     */
    // uint64_t u64TimeHRNS() { 
	// real-time Linux function:- can't locate get_high_resolution_time function 
    	// return (uint64_t) gethrtime(); 
    // }
    /**
     * clock_gettime time in nsec
     */
    uint64_t u64TimeNS() {
      struct timespec tp;
      clock_gettime(0, &tp); // nanosec's since power up
      return BILLION * tp.tv_sec + tp.tv_nsec; // nsec
    }
    /**
     * clock_getcpuclockid time in nsec
     */
    uint64_t u64CpuTimeNS() {
      struct timespec tp;
      clockid_t clockid;

      if (clock_getcpuclockid(getpid(), &clockid) != 0) {
        perror("clock_getcpuclockid");
        exit(EXIT_FAILURE);
      }
      
      clock_gettime(clk_id, &tp); // nanosec's since power up for this CPU
      return BILLION * tp.tv_sec + tp.tv_nsec; // nsec
    }

    /**
     * clock_getres timer resolution in nsec
     */
    uint64_t u64getResNS() {
        struct timespec res;
        clock_getres(clk_id, &res);
        return BILLION * res.tv_sec + res.tv_nsec; // nsec
    }
    
    /**
     * gettimeofday time in usec
     */
    uint64_t u64TimeUS() {
      struct timeval tv;
      gettimeofday(&tv, 0);
      return MILLION * tv.tv_sec + tv.tv_usec; // nsec
    }
};

int main(int argc, char **argv) 
{
    myMachineInfo();
    cout << endl;

    timers t;
    cout << "time resolution = " << t.u64getResNS() << " nanosec." << endl; // nsec

    cout << "'sleep(n)' guarantees to sleep for at least 'n' seconds\n";
    cout << "'usleep(n)' guarantees to sleep for at least 'n' micro-seconds\n";
    cout << endl;


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

    cout << "time for sleep("<< SECSLEEP << ") = " << stopRDTSC  - startRDTSC  << " TSC"  << endl;
    cout << "time for sleep("<< SECSLEEP << ") = " << stopNS     - startNS     << " nsec" << endl;
    cout << "time for sleep("<< SECSLEEP << ") = " << stopCPU_NS - startCPU_NS << " nsec - CPU clock" << endl;
    cout << "time for sleep("<< SECSLEEP << ") = " << stopUS     - startUS     << " usec, " 
         << stopUS - startUS - 1000000 * SECSLEEP  << " usec overhead" << endl;
    cout << endl;

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

        cout << "time for usleep("<< napTime << ") = " << stopRDTSC - startRDTSC  << " TSC"  << endl;
        cout << "time for usleep("<< napTime << ") = " << stopNS    - startNS     << " nsec" << endl;
        cout << "time for usleep("<< napTime << ") = " << stopCPU_NS- startCPU_NS << " nsec - CPU clock" << endl;
        cout << "time for usleep("<< napTime << ") = " << stopUS    - startUS     << " usec, "
             << stopUS - startUS - napTime  << " usec overhead" << endl;

        cout << endl;
    }
}

/* vim: set cin et ts=4 sw=4 cino=>1s,e0,n0,f0,{0,}0,^0,\:1s,=0,g1s,h0,t0,+1s,c3,(0,u0 : */
