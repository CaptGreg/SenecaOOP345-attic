// g++-10 -fcoroutines -std=c++2a   co_funs.cpp -o co_funs -lcppcoro -pthread && ./co_funs

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// https://mariusbancila.ro/blog/2020/06/22/a-cpp20-coroutine-example/

#include <chrono>
#include <iostream>
#include <string>
#include <cppcoro/generator.hpp>
void print_time()
{
   auto now = std::chrono::system_clock::now();
   std::time_t time = std::chrono::system_clock::to_time_t(now);   
 
   char mbstr[100];
   if (std::strftime(mbstr, sizeof(mbstr), "[%H:%M:%S] ", std::localtime(&time))) 
   {
      std::cout << mbstr;
   }
}
cppcoro::generator<std::string> produce_items()
{
  while (true)
  {
     auto v = rand();
     using namespace std::string_literals;
     auto i = "item "s + std::to_string(v);
     print_time();
     std::cout << "produced " << i << '\n';
     co_yield i;
  }
}

#include <cppcoro/task.hpp>
cppcoro::task<> consume_items(int const n)
{
  int i = 1;
  for(auto const& s : produce_items())
  {
     print_time();
     std::cout << "consumed " << s << '\n';
     if (++i > n) break;
  }
 
  co_return;
}

// The cppcoro::generator<T> produces values in a lazy but synchronously way. That means, using the co_await operator from a coroutine returning this type is not possible. However, the cppcoro library features an asynchronous generator, called cppcoro::async_generator<T>, that makes this possible.

// We can change the preceding example as follows: a new coroutine, next_value() returns a value that takes some time to be computed. We simulate that by awaiting for a random number of seconds. The produce_items() coroutine waits for a new value in each loop and then returns a new item from that value. The return type, this time, is cppcoro::async_generator<T>.


#include <cppcoro/async_generator.hpp>
#if 0
cppcoro::task<int> next_value()
{
  using namespace std::chrono_literals;
  // co_await std::chrono::seconds(1 + rand() % 5); // error: no member named ‘await_ready’ in ‘std::chrono::seconds’ {aka ‘std::chrono::duration<long int>’}
  auto wait = std::chrono::seconds(1 + rand() % 5); 
  co_await wait; // error: no member named ‘await_ready’ in ‘std::chrono::duration<long int>’
  co_return rand();
}
cppcoro::async_generator<std::string> produce_items_ag()
{
  while (true)
  {
     auto v = co_await next_value();
     using namespace std::string_literals;
     auto i = std::string("item ") + std::to_string(v);
     print_time();
     std::cout << "produced " << i << '\n';
     co_yield i;
  }
}
// The consumer requires a slight change, because it has to await for each new value. This is done with the use of the co_await operator in the for loop as follows:
cppcoro::task<> consume_items(int const n)
{
  int i = 1;
  for co_await(auto const& s : produce_items_ag())
  {
     print_time();
     std::cout << "consumed " << s << '\n';
     if (++i > n) break;
  }
}
// Another important thing to note here, is that invoking co_await with a time duration is not possible out of the box. However, it is made possible by overloading the co_await operator. An implementation that works on Windows is the following:
#include <windows.h>
auto operator co_await(std::chrono::system_clock::duration duration)
{
   class awaiter
   {
      static
         void CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE,
            void* Context,
            PTP_TIMER)
      {
         stdco::coroutine_handle<>::from_address(Context).resume();
      }
      PTP_TIMER timer = nullptr;
      std::chrono::system_clock::duration duration;
   public:
 
      explicit awaiter(std::chrono::system_clock::duration d) 
         : duration(d)
      {}
 
      ~awaiter()
      {
         if (timer) CloseThreadpoolTimer(timer);
      }
 
      bool await_ready() const
      {
         return duration.count() <= 0;
      }
 
      bool await_suspend(stdco::coroutine_handle<> resume_cb)
      {
         int64_t relative_count = -duration.count();
         timer = CreateThreadpoolTimer(TimerCallback,
            resume_cb.address(),
            nullptr);
         bool success = timer != nullptr;
         SetThreadpoolTimer(timer, (PFILETIME)&relative_count, 0, 0);
         return success;
      }
 
      void await_resume() {}
 
   };
   return awaiter{ duration };
}
#endif

#include <cppcoro/async_manual_reset_event.hpp>
#if 0
struct cppcoro::async_manual_reset_event::awaiter
{
  awaiter(const async_manual_reset_event& event) noexcept
  : m_event(event)
  {}

  bool await_ready() const noexcept;
  bool await_suspend(std::experimental::coroutine_handle<> awaitingCoroutine) noexcept;
  void await_resume() noexcept {}

private:

  const async_manual_reset_event& m_event;
  std::experimental::coroutine_handle<> m_awaitingCoroutine;
  awaiter* m_next;
};
#endif

cppcoro::async_manual_reset_event event;
#if 0
bool cppcoro::async_manual_reset_event::awaiter::await_ready() const noexcept
{
  return m_event.is_set();
}
#endif

