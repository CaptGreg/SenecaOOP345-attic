// approximate near real-time performance in Linux/unix systems
//   1. lock process in core so it will never be swapped out 
//   2. choose FIFO, first in first out real-time schedule policy
//   3. boost priority to max
// NOTE SCHED_DEADLINE support appears to coming...
// /usr/include/linux/sched.h:#define SCHED_DEADLINE		6

#include <errno.h>         //  errno
#include <sys/types.h>     //  getuid, geteuid
#include <ctime>           //  sched_rr_get_interval needs 'struct timespec t;'
#include <memory.h>        //  memset
#include <linux/sched.h>   //  sched_setscheduler, sched_getscheduler, define SCHED_DEADLINE
// #include <sched.h>      //  Header <linux/sched.h> is more complete. Use it instead.
#include <sys/mman.h>      //  mlock
#include <sys/utsname.h>
#include <sys/time.h>      // getpriority, setpriority - get/set program scheduling priority
#include <sys/resource.h>  // getpriority, setpriority - get/set program scheduling priority
#include <unistd.h>        // syscall, getuid, geteuid
#include <sys/syscall.h>   // For SYS_xxx definitions


#include <thread>          // std::thread::hardware_concurrency()
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>         // strerror
#include <map>
#include <vector>
using namespace std;

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
void machineInfo()
{
  cout << __FILE__ <<  " compiled: " << __DATE__<< " " << __TIME__ << "\n";

  cout << "CPU Vender = " << CPUVendor() << ".\n";
  cout << "CPU Model  = " << CPUName() << ".\n";

  cout << "This is a " << 8*sizeof(void*) << "-bit program running on a CPU with " << std::thread::hardware_concurrency()<< " cores.\n";

  char *cmd = (char*) "/usr/bin/lsb_release -a"; // -i ID, -d description, -r release, -c code name, or -a for all
  cout << "'" << cmd << "' output:\n";
  FILE *pipe = popen(cmd, "r");
  if(pipe) {
    char b[1000];
    while(fgets(b, sizeof(b)-1, pipe)) cout << b;
    pclose(pipe);
  }

  // what kernel is this?
  struct utsname utsnameBuf;
  if(uname(&utsnameBuf)) {
    string s = "line " + to_string(__LINE__-1) + ": uname failed ";
    perror(s.c_str());
  } else {
    cout << "kernel info (uts structure)\n";         // strerror
    cout << " Operating system name    : " << utsnameBuf.sysname    << ".\n";
    cout << " Name                     : " << utsnameBuf.nodename   << ".\n";
    cout << " Operating system release : " << utsnameBuf.release    << ".\n";
    cout << " Operating system version : " << utsnameBuf.version    << ".\n";
    cout << " Hardware identifier      : " << utsnameBuf.machine    << ".\n";
  #ifdef _GNU_SOURCE
    cout << " NIS or YP domain name    : " << utsnameBuf.domainname << ".\n";
  #endif
  }

}

// man sched_getattr(2)
// error: use of undeclared identifier 'sched_getattr', and linker error if prototyped
// bad <sched.h> header, sched_getattr syntax errors and not resolved by linker
// <sched> header for sched_getattr, sched_setattr broken
// work-around: use syscall
#if 1
   // dirty fix for syntax error from #include <sched.h> struct sched_attr
   // I'm not happy with this.  What if layout changes?
   // structure layout from man sched_getattr(2) as of Sept 30, 2015
   // from an up-to-date Ubuntu 15.05 install with 3.19.0-28-generic kernel
   // *** Oct 24, 2015 built against a 4.1.10-real-time-patched kernel
   struct sched_attr {
     uint32_t size;              /* Size of this structure */
     uint32_t sched_policy;      /* Policy (SCHED_*) */
     uint64_t sched_flags;       /* Flags */
     int32_t  sched_nice;        /* Nice value (SCHED_OTHER, SCHED_BATCH) */
     uint32_t sched_priority;    /* Static priority (SCHED_FIFO, SCHED_RR) */
     /* Remaining fields are for SCHED_DEADLINE */
   #ifdef SCHED_DEADLINE
     uint64_t sched_runtime;
     uint64_t sched_deadline;
     uint64_t sched_period;
   #endif
   };
#endif

/**
 * DIY sched_getattr via syscall mechanism
 */
int sched_getattr(pid_t pid, const struct sched_attr *attr, unsigned int size, unsigned int flags)
{
  return syscall(SYS_sched_getattr, pid, attr, size, flags);
}

/**
 * print scheduler modes
 */
void supportedModes()
{
  cout << "\n";
  cout << "Supported scheduler modes:\n";
  #define P(T,X) cout << " " << left << setw(12) << #T " " << X \
                      << left << setw(18) << " (" #X ")" << setw(0) \
                      << " priority max = " << sched_get_priority_max(X) << "\n";
    P(normal,SCHED_OTHER)
    P(real-time,SCHED_FIFO)
    P(real-time,SCHED_RR)
    P(normal,SCHED_BATCH)
    P(normal,SCHED_IDLE)
    #ifdef SCHED_DEADLINE
      P(real-time,SCHED_DEADLINE)
    #endif
  #undef P
}

