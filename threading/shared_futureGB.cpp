// http://en.cppreference.com/w/cpp/thread/shared_future

// GB this file compiles into a 700K file: (688639)

// A shared_future may be used to signal multiple threads simultaneously, 
// similar to std::condition_variable::notify_all()


#include <iostream>
#include <future>
#include <chrono>
 
int main()
{   
    std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
    std::shared_future<void> ready_future(ready_promise.get_future());
 
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
 
    auto fun1 = [&]() -> std::chrono::duration<double, std::milli> 
    {
        t1_ready_promise.set_value();
        ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };
 
 
    auto fun2 = [&]() -> std::chrono::duration<double, std::milli> 
    {
        t2_ready_promise.set_value();
        ready_future.wait(); // waits for the signal from main()
        return std::chrono::high_resolution_clock::now() - start;
    };
 
    auto result1 = std::async(std::launch::async, fun1);
    auto result2 = std::async(std::launch::async, fun2);
 
    // wait for the threads to become ready
    t1_ready_promise.get_future().wait();
    t2_ready_promise.get_future().wait();
 
    // the threads are ready, start the clock
    start = std::chrono::high_resolution_clock::now();
 
    // signal the threads to go
    ready_promise.set_value();
 
    std::cout << "Thread 1 received the signal "
              << result1.get().count() << " ms after start\n"
              << "Thread 2 received the signal "
              << result2.get().count() << " ms after start\n";
}

