#include <iostream>
#include "tpx.h"

ThreadPool::ThreadPool(int numberOfThreads)
{
  for(int t=0; t < numberOfThreads; t++)
    threads.emplace_back( &ThreadPool::Run, this );
}

ThreadPool::~ThreadPool()
{
  if(!stopped) ShutDown();
}

void ThreadPool::ShutDown()
{
  { 
    std::unique_lock<std::mutex> smartLock(mtx);
    quit = true;
   cv.notify_all();
  }  
  for(auto& e : threads) e.join();
  stopped = true;
}


void ThreadPool::AddJob( std::function<void(void)> job)
{
  if(stopped) throw "thread pool shutdown - not accepting jobs";
  if(quit)    throw "thread pool shutting down - not accepting jobs";
  { 
    std::unique_lock<std::mutex> smartLock(mtx);
    jobQueue.push( job );
    cv.notify_one();
  }
}

void ThreadPool::Run() // note this is a private member function
{
  std::function<void(void)> job;
  while(true) {
    { 
      std::unique_lock<std::mutex> smartLock(mtx);

      auto f = [this] () -> bool { return !jobQueue.empty() || quit; };
      cv.wait( smartLock, f);

      if(quit && jobQueue.empty()) return;

      job = jobQueue.front();
      jobQueue.pop();
    }

    job();
  }  
}

#define TEST
#ifdef TEST
#include <iostream>
#include <sstream>
#include <chrono>
#include <unistd.h>
using namespace std;

int main()
{
  ThreadPool tp;

  for(int j  = 0; j < 25 ; j++ ) {
    tp.AddJob( [j] () {
      {
        stringstream ss;
        ss << "job " << j << " start\n";
        cout << ss.str();
      }
      int nap;
      nap = (rand() % 1000) + 1000; // millisec sleep for a random number of seconds between 1 and 2
      this_thread::sleep_for(chrono::milliseconds(nap));
      {
        stringstream ss;
        ss << "job " << j << " woke up after "  << nap << " millisecs\n";
        cout << ss.str();
      }

    } );
  }
  tp.ShutDown();
}

#endif
