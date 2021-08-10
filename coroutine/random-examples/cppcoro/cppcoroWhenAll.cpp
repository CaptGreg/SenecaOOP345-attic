// https://www.modernescpp.com/index.php/c-20-thread-pools-with-cppcoro

// g++-10 -fcoroutines -std=c++2a cppcoroWhenAll.cpp -o cppcoroWhenAll -ltbb -lcppcoro -pthread && ./cppcoroWhenAll 
// doesn't compile


// cppcoroWhenAll.cpp

#include <chrono>
#include <iostream>
#include <thread>

#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/when_all.hpp>

using namespace std::chrono_literals;

cppcoro::task<std::string> getFirst() {
    std::this_thread::sleep_for(1s);                       // (3)
    co_return "First";
}

cppcoro::task<std::string> getSecond() {
     std::this_thread::sleep_for(1s);                      // (3)
    co_return "Second";
}

cppcoro::task<std::string> getThird() {
     std::this_thread::sleep_for(1s);                     // (3)
    co_return "Third";
}


cppcoro::task<> runAll() {
                                                          // (2)
    auto[fir, sec, thi] = co_await cppcoro::when_all(getFirst(), getSecond(), getThird());
    
    std::cout << fir << " " << sec << " " << thi << std::endl;
    
}

int main() {
    
    std::cout << std::endl;
    
    // auto start = std::chrono::steady_clock::now();
    auto start = std::chrono::high_resolution_clock::now();   // GB use the same clock
    
    cppcoro::sync_wait(runAll());                          // (1)
    
    std::cout << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;   // (4)
    std::cout << "Execution time " << elapsed.count() << " seconds." << std::endl;
    
    std::cout << std::endl;

}
