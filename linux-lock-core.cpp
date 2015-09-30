// approximate near real-time performance in Linux/unix systems
//   1. lock process in core so it will never be swapped out 
//   2. choose FIFO, first in first out real-time schedule policy
//   3. boost priority to max
// NOTE SCHED_DEADLINE support appears to coming...

#include <errno.h>         //  errno
#include <unistd.h>        //  sleep
#include <stdlib.h>        //  exit
#include <sys/types.h>     //  getuid, geteuid
#include <time.h>          //  time
#include <memory.h>        //  memset
#include <sched.h>         //  sched_setscheduler, sched_getscheduler
#include <sys/mman.h>      //  mlock
#include <sched.h>         //  sched_setattr, sched_getattr 


// <sched> header for sched_getattr, sched_setattr broken
// work-around: use syscall
// #define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <iostream>
using namespace std;


int main(int argc, char ** argv)
{
  uid_t uid=getuid(), euid=geteuid();

  if(uid != 0 && euid != 0) {
    cout<<"uid="<<uid<<", euid="<<euid<<"\n";
    cerr << "Root permissions required to alter scheduling algorithm and raise priority, try \"sudo " << argv[0] << "\"\n";
    return -1;
  }

  // in-core lock to prevent swapping
  mlockall(MCL_CURRENT | MCL_FUTURE);

  // boost priority to max RT
  cout << "\n";
  #define P(T,X) cout << X << " (" #X ") priority max = " << sched_get_priority_max(X) << " (" << #T << ")\n";
    P(normal,SCHED_OTHER)
    P(normal,SCHED_BATCH)
    P(normal,SCHED_IDLE)
    P(real-time,SCHED_FIFO)
    P(real-time,SCHED_RR)
    #ifdef SCHED_DEADLINE
      P(real-time,SCHED_DEADLINE)
    #endif
    #undef P
  cout << "\n";

  // If pid equals zero, the scheduling policy and parameters of the calling thread will be set.
  pid_t pid = 0; 

  int s = sched_getscheduler(pid);
  cout << "STARTUP DEFAULT scheduler in use = " << s << "  (";
  switch(s) {
    case SCHED_OTHER:     cout<<"SCHED_OTHER";       break;
    case SCHED_BATCH:     cout<<"SCHED_BATCH";       break;
    case SCHED_IDLE:      cout<<"SCHED_IDLE";        break;
    case SCHED_FIFO:      cout<<"SCHED_FIFO";        break;
    case SCHED_RR:        cout<<"SCHED_RR";          break;
    #ifdef SCHED_DEADLINE
    case SCHED_DEADLINE:  cout<<"SCHED_DEADLINE";    break;
    #endif
  }
  cout << ")  priority min,max = " << sched_get_priority_min(pid) <<","<< sched_get_priority_max(pid) << "\n";

  int policy = SCHED_FIFO;

  struct sched_param sp;
  memset(&sp, 0, sizeof(sp));
  sp.sched_priority = sched_get_priority_max(policy);


  if(sched_setscheduler(pid, policy, &sp)==0) cout<<"sched_setscheduler worked!\n"; else perror("sched_setscheduler");

  s = sched_getscheduler(pid);
  cout << "scheduler NOW in use = " << s << "  (";
  switch(s) {
    case SCHED_OTHER:  cout<<"SCHED_OTHER"; break;
    case SCHED_BATCH:  cout<<"SCHED_BATCH"; break;
    case SCHED_IDLE:   cout<<"SCHED_IDLE";  break;
    case SCHED_FIFO:   cout<<"SCHED_FIFO";  break;
    case SCHED_RR:     cout<<"SCHED_RR";    break;
  }
  cout << ")  priority min,max = " << sched_get_priority_min(pid) <<","<< sched_get_priority_max(pid) << "\n";

  // man sched_getattr(2)
  // bad <sched.h> header, sched_getattr syntax errors and not resolved by linker
  #if 1
       // dirty fix for weird syntax error from #include <sched.h> struct sched_attr
       // I'm not happy with this.  What if layout changes?
       // structure layout from man sched_getattr(2) as of Sept 30, 2015
       // from an up-to-date Ubuntu 15.05 install with 3.19.0-28-generic kernel
           struct sched_attr {
               uint32_t size;              /* Size of this structure */
               uint32_t sched_policy;      /* Policy (SCHED_*) */
               uint64_t sched_flags;       /* Flags */
               int32_t  sched_nice;        /* Nice value (SCHED_OTHER, SCHED_BATCH) */
               uint32_t sched_priority;    /* Static priority (SCHED_FIFO, SCHED_RR) */
               /* Remaining fields are for SCHED_DEADLINE */
               uint64_t sched_runtime;
               uint64_t sched_deadline;
               uint64_t sched_period;
           };
  #endif

  struct sched_attr sa; // error: variable has incomplete type 'struct sched_attr'
                        // note: forward declaration of 'sched_attr'

  sa.size        = sizeof(sa);
  // sa.sched_flags = SCHED_FLAG_RESET_ON_FORK; // only flag supported

  // int sched_getattr(pid_t pid, const struct sched_attr *attr, unsigned int size, unsigned int flags);
  // int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags);
  
  int flags = 0;
  // sched_getattr(pid, &sa, sizeof(sa), flags); // error: use of undeclared identifier 'sched_getattr', and linker error if prototyped
  syscall(SYS_sched_getattr, pid, &sa, sizeof(sa), flags); // work-around: use syscall to make sched_getattr call
  #define P(X) cout << #X "=" << sa.X << "\n";
    P(sched_policy)
    P(sched_flags)
    P(sched_nice)
    P(sched_priority)
    P(sched_runtime)
    P(sched_deadline)
    P(sched_period)
  #undef P

  if(sa.sched_priority ==  sched_get_priority_max(policy)) { 
    cout << "SET REAL_TIME PRIORITY WORKED!\n";
    cout << "The current priority is " << sa.sched_priority << " which is set to the max "<< sched_get_priority_max(policy) <<".\n";
  }

}
