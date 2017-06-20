// http://en.cppreference.com/w/cpp/thread/promise/set_value_at_thread_exit
#include <iostream>
#include <future>
#include <thread>
#include <chrono> // GB
 
int main()
{
    // using namespace std::chrono_literals;               // unknown namespace
    using namespace std;                                   // GB
    std::promise<int> p;
    std::future<int> f = p.get_future();
    std::thread([&p] {
          // std::this_thread::sleep_for(1s);              // GB 1s not defined
          std::this_thread::sleep_for(chrono::seconds(1)); // GB this works
          p.set_value_at_thread_exit(9);
    }).detach();
 
    std::cout << "Waiting..." << std::flush;
    f.wait();
    std::cout << "Done!\nResult is: " << f.get() << '\n';
}