// An example: Implementing a simple thread-synchronisation primitive
// Now that we’ve covered a lot of the mechanics of the co_await operator, I want to show how to put some of this knowledge into practice by implementing a basic awaitable synchronisation primitive: 
// An asynchronous manual-reset event.
// The basic requirements of this event is that it needs to be Awaitable by multiple concurrently executing coroutines and 
// when awaited needs to suspend the awaiting coroutine until some thread calls the .set() method, at which point any awaiting coroutines are resumed. 
// If some thread has already called .set() then the coroutine should continue without suspending.
// Ideally we’d also like to make it noexcept, require no heap allocations and have a lock-free implementation.
// Example usage should look something like this:
int value;
// A single call to produce a value
void producer()
{
  value = rand();

  // Publish the value by setting the event.
  event.set();
}
// Supports multiple concurrent consumers
cppcoro::task<> consumer()
{
  // Wait until the event is signalled by call to event.set()
  // in the producer() function.
  co_await event;

  // Now it's safe to consume 'value'
  // This is guaranteed to 'happen after' assignment to 'value'
  std::cout << value << std::endl;
}
#if 0
cppcoro::async_manual_reset_event::awaiter
cppcoro::async_manual_reset_event::operator co_await() const noexcept
{
  return awaiter{ *this };
}
#endif


#include <cppcoro/sync_wait.hpp>
int main()
{
   cppcoro::sync_wait(consume_items(5));
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// https://kirit.com/How%20C%2B%2B%20coroutines%20work/Awaiting

// We've covered co_return and co_yield now. The final new keyword introduced by the coroutines TS is co_await. 
// Where the first two keywords allow a coroutine to suspend whilst its consumer gets ready to use results, co_await is used to allow the coroutine to suspend whilst it waits for things it needs. 
// I tend to think of co_return and co_yield as servicing the outside of the coroutine and co_await as dealing with the inside.

#if 0
lazy<int> answer() {
    std::cout << "Thinking deep thoghts..." << std::endl;
    co_return 42;
}

sync<int> await_answer() {
    std::cout << "Started await_answer" << std::endl;g++-10 -fcoroutines -std=c++2a   co_funs.cpp -o co_funs -pthread && ./co_funs

    auto a = answer();
    std::cout << "Got a coroutine, let's get a value" << std::endl;
    auto v = co_await a;
    std::cout << "And the coroutine value is: " << v << std::endl;
    v = co_await a;
    std::cout << "And the coroutine value is still: " << v << std::endl;
    co_return 0;
}

int main() {
    return await_answer().get();
}
#endif

#if 0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
https://lewissbaker.github.io/2017/09/25/coroutine-theory



#include <coroutine>
#include <future>
#include <iostream>

using namespace std;

template<typename RET, typename... ARGS>
struct coroutine_traits<RET, ARGS...>
// error: partial specialization ‘struct std::__n4861::coroutine_traits<RET, ARGS ...>’ does not specialize any template arguments and is not more constrained than the primary template; 
//        to define the primary template, remove the template argument list
{
  using promise_type = typename RET::promise_type;
};


template<typename ALLOCATOR>
struct my_promise_type
{
  template<typename... ARGS>
  void* operator new(std::size_t sz, std::allocator_arg_t, ALLOCATOR& allocator, ARGS&... args)
  {
    // Round up sz to next multiple of ALLOCATOR alignment
    std::size_t allocatorOffset =
      (sz + alignof(ALLOCATOR) - 1u) & ~(alignof(ALLOCATOR) - 1u);

    // Call onto allocator to allocate space for coroutine frame.
    void* ptr = allocator.allocate(allocatorOffset + sizeof(ALLOCATOR));

    // Take a copy of the allocator (assuming noexcept copy constructor here)
    new (((char*)ptr) + allocatorOffset) ALLOCATOR(allocator);

    return ptr;
  }

  void operator delete(void* ptr, std::size_t sz)
  {
    std::size_t allocatorOffset =
      (sz + alignof(ALLOCATOR) - 1u) & ~(alignof(ALLOCATOR) - 1u);

    ALLOCATOR& allocator = *reinterpret_cast<ALLOCATOR*>(
      ((char*)ptr) + allocatorOffset);

    // Move allocator to local variable first so it isn't freeing its
    // own memory from underneath itself.
    // Assuming allocator move-constructor is noexcept here.
    ALLOCATOR allocatorCopy = std::move(allocator);

    // But don't forget to destruct allocator object in coroutine frame
    allocator.~ALLOCATOR();

    // Finally, free the memory using the allocator.
    allocatorCopy.deallocate(ptr, allocatorOffset + sizeof(ALLOCATOR));
  }
};
#endif

#if 0
struct my_promise_type
{
  void* operator new(std::size_t size)
  {
    void* ptr = my_custom_allocate(size);
    if (!ptr) throw std::bad_alloc{};
    return ptr;
  }

  void operator delete(void* ptr, std::size_t size)
  {
    my_custom_free(ptr, size);
  }

  // ...
};
#endif

#if 0
int funs()
{
  co_await promise.initial_suspend(); // error: missing template arguments before ‘.’ token
  try
  {
    std::cout << "body ...\n";
  }
  catch (...)
  {
    promise.unhandled_exception();  // error: expected unqualified-id before ‘.’ token
  }
FinalSuspend:
  co_await promise.final_suspend(); // error: missing template arguments before ‘.’ token
}
#endif

#if 0
template<typename ALLOCATOR, typename... ARGS>
struct coroutine_traits<my_return_type, std::allocator_arg_t, ALLOCATOR, ARGS...>
{
  using promise_type = my_promise_type<ALLOCATOR>;
};
#endif


#if 0
int main()
{
  // funs();
}
#endif
