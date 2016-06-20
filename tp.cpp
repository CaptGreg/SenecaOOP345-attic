#include "tp.h"

ThreadPool::ThreadPool(int numberOfThreads)
  : quit(false), stopped(false)
{
  for(int t=0; t < numberOfThreads; t++)
    threads.emplace_back( &ThreadPool::Run, this );
}

ThreadPool::~ThreadPool() { if (!stopped)  ShutDown(); }

void ThreadPool::ShutDown()
{
  { 
    std::lock_guard<std::mutex> smartLock (mtx); //  lock the thread pool
    quit = true;                                  //  set the 'quit' flag
  }  
  cv.notify_all();                              //  wake up all threads

  for(auto& e : threads) e.join();                // join all threads: wait for threads to exit

  stopped = true;                                 // set the stopped flag
}


void ThreadPool::AddJob(std::function<void(void)> job) // a 'void(void)' function pointer
{
  if(stopped)      
    throw "thread pool shutdown, not accepting jobs.";
  if(quit)         
    throw "thread pool shutting down, not accepting jobs.";

  { 
    std::lock_guard<std::mutex> smartLock (mtx);  //  lock the thread pool
    jobQueue.push(job);                           //  push 'job' onto 'jobQueue'
  }
  cv.notify_one();                                //  wake up a sleeping thread
}

void ThreadPool::Run()                            //  private member function
{
  std::function<void(void)> job;                  // 'void(void)' function pointer to 'job'

  while(true) {
    { 
      std::unique_lock<std::mutex> smartLock (mtx); //  lock the thread pool with unique_lock

      cv.wait(smartLock, [this] () -> bool { return !jobQueue.empty() || quit; } );

      if(quit && jobQueue.empty())                //  if the quit flag is set and there aren't any jobs
        return;

      job = jobQueue.front();                     //  pull a job off of the jobQueue
      jobQueue.pop();                             //  pop
    }
    job();                                        //  run 'job'
  }  
}

// #define TEST
#ifdef TEST
#include <iostream>
#include <chrono>
#include <mutex>
#include <sstream>
using namespace std;

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t msecs() {
    typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    return duration_get.count();
  }
  uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    return duration_get.count();
  }
  uint64_t nsecs() {
    typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    return duration_get.count();
  }
};

int main()
{
  cout << "This machine has " << thread::hardware_concurrency()  << " cores.\n";
  // ThreadPool tp( 2*thread::hardware_concurrency() );
  ThreadPool tp;
  mutex coutLock;
  Timer t;
  t.Start();
  t.Stop();
  cout << t.usecs() << ": -PROGRAM START-\n";

  for(int j = 0; j < 17; j++) {
    tp.AddJob( 
      [j, &coutLock, &t] ()-> void {
        t.Stop();
        {
          unique_lock<mutex> smartLock(coutLock); // lock the screen
          cout << t.usecs() << ": job "
               << j << " start.\n";
        }
        int sleep = (rand() % 1000000) + 750000;
        this_thread::sleep_for(chrono::microseconds(sleep));
        t.Stop();
        {
          unique_lock<mutex> smartLock(coutLock); // lock the screen
          cout << t.usecs() << ": job "
               << j << " finished: sleep time was " << sleep * 1e-6 << " secs.\n";
        }
      }
    );
  }

  tp.ShutDown();
  t.Stop();
  cout << t.usecs() << ": -PROGRAM FINISHED-\n";
}
#endif
