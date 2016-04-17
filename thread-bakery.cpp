// https://matthewarcus.wordpress.com/2014/06/10/at-the-bakery/

// GB March 27, 2016 convert to C++11 threads
// GB see
// https://en.wikipedia.org/wiki/Lamport's_bakery_algorithm
// http://research.microsoft.com/en-us/um/people/lamport/pubs/pubs.html#bakery
// http://research.microsoft.com/en-us/um/people/lamport/pubs/bakery.pdf
// http://mykeepit.blogspot.ca/2013/11/study-and-implement-lamports-bakery.html
// http://mykeepit.blogspot.ca/2013/11/study-and-implement-lamports-bakery.html
// (Study and implement the Lamport’s Bakery Algorithm for Interprocess synchronization using C/C++ programming language)
//

#include <thread>
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

int usleep(unsigned usec) // A DIY C++11 std::chrono platform independent usleep()
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
  return 0;
}

#define NUMBER1
// #define NUMBER2
// #define NUMBER3

#ifdef NUMBER1
#include <thread>      // GB C++11
#include <mutex>       // GB C++11
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
 
// Compile with: g++ -Wall -O3 bakery.cpp -pthread -o bakery
 
static const int NTHREADS = 6;      // GB On the 6-core AMD1100T processor, doesn't print if NTHREADS > 6
volatile bool    exitFlag = false;  // GB shutdown threads
 
// Some features to play with

//#define NCHECK     // Disable crucial check
//#define NSYNC      // Disable memory barrier
//#define NVOLATILE  // Disable volatile
 
#if defined NVOLATILE
#define VOLATILE
#else
#define VOLATILE volatile
#endif
 
VOLATILE bool     entering[NTHREADS];
VOLATILE unsigned number  [NTHREADS];
VOLATILE int count = 0;
VOLATILE int total = 0;
 
unsigned getmax(int n)
{
  unsigned max = 0;
  for (int i = 0; i < n; i++) {
    if (number[i] > max) max = number[i];
  }
  return max;
}
 
bool check(int i, int j)
{
  return number[j] < number[i] || 
         (number[j] == number[i] && j < i);
}
 
inline void synchronize()
{
#if !defined NSYNC
    // gcc builtin full memory barrier
  __sync_synchronize();
#endif
}
 
void lock(int i) {
  entering[i] = true;
  synchronize();
  // Simulate non-atomic write
  number[i] = rand();  // GB, mutex is about 2 x as fast,  
  // number[i] = 1; // rand();  // GB, mutex is about 2 x as fast,  
  // eliminating the rand() call dropped the wait time from 4100-4800 ns to 3600 ns
  // mutex is 2300 ns

  synchronize();
  number[i] = 1 + getmax(NTHREADS);
  assert(number[i] > 0);
  entering[i] = false;
  synchronize();
  for (int j = 0; j < NTHREADS; j++) {
    // Wait until thread j receives its number:
#if !defined NCHECK
    while (entering[j]) { /* nothing */ }
#endif
    // At this point, we have read a false value for 
    // "entering[j]", therefore any number picked by j
    // later will takes our choice into account, any value
    // chosen earlier (and so might be less than ours) 
    // will be visible to us in the following test.
 
    // Wait until all threads with smaller numbers or with 
    // the same number, but with higher priority, finish 
    // their work:
    while ((number[j] != 0) && check(i,j)) { /* nothing */ }
  }
}
 
void unlock(int i)
{
  number[i] = 0;
}
 
void *threadfun(void *arg) 
{
  Timer t;
  uint64_t nanosecs =0;
  uint64_t calls    =0;
  int i = *(int*)(arg);
  while (true) {
    if(exitFlag) break;
  t.Start();
    lock(i);
  t.Stop();
    nanosecs+= t.nanosecs();
    calls++;
    total++;   // global
    if (total % 1000000 == 0) fprintf(stderr,"%c", 'a'+i);
    assert(count==0); // Check we have exclusive access
    count++;   // global
    // It's not clear that these syncs are unnecessary,
    // but nothing seems to break if I remove them.
    //synchronize();
    count--;   // global
    //synchronize();
    unlock(i);
    // non-critical section...
  }
t.Start();
  lock(i);
t.Stop();
  nanosecs+= t.nanosecs();
  calls++;
    std::cout << "thread " << i
              << " " << nanosecs << " nsec waiting" 
              << ", " << calls  << " calls"
              << ", " << nanosecs / calls << " nsec/call"
              << "\n";
  unlock(i);
  return NULL;
}

