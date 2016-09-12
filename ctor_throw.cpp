// ctor throws implies the ctor failed, C++ does not need to call dtor, and it doesn't

// The actual mechanics are:
// 1. The ctor allocates space for any static sized private data members.
// 2. the ctor code executes.
// 3. If it throws, the private data elelments are deallocated.

#include <exception>   // std::bad_alloc
#include <cstdlib>     // malloc/free
#include <memory>      // unique_ptr/smart_ptr
#include <new>         // new/delete prototypes 
#include <vector>
#include <iostream>
#include <iomanip>     // scientific notation

#ifdef __linux__
#include <cstdio>      // popen, fread, fclose
#include <sys/time.h>
#include <sys/resource.h> // rlimit(2)
#include <sys/times.h>    // times(2)
// times(2) units:
#include <sys/auxv.h> // unsigned long getauxval(unsigned long type)
// AT_CLKTCK The frequency with which times(2) counts. This value can also be obtained via sysconf(_SC_CLK_TCK).
#include <unistd.h> // long sysconf(int name);
#endif

using namespace std;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// override new/delete with verbose DIY code
bool bVerboseNewDelete = true;  // if true, new/delete is verbose
int64_t mallocBlocks   = 0;     // count of new - delete calls

void* operator new (size_t size)
{
  void *p=malloc(size); 
  if(bVerboseNewDelete) 
    cout << "global DIY new(" << size << ") = " << p << "\n";
  if (!p) // did malloc succeed?
    throw std::bad_alloc(); // ANSI/ISO compliant behavior
  mallocBlocks++;
  return p;
}

void operator delete (void *p)
{
  if(bVerboseNewDelete) 
    cout << "global DIY delete(" << p << ")\n";
  if(p) 
    free(p); 
  mallocBlocks--;
}

void printVM ()
{
#ifdef __linux__
  cout << "\n/proc/meminfo virtual memory Vmalloc details:\n";
  char *cmd = (char*) "/bin/grep Vmalloc /proc/meminfo"; // NOTE: not /usr/bin
  FILE *pipe = popen(cmd, "r");
  if(pipe) {
    static char b[1000];                  // mem maxed out, stack allocation can fail.
    while(fgets(b, sizeof(b)-1, pipe)) 
      cout << b;
    pclose(pipe);
  }
  cout << "\n";
#endif
}

