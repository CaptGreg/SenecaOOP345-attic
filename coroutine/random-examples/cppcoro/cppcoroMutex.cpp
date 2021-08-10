// https://www.modernescpp.com/index.php/c-20-coroutine-abstraction-with-cppcoro

// cppcoroMutex.cpp

// sync using coroutine async_mutex

#include <cppcoro/async_mutex.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>

#include <iostream>
#include <thread>
#include <vector>


cppcoro::async_mutex mutex;

uint64_t sum{};                                                                  // (2)

cppcoro::task<> addToSum(int num) 
{
    cppcoro::async_mutex_lock lockSum = co_await mutex.scoped_lock_async(); // (3)  
    sum += num;
  
}                                                                           // (4)

int main() 
{
    
    std::cout << std::endl;
    
    std::vector<std::thread> vec(100);                                       // (1)
    
    auto start = std::chrono::high_resolution_clock::now();

    for(auto& thr: vec) {
        thr = std::thread([]{ for(int n = 0; n < 100000; ++n) cppcoro::sync_wait(addToSum(n)); } );
    }
    
    for(auto& thr: vec) thr.join();

    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = now - start;    // (4)
    std::cout << std::to_string(elapsed.count()) + " s. 100 threads, 100000 counts per thread, 10 million mutex access\n";

    
    std::cout << "sum: " << sum << std::endl;
    
    std::cout << std::endl;

    return EXIT_SUCCESS;
}


