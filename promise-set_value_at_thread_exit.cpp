// http://en.cppreference.com/w/cpp/thread/promise/set_value_at_thread_exit
#include <iostream>
#include <future>
#include <thread>
#include <chrono> // GB
 
int main()
{
    // using namespace std::chrono_literals;                    // unknown namespace
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread(
      [&prom] {
          std::cout << "  thread alive... " << std::flush;
          // prom.set_value(99);
          // std::this_thread::sleep_for(1s);                   // GB 1s not defined, -std=c++17 doesn't help
          std::this_thread::sleep_for(std::chrono::seconds(2)); // GB this works
          prom.set_value_at_thread_exit(9);
          std::cout << "thread exit value set..." << std::flush;
          std::this_thread::sleep_for(std::chrono::seconds(2));
          std::cout << "   thread exiting\n" << std::flush;
      }
    ).detach();
 
    // can only retrieve fut.get() once
    // retrieves first value set by prom.set_value(...)
    std::cout << "Waiting..." << std::flush;
    fut.wait();
    std::cout << "Done!\nResult is: " << fut.get() << '\n';
}