int main()
{
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef __MSVC__   // one of the many, many, many Microsoft visual studio c compilers
  cout << R"msc(
  Everytime I go near Windows, Windows bites me. 
  
  Windows programs execute with a tiny stack.

  This code generates a class with a data element that is a million bytes.
  For encapsulation, these classes are created local to a function, that is, on the stack.

  This causes a segfault if this code is run under windows. :-( :-( :-(

  One can specify the stack size for a thread under windows.  See 
  http://msdn.microsoft.com/en-us/library/windows/desktop/ms686774%28v=vs.85%29.aspx

  One can request Windows to run a program with a realistic stack size.
 
  Here's how:
    1. if using the MSC compiler try the /F <size> compiler option
           to set the stack to 10 MB:
           CL ctor_throw.cpp /F 10000000
    2. set the stack size at run time in the .exe binary file using editbin.exe

    http://www.cs.nyu.edu/exact/core/doc/stackOverflow.txt
    
    Stack Overflow Problems
    =======================
    This file gives some hints on addressing this problem on different platforms.
    Windows (during compilation):
    =============================
    	1. Select "Project->Setting".
    	2. Select "Link" page.
    	3. Select "Category" to "Output".
    	4. Type your preferred stack size in "Reserve:" field under "Stack 
    	   allocations". eg, 32768 in decimal or 0x20000 in hexadecimal.
    
    Windows (to modify the executable file):
    =======================================
    There are two programs included in Microsoft Visual Studio, "dumpbin.exe"
    and "editbin.exe".  
    
    Run "dumpbin /headers executable_file", to see the "size of stack reserve" information 
    in the "optional header values".  
    
    Run "editbin /STACK:size" to change the default stack size.

    I like windows.  One can charge a lot of money to make things work in a windows environment.

    Hope this helps.  Greg
  
  )msc";
  #endif

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  class X {
  uint8_t buf1MB [1000*1000];
  public:
    X()  { cout << "X() - will now throw.\n"; throw nullptr; }
    ~X() { cout << "~X()\n"; };
  };

  cout << "\nnormal instance\n";
  try { X x; }
  catch (...) { cout << "It threw.\n"; }
  cout << "Note: The dtor is never called\n";


  cout << "\nraw pointer to instance.\n";
  X* p = nullptr;
  try { p = new X; cout<< "'new X' succeeded - calling 'delete'\n"; delete p; } 
  catch (...) { cout << "It threw.\n"; }
  cout << "p=" << (void*) p << "\n";
  cout << "Note: The dtor is never called\n";

  cout << "\nunique_ptr to instance.\n";
  try { unique_ptr<X> up ( new X ); } 
  catch (...) { cout << "It threw.\n"; }
  cout << "Note: The dtor is never called\n";

  cout << "\nshared_ptr to instance.\n";
  shared_ptr<X> sp(nullptr);
  cout << "sp=" << (void*) sp.get() << ", use_count=" << sp.use_count() << "\n";
  try { sp = shared_ptr<X>(new X); } 
  catch (...) { cout << "It threw.\n"; }
  cout << "sp=" << (void*) sp.get() << ", use_count=" << sp.use_count() << "\n";
  cout << "Note: The dtor is never called\n";

  cout << "'new calls minus delete calls' = " << mallocBlocks << "\n";

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  class Y { // make the constructor throw
    const size_t TABLE_SIZE  = 1000*1000;      // 1 million pointers
    const size_t BUFFER_SIZE = 1000*1000*1000; // 1 billion byte buffers
    // something wrong with vector constructor w/TABLE_SIZE, but for-loops compile
    // vector<uint8_t*> v(TABLE_SIZE,nullptr);  // error: ‘TABLE_SIZE’ is not a type
    // vector<uint8_t*> v(TABLE_SIZE);          // error: ‘TABLE_SIZE’ is not a type

    vector<uint8_t*> v;                         // WORK-AROUND: use v.reserve(TABLE_SIZE);

  public:
    Y() { 
      // Programs are allowed to allocate a huge amount of RAM and use only a few virtual memory pages 
      // g++ 5.2.1 compiler, x64 Ubuntu 15.10 machine with 8 GB RAM + 16 GB swap space
      // From /proc/meminfo:
      //   MemTotal:        8090196 kB
      //   SwapTotal:      16506828 kB
      //   PageTables:        30308 kB
      //   CommitLimit:    20551924 kB
      //   VmallocTotal:   34359738367 kB
      //   VmallocChunk:   34358947836 kB

      cout << "Y()\n"; 
      size_t i;
      try { 
        // Try to allocate 1e12 bytes.
        // Apparently 'new' fails when the VM page table is full.
        v.reserve(TABLE_SIZE);              // work-around
        for(i = 0; i < TABLE_SIZE; i++) {
          v[i] = new uint8_t[BUFFER_SIZE];  // created in VM

          // no memory access case runs in about 4.5 seconds
          // Y() threw std::bad_alloc, i=140736
          // 'Y y;' threw std::exception
          // real    0m4.647s user    0m0.088s sys     0m4.544s

          // touch memory to force VM page swap
          // v[i][0] = v[i][BUFFER_SIZE/2] =  v[i][BUFFER_SIZE-1] = i;

          // memory access case runs in 90 seconds, about 20 x slower
          // Y() threw std::bad_alloc, i=140736
          // 'Y y;' threw std::exception
          // real    1m30.539s user    0m0.116s sys     1m30.284s

          // interesting that the system allocated 140,736,000,000,000 bytes, 1.4e14 bytes
          // obvious larger than the physically available 2.4e10 RAM + SWAP space
          // running top, the VM grows to .14 petabytes, .14e15=1.4e14:

          // KiB Mem:   8090196 total,  5541844 used,  2548352 free,    21332 buffers
          // KiB Swap: 16506828 total,   565240 used, 15941588 free.   175912 cached Mem
          //   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
          // 27474 greg      20   0  0.140p 3.752g   2796 R  99.9 48.6   0:35.83 ctor_throw

          // System call ulimit sets the VM system limit.

        }
      } catch(std::exception& e) {
        cout << "Y() threw " << e.what() << ", i=" << i << "\n";
          printVM();

        for(size_t ii = 0; ii < i; ii++) {
          delete [] v[ii];
        }
        throw e;
      }
    }
    ~Y() { 
      cout << "~Y()\n"; 
      for(size_t i = 0; i < TABLE_SIZE; i++)
        delete [] v[i];
    };
  };

#ifdef __linux__
  struct rlimit rlim;
  getrlimit(RLIMIT_DATA, &rlim);
  cout << "\nRLIMIT_DATA: "
       << "rlim_cur=" << scientific << double(rlim.rlim_cur) << ","
       << "rlim_max=" << scientific << double(rlim.rlim_max)
       << " bytes\n\n";
#endif
  printVM();

  bVerboseNewDelete = false;  // avoid 1 million new/delete debug prints
  try {
    Y y;
    cout << "'Y y;' did not throw.\n";
  } catch(std::exception& e) {
    cout << "'Y y;' threw " << e.what() << "\n";
  }

  cout << "'new calls minus delete calls' = " << mallocBlocks << "\n";

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef __linux__
  struct tms tmsb;
  clock_t ticks = times(&tmsb);
  cout << "\n"
       << "times system call: " 
       << "user time "   << scientific << tmsb.tms_utime  / double(getauxval(AT_CLKTCK)) << ", "
       << "system time " << scientific << tmsb.tms_stime  / double(getauxval(AT_CLKTCK)) << " sec."
       << "\n";

  cout << "times system call: " 
       << "user time "   << scientific << tmsb.tms_utime  / double(sysconf(_SC_CLK_TCK)) << ", "
       << "system time " << scientific << tmsb.tms_stime  / double(sysconf(_SC_CLK_TCK)) << " sec."
       << "\n";

  cout << "\nresource usage:\n";
  struct rusage usage;
  int who = RUSAGE_SELF;
  getrusage(who, &usage);
  // NOTE:
  // struct timespec { time_t tv_sec; long tv_nsec; };
  // struct timeval  { time_t tv_sec; long tv_usec; };

  //struct timeval ru_utime; /* user CPU time used */
  //struct timeval ru_stime; /* system CPU time used */
  #define PT(V,LABEL) cout << "  rusage." #V "=" << scientific << usage.V.tv_sec+1e-6*usage.V.tv_usec << " - " << LABEL << "\n";
    PT(ru_utime, "user CPU time used")
    PT(ru_stime, "system CPU time used")
  #undef PT

  #define P(V,LABEL) if(usage.V) cout << "  rusage." #V "=" << usage.V << " - " << LABEL << "\n";
    P(ru_maxrss,   "maximum resident set size (KBytes)")
  //P(ru_ixrss,    "integral shared memory size ")        // not used by Linux
  //P(ru_idrss,    "integral unshared data size ")        // not used by Linux
  //P(ru_isrss,    "integral unshared stack size ")       // not used by Linux
    P(ru_minflt,   "page reclaims (soft page faults) ")
    P(ru_majflt,   "page faults (hard page faults) ")
  //P(ru_nswap,    "swaps ")                              // not used by Linux
    P(ru_inblock,  "block input operations ")
    P(ru_oublock,  "block output operations ")
  //P(ru_msgsnd,   "IPC messages sent ")                  // not used by Linux
  //P(ru_msgrcv,   "IPC messages received ")              // not used by Linux
  //P(ru_nsignals, "signals received ")                   // not used by Linux
    P(ru_nvcsw,    "voluntary context switches ")
    P(ru_nivcsw,   "involuntary context switches ")
    #undef P
#endif

}

/*
times system call: user time 1.000000e-01, system time 8.767000e+01

resource usage:
  rusage.ru_utime=1.000000e-01 - user CPU time used
  rusage.ru_stime=8.767600e+01 - system CPU time used
  rusage.ru_maxrss=6342616 - maximum resident set size (KBytes)
  rusage.ru_minflt=348665 - page reclaims (soft page faults)
  rusage.ru_majflt=975 - page faults (hard page faults)
  rusage.ru_inblock=8104 - block input operations
  rusage.ru_nvcsw=982 - voluntary context switches
  rusage.ru_nivcsw=9826 - involuntary context switches

real    1m38.671s
user    0m0.100s
sys     1m28.136s
*/
