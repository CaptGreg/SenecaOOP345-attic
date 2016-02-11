// http://en.cppreference.com/w/cpp/thread/future

// GB doesn;t compile:
// error: ‘class std::promise<int>’ has no member named ‘make_ready_at_thread_exit’

#include <iostream>
#include <future>
#include <thread>
 
int main(int argc, char**argv)
{
    // future from a packaged_task
    std::packaged_task<int()> task([](){ return 7; }); // wrap the function
    std::future<int> f1 = task.get_future();  // get a future
    std::thread(std::move(task)).detach(); // launch on a thread
 
    // future from an async()
    std::future<int> f2 = std::async(std::launch::async, [](){ return 8; });
 
    // future from a promise
    std::promise<int> promise;
    std::future<int> f3 = promise.get_future();
    std::thread( 

         // function:
         // [](std::promise<int> promise){ promise.set_value_at_thread_exit(9); }, // error no member
         [](std::promise<int> promise){ promise.make_ready_at_thread_exit(9); }, // error no member
         // error: ‘class std::promise<int>’ has no member named ‘make_ready_at_thread_exit’

         // arg:
         std::move(promise) 
    ).detach();
 
    std::cout << "Waiting..." << std::flush;
    f1.wait();
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: "
              << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
}
