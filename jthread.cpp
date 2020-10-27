#include <iostream>
#include <string>
#include <thread>
// #include <jthread> // C++20 not defined in g++-10.2.0 (tested 2020-10-27)
#include <stop_token> // C++20 g++-10 stop_token found in /usr/include/c++/10/ 2020-10-27 
// g++-10 jthread.cpp -o jthread -pthread


#if 0
 Thread cancellation

The stop_XXX types are designed to enable thread cancellation for std::jthread, although they can also be used independently of std::jthread - for example to interrupt std::condition_variable_any waiting functions, or for a custom thread management implementation. In fact they do not even need to be used to "stop" anything, but can instead be used for a thread-safe one-time function(s) invocation trigger, for example.

Defined in header <stop_token>
  * stop_token (C++20) an interface for querying if a std::jthread cancellation request has been made (class)
  * stop_source (C++20) class representing a request to stop one or more std::jthreads (class)
  * stop_callback (C++20) an interface for registering callbacks on std::jthread cancellation (class template)
#endif

using namespace std;

const int NUM_THREADS = 10;

void threadfunction_cout(int arg) 
{
  // cout << "cout: " << arg << "\n";
  cout << string("cout: ") +  to_string(arg) + "\n";
}

int main1() 
{
  int arg;
  // jthread t[NUM_THREADS];     // jthread c++20, not defined in g++-10.2.0 (tested 2020-10-27)
  thread t[NUM_THREADS];     

  cout << "\nthreads using cout:\n\n";
  // launch threads
  arg = 100;
  for (int i = 0; i < NUM_THREADS; ++i)
    t[i] = thread(threadfunction_cout, arg++); // pass arg to thread

  // wait for thread to complete
  for(auto& e: t)
    e.join();

  return EXIT_SUCCESS;
}

 
// https://en.cppreference.com/w/cpp/thread/stop_token
using namespace std::literals::chrono_literals;
 
#if 0
void f(std::stop_token stop_token, int value) // error: variable or field ‘f’ declared void
// error: ‘stop_token’ is not a member of ‘std’
{
    while (!stop_token.stop_requested()) {
        std::cout << value++ << ' ' << std::flush;
        std::this_thread::sleep_for(200ms);
    }
    std::cout << std::endl;
}
#endif
 
int main2()
{
#if 0
    std::jthread thread(f, 5); // prints 5 6 7 8... for approximately 3 seconds
    // error: ‘jthread’ is not a member of ‘std’; did you mean ‘thread’?
    std::this_thread::sleep_for(3s);
    // The destructor of jthread calls request_stop() and join().
#endif
  return EXIT_SUCCESS;
}

int main()
{
  main1();
  main2();
  std::this_thread::sleep_for(3s);
  // The destructor of jthread calls request_stop() and join().
  return EXIT_SUCCESS;
}