std::mutex mtx;
void *threadfunMutex(void *arg) 
{
  Timer t;
  uint64_t nanosecs =0;
  uint64_t calls    =0;
  int i = *(int*)(arg);
  while (true) {
    if(exitFlag) break;
  t.Start();
    mtx.lock();
  t.Stop();
    nanosecs+= t.nanosecs();
    calls++;
    total++;   // global
    if (total % 1000000 == 0) fprintf(stderr,"%c", 'a'+i);
    assert(count==0); // Check we have exclusive access
    count++;   // global
    // It's not clear that these syncs are unnecessary,
    // but nothing seems to break if I remove them.
    //synchronize();
    count--;   // global
    //synchronize();
    mtx.unlock();
    // non-critical section...
  }
t.Start();
  mtx.lock();
t.Stop();
  nanosecs+= t.nanosecs();
  calls++;
    std::cout << "thread " << i
              << " " << nanosecs << " nsec waiting" 
              << ", " << calls  << " calls"
              << ", " << nanosecs / calls << " nsec/call"
              << "\n";
  mtx.unlock();
  return NULL;
}
 

int main()
{
  std::thread t[NTHREADS];
  int n[NTHREADS];
  for (int i = 0; i < NTHREADS; i++) {
    n[i] = i;
    t[i] = std::thread(threadfun, (void*)&n[i]);
  }
  usleep(60*1000*1000);
  exitFlag= true;
  std::cout << "\n";
  for (int i = 0; i < NTHREADS; i++) {
    t[i].join();
  }

  // See what has less overhead: bakery or mutex
  std::cout << "\n";
  exitFlag= false;
  for (int i = 0; i < NTHREADS; i++) {
    n[i] = i;
    t[i] = std::thread(threadfunMutex, (void*)&n[i]);
  }
  usleep(60*1000*1000);
  exitFlag= true;
  std::cout << "\n";
  for (int i = 0; i < NTHREADS; i++) {
    t[i].join();
  }

}

#endif
#ifdef NUMBER2
// http://mykeepit.blogspot.ca/2013/11/study-and-implement-lamports-bakery.html
// (Study and implement the Lamport’s Bakery Algorithm for Interprocess synchronization using C/C++ programming language)
#include<exception>   // GB
#include<iostream>    // GB
#include<thread>      // GB C++11 threads
// #include<pthread.h>   // GB for kill thread  *** compiles without header
#include<csignal>    // GB for kill thread  (SIGTERM)
#include<cstdio>
#include <cassert>
volatile int NUM_THREADS = 10;
volatile int Number[11] = {0};    // GB bump to 11, indexing is base 0? looks like it might be base 1 in places
volatile int count_cs[11] = {0};
volatile int Entering[11] = {0};

volatile bool exitFlag = false;

int Max()
{
  int i = 0;
  // int j = 0;
  int maxvalue = 0;
  for(i = 0; i < 10; i++) {
    if ((Number[i]) > maxvalue) {
      maxvalue = Number[i];
    }
  }
  return maxvalue;
}
void Lock(int i)
{
  Entering[i] = 1;
  Number[i] = 1 + Max();
  Entering[i] = 0;
  for (int j = 1; j <= NUM_THREADS; j++) {
    while (Entering[j]) { } /* Do nothing */
    while ((Number[j] != 0) 
    && (Number[j] < Number[i] ||
       (Number[j] == Number[i] && j < i))) 
    { }
  }
}
void Unlock(int i) {
  Number[i] = 0;
}
void Thread(void* voidPtr)
{
  int i = *(int*) voidPtr;
  while (1) {
    if(exitFlag) break;
    Lock(i);
    count_cs[i+1] = count_cs[i+1] + 1 ;
    //printf("critical section of %d\n", i+1);
    Unlock(i);
  }
}
int main()
{
  try {
    int duration = 5000;
    std::thread threads[NUM_THREADS];
    for(int t = 0; t < NUM_THREADS; t++){
      printf("In main: creating thread %d\n", t+1);
      threads[t] =  std::thread(Thread, (void*)&t);
    }
    usleep(duration*1000);
    for(int t=0; t < NUM_THREADS; t++) {
      printf("count of thread no %d is %d\n",t+1,count_cs[t+1]);
    }

    usleep(duration*1000);
    for(int t=0; t < NUM_THREADS; t++) {
      printf("count of thread no %d is %d\n",t+1,count_cs[t+1]);
    }

    exitFlag = true;
    std::cout << " GB waiting for threads to exit\n";
    usleep(1000*1000);
    std::cout << " GB kill threads\n";
    for(int t=0; t < NUM_THREADS; t++)
      pthread_kill(threads[t].native_handle(), SIGTERM);

    std::cout << " GB join threads\n";
    for(int t=0; t < NUM_THREADS; t++)
      threads[t].join();

  } catch(const std::exception& e) {
    std::cerr << e.what() << "\n";
  }
  std::cout << " GB main finished.\n";
  return 0;
}
#endif

