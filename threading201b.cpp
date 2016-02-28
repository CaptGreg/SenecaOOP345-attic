// adapted from http://www.cplusplus.com/reference/future/promise/

// promise example
#include <iostream>       // std::cout
#include <chrono>         // std::chrono
#include <sstream>        // std::stringstream
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <mutex>          // std::promise, std::future

using namespace std;

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

    stringstream ss;
    ss << msg << tWait << " msecs\n";
    std::cout << ss.str();
  }
};

// void threadFunction (future<int>& inFuture) 
void threadFunction (std::future<int>& inFuture, std::promise<int>& outPromise) 
{
  stringstream ss;

  cout  << "threadFunction: alive, waiting for future to be set\n";

  Timer timer;

  timer.Reset();
  int futureValue = inFuture.get();
  timer.Print("threadFunction: waited ");

  ss << "threadFunction: main passed in future value of " << futureValue << '\n';
  cout << ss.str(); ss.str("");

  long nap = 750;
  ss << "threadFunction: will wait " << nap << " msec before setting return promise value\n";
  cout << ss.str(); ss.str("");
  this_thread::sleep_for (std::chrono::milliseconds(nap)); 

  outPromise.set_value(futureValue * 2);

  ss << "threadFunction: set promise to " << futureValue * 2 << "\n";
  cout << ss.str(); ss.str("");
}

int main (int argc,char**argv) // promise + future
{
  stringstream ss;

  Timer timer;
  std::promise<int> inPromise;
  std::future<int> future = inPromise.get_future();

  std::promise<int> promise;
  thread t ( threadFunction, ref(future), ref(promise) );
  // thread t ( threadFunction, future, promise );  // syntax errors if we omit 'ref'

  long nap = 1500;
  ss << "main: will wait " << nap << " msec before setting future value\n";
  cout << ss.str(); ss.str("");
  this_thread::sleep_for (std::chrono::milliseconds(nap)); 

  // Set the promise value to something
  inPromise.set_value (42); 
  cout << "main: set 'future' to 42\n";
  
  timer.Reset();
  int promise_GetFuture_Get = promise.get_future().get();
  timer.Print( "main: read promise from thread wait time " );

  ss << "main: thread return value retrieved via promise.get_future().get() = " << promise_GetFuture_Get << "\n";
  cout << ss.str(); ss.str("");

  t.join();

  return 0;
}
