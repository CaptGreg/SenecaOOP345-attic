// https://www.modernescpp.com/index.php/c-20-thread-pools-with-cppcoro

// g++-10 -fcoroutines -std=c++2a cppcoroWhenAllOnThreadPool.cpp -o cppcoroWhenAllOnThreadPool -ltbb -lcppcoro -pthread && ./cppcoroWhenAllOnThreadPool 
// doesn't compile


// cppcoroWhenAllOnThreadPool.cpp

#include <chrono>
#include <iostream>
#include <thread>

#include <memory.h>

#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/static_thread_pool.hpp>
#include <cppcoro/when_all.hpp>


using namespace std::chrono_literals;

auto start = std::chrono::high_resolution_clock::now();

void at (std::string s) 
{
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = now - start;    // (4)
  std::cout << std::to_string(elapsed.count()) + " : " + s + "\n";
}

uint64_t gettid()
{
    auto tid = std::this_thread::get_id();
    uint64_t utid;
    memcpy(&utid, &tid, 8);
    return utid;
}

cppcoro::task<std::string> getFirst() 
{
    uint64_t utid = gettid();
    std::string stid = std::to_string(gettid());
    at( std::string("1st thread ") + stid + " sleeping for 1s");
    std::this_thread::sleep_for(1s);
    at(std::string("1st thread ") + stid + " woke up");
    co_return "First";
}

cppcoro::task<std::string> getSecond() 
{
    uint64_t utid = gettid();
    std::string stid = std::to_string(gettid());
    at( std::string("2nd thread ") + stid + " sleeping for 1s");
    std::this_thread::sleep_for(1s);
    at( std::string("2nd thread ") + stid + " woke up");

    co_return "Second";
}

cppcoro::task<std::string> getThird() 
{
    uint64_t utid = gettid();
    std::string stid = std::to_string(gettid());
    at( std::string("3rd thread ") + stid + " sleeping for 1s");
    std::this_thread::sleep_for(1s);
    at( std::string("3rd thread ") + stid + " woke up");

    co_return "Third";
}

cppcoro::task<std::string> getFourth() 
{
    uint64_t utid = gettid();
    std::string stid = std::to_string(gettid());
    at( std::string("4th thread ") + stid + " sleeping for 1s");
    std::this_thread::sleep_for(1s);
    at( std::string("4th thread ") + stid + " woke up");

    co_return "Fourth";
}

cppcoro::task<std::string> getFifth() 
{
    uint64_t utid = gettid();
    std::string stid = std::to_string(gettid());
    at( std::string("5th thread ") + stid + " sleeping for 1s");
    std::this_thread::sleep_for(1s);
    at( std::string("5th thread ") + stid + " woke up");

    co_return "Fifth";
}



template <typename Func>
cppcoro::task<std::string> runOnThreadPool(cppcoro::static_thread_pool& tp, Func func) 
{
    co_await tp.schedule();
    auto res = co_await func();
    co_return res;
}

cppcoro::task<> runAll(cppcoro::static_thread_pool& tp) 
{
    auto[one, two, three, four, five] = 
    co_await cppcoro::when_all(    // (3)
        runOnThreadPool(tp, getFirst),
        runOnThreadPool(tp, getSecond), 
        runOnThreadPool(tp, getThird),
        runOnThreadPool(tp, getFourth),
        runOnThreadPool(tp, getFifth)
    );
    
    std::cout << one << " " << two << " " << three << " " << four << " " << five << std::endl;
    
}
    
int main() {
    std::cout << "main tid=" << std::to_string(gettid()) << std::endl;
    
    // auto start = std::chrono::steady_clock::now();

    cppcoro::static_thread_pool tp;                         // (1)
    cppcoro::sync_wait(runAll(tp));                         // (2)
    
    std::cout << std::endl;
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;    // (4)
    std::cout << "Execution time " << elapsed.count() << " seconds." << std::endl;
    
    std::cout << std::endl;

    std::cout << "try again\n";
    start = std::chrono::high_resolution_clock::now();
    cppcoro::sync_wait(runAll(tp));                         // (2)
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;    // (4)
    std::cout << "Execution time " << elapsed.count() << " seconds." << std::endl;

    return EXIT_SUCCESS;
}
