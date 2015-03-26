
// adapted from http://www.cplusplus.com/reference/future/promise/

// promise example
#include <iostream>       // std::cout
#include <chrono>         // std::chrono
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <mutex>          // std::promise, std::future

using namespace std;

mutex coutLock;

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Reset() { start = std::chrono::high_resolution_clock::now(); }
  void Print(string msg) {
    stop  = std::chrono::high_resolution_clock::now();
    typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    long tWait = duration_get.count();
    coutLock.lock();
      std::cout << msg << tWait << " msecs\n";
    coutLock.unlock();
  }
};

void threadFunction (std::future<int>& in) 
{
  { // acquire lock
    std::unique_lock<std::mutex> smartLock (coutLock);
    cout << "threadFunction: alive, waiting for future to be set\n";
  } // release lock

  Timer timer;

  timer.Reset();
  int futureValue = in.get();
  timer.Print("threadFunction: waited ");

  { // acquire lock
    std::unique_lock<std::mutex> smartLock (coutLock);
    cout << "threadFunction: future value was " << futureValue << '\n';
  } // release lock
}

int main (int argc,char**argv) // promise + future
{
  std::promise<int> promise;
  std::future<int> future = promise.get_future();

  thread t ( threadFunction, ref(future) );

  long nap = 1500;
  { // acquire lock
    std::unique_lock<std::mutex> smartLock (coutLock);
      cout << "main: will wait " << nap << " msec before setting future value\n";
  } // release lock
  this_thread::sleep_for (std::chrono::milliseconds(nap)); 

  promise.set_value (42); 
  { // acquire lock
    std::unique_lock<std::mutex> smartLock (coutLock);
      cout << "main: set 'future' to 42\n";
  } // release lock
  
  t.join();

  return 0;
}