#ifdef NUMBER3
// http://www.csee.wvu.edu/~jdm/classes/cs550/notes/tech/mutex/Bakery.html
// A Novel N-Process Solution: Lamport's Bakery Algorithm
// The bakery algorithm is a very different approach proposed by Leslie Lamport. It's based on the "take-a-number" system used in bakeries and delicatessens.

// CONCEPT: A process waiting to enter its critical section chooses a number. This number must be greater than all other numbers currently in use. There is a global shared array of current numbers for each process. The entering process checks all other processes sequentially, and waits for each one which has a lower number. Ties are possible; these are resolved using process IDs.

#include <thread>
#include <mutex>
#include <iostream>
#include <csignal>    // sigterm
#include <cstring>    // strsignal

const int NUM_THREADS = 4;
volatile bool choosing[NUM_THREADS];
volatile int num[NUM_THREADS];

void lockInit()
{
    for (int j=0; j < NUM_THREADS; j++) {
      num[j] = 0;
    }
}

void lock(int i)
{
	/* choose a number */
	choosing[i] = true;
	// num[i] = max(num[0], ..., num[NUM_THREADS-1]) + 1;
  int max = num[0]; 
  for(int i = 1; i < NUM_THREADS ; i++) if(num[i] > max) max = num[i];
  num[i] = max+1;

	choosing[i] = false;
	
	/* for all other processes */
	for (int j=0; j < NUM_THREADS; j++) {
	
		/* wait if the process is currently choosing */
		while (choosing[j]) 
      {std::this_thread::yield();}
		
		/* wait if the process has a number and comes ahead of us */
		if ((num[j] > 0) &&
		  ((num[j] < num[i]) ||
		  (num[j] == num[i]) && (j < i))
    ) {
			while (num[j] > 0)
        {std::this_thread::yield();}
		}
	}
}
		
void unlock(int i)
{
	num[i] = 0; /* clear our number */
}

volatile int count_cs[11] = {0};
volatile bool exitFlag = false;

std::mutex m;
void Thread(void* voidPtr)
{
  int i = *(int*) voidPtr;

  // 1. capture 'i' as [i], [&], or [=] ---  no error on capture
  // 2. using 'i' generates syntax error
  signal(SIGTERM, [&] (int sig) {m.lock();std::cout << "thread " << ": sig=" << sig << " " << strsignal(sig) << "\n";m.unlock(); exit(sig);});
  // signal(SIGTERM, [&] (int sig) {std::cout << "thread " << i << ": sig=" << sig << " (SIGTERM)\n"; exit(sig);});

  // signal(SIGTERM, [&] (int sig) { exit(sig); } );

  while (1) {
    if(exitFlag) break;
    lock(i);
    count_cs[i+1] = count_cs[i+1] + 1 ;
    // printf("critical section of %d\n", i+1);
    unlock(i);
  }
lock(i);
  std::cout << "thread " << i << " exiting.\n";
unlock(i);
}

int main(int argc,char*argv[])
{
  try {
    int duration = 5000;
    std::thread threads[NUM_THREADS];
    lockInit();
    for(int t = 0; t < NUM_THREADS; t++){
      printf("In main: creating thread %d\n", t+1);
      threads[t] =  std::thread(Thread, (void*)&t);
    }
    usleep(duration*1000);
    for(int t=0; t < NUM_THREADS; t++) {
      printf("count of thread no %d is %d\n",t+1,count_cs[t+1]);
    }

    usleep(duration*1000);
    for(int t=0; t < NUM_THREADS; t++) {
      printf("count of thread no %d is %d\n",t+1,count_cs[t+1]);
    }

    exitFlag = true;
    std::cout << " GB waiting for threads to exit\n";
    usleep(1000*1000);
    std::cout << " GB kill threads\n";
    for(int t=0; t < NUM_THREADS; t++)
      pthread_kill(threads[t].native_handle(), SIGTERM);

    std::cout << " GB join threads\n";
    for(int t=0; t < NUM_THREADS; t++)
      threads[t].join();

  } catch(const std::exception& e) {
    std::cerr << e.what() << "\n";
  }
  std::cout << " GB main finished.\n";
  return 0;
}
#endif
