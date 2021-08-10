// co_routines are a C++20 feature
// How to test with g++-9.2 -std=c++2a?
//  --- not yet

// clang++ compile with -fcoroutines-ts –stdlib=libc++ -std=c++2a
// clang++ -fcoroutines-ts -stdlib=libc++ -std=c++2a co_routine.cpp -o co_routine -lrt -pthread -ltbb -ltbb
// clang++ -fcoroutines-ts -std=c++2a co_routine.cpp -o co_routine -lrt -pthread -ltbb -ltbb
// g++-10 -fcoroutines -std=c++2a co_routine.cpp -o co_routine -ltbb -pthread
// g++-10     co_routine.cpp:19:1: error: ‘async_manual_reset_event’ does not name a type
// g++-10     co_routine.cpp: In function ‘void producer()’:
// g++-10     co_routine.cpp:39:3: error: ‘event’ was not declared in this scope
// g++-10     co_routine.cpp: At global scope:
// g++-10     co_routine.cpp:43:1: error: ‘task’ does not name a type



#include <iostream>
#include <future>      // async
#include <coroutine>

#include <thread>  // coroutines added to thread?


int value;
async_manual_reset_event event; // error: unknown type name 'async_manual_reset_event'
// async_manual_reset_event::set() resumes each of the suspended coroutines sequentially before the call to set() returns. Each resumed coroutine will only execute until it hits the next suspend-point.
// grep -R "async_manual_reset_event" /usr 2> /dev/null
//  --- not found
// grep -R "event" /usr/lib/llvm-9/include/ 2> /dev/null| grep async
//  --- not found
// grep -R "reset" /usr/lib/llvm-9/include/ 2> /dev/null | grep event
//  --- not found
// nothing found in <future>
// future<T> fut = async(std::launch::deferred, function, parms, ...)


int some_long_running_computation() {return 42;}

// A single call to produce a value
void producer()
{
  value = some_long_running_computation();

  // Publish the value by setting the event.
  event.set();
}

// Supports multiple concurrent consumers
task<> consumer()                                // error: no template named 'task'
{
  // Wait until the event is signalled by call to event.set()
  // in the producer() function.
  co_await event;

  // Now it's safe to consume 'value'
  // This is guaranteed to 'happen after' assignment to 'value'
  std::cout << value << std::endl;
}

int main(int argc,char*arv[])
{
}
