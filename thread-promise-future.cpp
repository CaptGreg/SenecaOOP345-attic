// http://stackoverflow.com/questions/41186639/why-doesnt-futurewait-block

#include <iostream>
#include <string>
#include <thread>
#include <future>

int main1()
{
  auto prom = std::promise<std::string>();
  auto fut  = prom.get_future();

  std::thread tid([&](){prom.set_value("hello world");});    
    fut.wait();
    std::cout << fut.get() << std::endl;
  tid.join ();

  return 0;
}

// http://www.cplusplus.com/reference/future/future/
// class template
// <future>
// std::future
// 
// template <class T>  future;
// template <class R&> future<R&>;     // specialization : T is a reference type (R&)
// template <>         future<void>;   // specialization : T is void
// 
// Future
// A future is an object that can retrieve a value from some provider object or function, properly synchronizing this access if in different threads.
// 
// "Valid" futures are future objects associated to a shared state, and are constructed by calling one of the following functions:
// 
//     async
//     promise::get_future
//     packaged_task::get_future
// 
// 
// future objects are only useful when they are valid. Default-constructed future objects are not valid (unless move-assigned a valid future).
// 
// Calling future::get on a valid future blocks the thread until the provider makes the shared state ready (either by setting a value or an exception to it). This way, two threads can be synchronized by one waiting for the other to set a value.
// 
// The lifetime of the shared state lasts at least until the last object with which it is associated releases it or is destroyed. Therefore, if associated to a future, the shared state can survive the object from which it was obtained in the first place (if any).
// 
// Member functions
// (constructor)
//     Construct future (public member function )
// (destructor)
//     Destroy future (public member function )
// operator=
//     Move-assign future (public member function )
// share
//     Get shared future (public member function )
// get
//     Get value (public member function )
// valid
//     Check for valid shared state (public member function )
// wait
//     Wait for ready (public member function )
// wait_for
//     Wait for ready during time span (public member function )
// wait_until
//     Wait for ready until time point (public member function )
// 
// Template specializations
// Two specific future specializations are declared in <future>:
// 
// template <class R&> future<R&>;     // specialization : T is a reference type (R&)
// template <>         future<void>;   // specialization : T is void 
// 
// They operate in the same way as the unspecialized template, except for the return value of their future::get member function.
// 
// Example

// GB hacked from cplusplus.com example
// future example
#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <chrono>         // std::chrono::milliseconds

// a non-optimized way of checking for prime numbers:
bool is_prime (int x) {
  for (int i=2; i<x; ++i) if (x%i==0) return false;
  return true;
}

int main2 () // GB <-- hacked from cplusplus.com future example
{
  // call function asynchronously: <-- GB not true unless std::launch::asnc specified
  // std::future<bool> fut = std::async (is_prime,444444443); // GB much more interesting to run as thread

  uint32_t num = 444444443;  // 444 million, fits in int32_t
  // GB much more interesting to run as thread, use std::launch::async
  std::future<bool> fut = std::async (std::launch::async, is_prime,num);

  // do something while waiting for function to set future:
  std::cout << "async checking, please wait";
  std::chrono::milliseconds span (100);
  while (fut.wait_for(span)==std::future_status::timeout)
    std::cout << '.' << std::flush;

  bool x = fut.get();     // retrieve return value
  std::cout << "\n" << num << (x?" is":" is not") << " prime.\n";

  return 0;
}

int main3() // GB blend main1 and main2 together
{
  auto prom = std::promise<bool>();
  auto fut  = prom.get_future();

  uint32_t num = 444444443;  // 444 million, fits in int32_t

  std::thread tid([&](){prom.set_value(is_prime( num ));});
    // fut.wait();  <-- GB replace with main2 polling

    std::cout << "thread fut.wait_for checking, please wait";
    std::chrono::milliseconds span (100);
    while (fut.wait_for(span)==std::future_status::timeout)
      std::cout << '.' << std::flush;

    bool x = fut.get();     // retrieve return value
    std::cout << "\n" << num << (x?" is":" is not") << " prime.\n";
  tid.join ();

  return 0;
}

int main4() // GB blend main1 and main2 together and use sleep+is_joinable
{
  auto prom = std::promise<bool>();
  auto fut  = prom.get_future();

  uint32_t num = 444444443;  // 444 million, fits in int32_t

  std::thread tid([&](){prom.set_value(is_prime( num ));});
    // fut.wait();  <-- GB replace with main2 polling

    std::cout << "thread joinable checking, please wait" << std::flush;
    std::chrono::milliseconds span (100);
    while (true) {
      std::cout << "\ncalling sleep\n";
      std::this_thread::sleep_for((span));
      std::cout << "\ncalling joinable\n";
      if(tid.joinable()) {// means thread is finished <-- doesn't work, call waits for thread to finish, or does it?
        std::cout << "joinable() returned\n";
        break;
      }
      std::cout << "trying to print a . <-- just not happening\n";
      std::cout << '.' << std::flush;
    }

    bool x = fut.get();     // retrieve return value
    std::cout << "\n" << num << (x?" is":" is not") << " prime.\n";
  tid.join ();

  return 0;
}

#include <memory> // shared_ptr
int main5()
{
  // GB http://en.cppreference.com/w/cpp/memory/shared_ptr
  // GB says shared_ptr are thread safe.  They use atomics internally.
  // GB (If it is on the web, it must be true, maybe ...)

  auto prom = std::promise<bool>();
  auto fut  = prom.get_future();

  uint32_t num = 444444443;  // 444 million, fits in int32_t

  std::shared_ptr<uint8_t> threadAlive(new uint8_t);
  std::thread tid(
    [&](){
      std::shared_ptr<uint8_t> threadAliveCopy(threadAlive);
      prom.set_value(is_prime( num ));}
  );
    // fut.wait();  <-- GB replace with main2 polling

    std::chrono::milliseconds span (100);

    std::cout << "Wait for thread to start\n";
    while(threadAlive.use_count() == 1)
      std::this_thread::sleep_for((span));

    std::cout << "Waiting for thread to exit, please wait" << std::flush;
    while(threadAlive.use_count() == 2) {
      std::cout << '.' << std::flush;
      std::this_thread::sleep_for((span));
    }

    bool x = fut.get();     // retrieve return value
    std::cout << "\n" << num << (x?" is":" is not") << " prime.\n";
  tid.join ();

  return 0;
}

int main()
{
  std::cout << "\nmain1\n";
  main1();
  std::cout << "\nmain2\n";
  main2();
  std::cout << "\nmain3\n";
  main3();
  std::cout << "\nmain4\n";
  main4();
  std::cout << "\nmain5\n";
  main5();
  return 0;
}