/**
 * print scheduler policy parameters
 */
map<int,string> schedMap ={   // :-) C++11 supports static STL initialization
  #define TABLE(X) {X, #X},
      TABLE(SCHED_OTHER)
      TABLE(SCHED_FIFO)
      TABLE(SCHED_RR)
      TABLE(SCHED_BATCH)
    #ifdef SCHED_ISO
      TABLE(SCHED_ISO)
    #endif
      TABLE(SCHED_IDLE)
    #ifdef SCHED_DEADLINE
      TABLE(SCHED_DEADLINE)
    #endif
  #undef TABLE
};
void printScheduler()
{
  // If pid equals zero, the scheduling policy and parameters of the calling thread will be set.
  pid_t pid = 0; 

  int policy = sched_getscheduler(pid);

  cout << "scheduler NOW in use = " << policy << "  (" << schedMap[policy] << ")\n";

  cout << " priority sched_get_priority_min,sched_get_priority_max = "
       << sched_get_priority_min(policy) <<","
       << sched_get_priority_max(policy) 
       << "\n";

  struct sched_attr sa; // error: variable has incomplete type 'struct sched_attr'
                        // note: forward declaration of 'sched_attr'

  sa.size = sizeof(sa);
  // sa.sched_flags = SCHED_FLAG_RESET_ON_FORK; // only flag supported

  // int sched_getattr(pid_t pid, const struct sched_attr *attr, unsigned int size, unsigned int flags);
  // int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags);
  
  int flags = 0;
  sched_getattr(pid, &sa, sizeof(sa), flags); 
  #define P(X) cout << " " #X "=" << sa.X << "\n";
    // P(sched_policy)
    cout << " sched_policy = " << sa.sched_policy << " (" << schedMap[sa.sched_policy] << ")\n";
    P(sched_flags)
    P(sched_nice)
    P(sched_priority)
  #undef P
  #define P(X) cout << " " << left << setw(15) << #X " " << sa.X << " nanosecs (if DEADLINE scheduler)\n";
    P(sched_runtime)
    P(sched_deadline)
    P(sched_period)
  #undef P
  // sched_getaffinity(2), sched_getattr(2), sched_getparam(2), sched_rr_get_interval(2),
  if(policy == SCHED_RR) {
    struct timespec t;
    sched_rr_get_interval(pid, &t);
    cout << " sched_rr_get_interval = " << t.tv_sec + 1e-9*t.tv_nsec << " secs (nanosec resolution)\n";
  }

  int which;  
  id_t who = 0;
  which = PRIO_PROCESS;

  #define P(X) cout << "    priority " #X "=" << getpriority(X, who) << "\n";
    P(PRIO_PROCESS)
    P(PRIO_PGRP)
    P(PRIO_USER)
  #undef P
}

/**
 * main program
 */
int main(int argc, char ** argv)
{
  uid_t uid=getuid(), euid=geteuid();

  if(uid != 0 && euid != 0) {
    cout<<"uid="<<uid<<", euid="<<euid<<"\n";
    cerr << "Root permissions required to alter scheduling algorithm and raise priority, try \"sudo " << argv[0] << "\"\n";
    return -1;
  }

  machineInfo();
  cout << "\n";

  // in-core lock to prevent swapping
  if(mlockall(MCL_CURRENT | MCL_FUTURE)) {
    perror("mlockall failed :-");
  }

  cout << "Startup DEFAULT scheduler:\n";
  printScheduler();
  cout << "\n";

  supportedModes();
  cout << "\n";

  vector<int> policies
    = {SCHED_OTHER, SCHED_FIFO, SCHED_RR, SCHED_BATCH, SCHED_IDLE, SCHED_DEADLINE, };

  for(auto policy : policies) {
    try {
      cout << "+++++++++++++++++++++++++++++++++++++\n";
      // boost priority to max RT
      struct sched_param sp;
      memset(&sp, 0, sizeof(sp));
      sp.sched_priority = sched_get_priority_max(policy);

      // If pid equals zero, the scheduling policy and parameters of the calling thread will be set.
      pid_t pid = 0; 
      if(sched_setscheduler(pid, policy, &sp)) {
        string s = "line " + to_string(__LINE__-1) + ": sched_setscheduler failed, policy= " + schedMap[policy];
        throw s + ": " + strerror(errno);
      } else {
        cout<<"sched_setscheduler " << schedMap[policy] << " worked!\n"; 
      }
      cout << "\n";

      printScheduler();
      cout << "\n";

      int flags = 0;
      struct sched_attr sa;
      sched_getattr( pid, &sa, sizeof(sa), flags);
      if(sa.sched_priority ==  sched_get_priority_max(policy)) { 
        cout << "SET "<< schedMap[policy] << " PRIORITY WORKED!\n"
             << "The current priority is " << sa.sched_priority 
             << " which is set to the max "<< sched_get_priority_max(policy) 
             <<".\n";
      }
    } catch (std::string& e) {
      cerr << e << "\n";
    }
  }

}
