// https://www.linkedin.com/pulse/c20-powerful-coroutines-cppcoro-rainer-grimm/

// cppcoroMutex.cpp

#include <cppcoro/async_mutex.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>


const int CALLS_PER_THREAD = 10000;
const int THREADS = 100;

cppcoro::async_mutex mutex;

int sum{};                                                                  // (2)

cppcoro::task<> addToSum(int num) {
    cppcoro::async_mutex_lock lockSum = co_await mutex.scoped_lock_async(); // (3)  
    sum += num;
}                                                                           // (4)

std::mutex m;
void addToSumLockGuard(int num) {
    std::lock_guard<std::mutex> smartLock(m);
    sum += num;
}  

int main(int argc,char*argv[]) {
  std::cout <<"+++++++++++++++++++++++++++++++\n";
  std::cout << argv[0] << "\n";
  std::cout << "FILE '" __FILE__ "' compiled " __DATE__ " at " __TIME__  " on " __DATE__ 
    #ifdef __GNUC__  // either g++ or clang++
      #ifdef __clang__
        " by clang++ "
      #else
        " by g++ "
      #endif
       __VERSION__
    #endif
       "\n\n";

    std::cout << std::endl;
    
    std::vector<std::thread> vec(THREADS);                                  // (1)
    
    sum = 0;
    auto start = std::chrono::high_resolution_clock::now();

      for(auto& thr: vec) {
          thr = std::thread([]{ for(int n = 0; n < CALLS_PER_THREAD; ++n) cppcoro::sync_wait(addToSum(n)); } );
      }
      
      for(auto& thr: vec) thr.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "cppcoro async_mutex " << elapsed.count() << " seconds." << std::endl;
    
    std::cout << "sum: " << sum << std::endl;
    
    std::cout << std::endl;

    ///////////////////////////////////////////////////////////

    sum = 0;
    start = std::chrono::high_resolution_clock::now();

      for(auto& thr: vec) {
          thr = std::thread([]{ for(int n = 0; n < CALLS_PER_THREAD; ++n) addToSumLockGuard(n); } );
      }
      
      for(auto& thr: vec) thr.join();

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "lock_guard " << elapsed.count() << " seconds." << std::endl;
    
    std::cout << "sum: " << sum << std::endl;
    
    std::cout << std::endl;
}
