// g++-10 -fcoroutines -std=c++2a c1.cpp -o c1 -ltbb -pthread && ./c1

#include <coroutine>
#include <future>
#include <iostream>
using namespace std;

#if 1
struct co_awaitable_type
{
  bool await_ready();
  void await_suspend(std::coroutine_handle<>);
  auto await_resume();
};

struct suspend_never
{
  bool await_ready() { return true; };
  void await_suspend(std::coroutine_handle<>) {};
  auto await_resume() {};
};

struct suspend_always
{
  bool await_ready() { return false; };
  void await_suspend(std::coroutine_handle<>) {};
  auto await_resume() {};
};
#endif

#if 1
std::future<int> a()
{
  co_return 42; // g++-10 error: unable to find the promise type for this coroutine
}

std::future<int> doSomething()
{
  auto answer = co_await a();
  // do something with the answer
  return answer + 1;
}
#endif

#if 1
struct coroutine_type
{
  struct promise_type
  {
    int _value;
    coroutine_type get_return_object() { return {*this}; }
    auto initial_suspend() { return std::suspend_never{}; }  //  error: reference to ‘suspend_never’ is ambiguous
    auto final_suspend() { return std::suspend_never{}; }
    void return_value(int val) { _value = val; }
    void unhandled_exception() { std::terminate(); }
  };

  using HDL = coroutine_handle<promise_type>;
  coroutine_type() = default;
  coroutine_type(const coroutine_type&) = delete;
  ~coroutine_type() { _coro.destroy(); }
private:
  coroutine_type(promise_type& p) : _coro(HDL::from_promise(p)) {}
  coroutine_handle<promise_type> _coro;
};
#endif

// coroutine_handle coroutine()
coroutine_type coroutine()  // factory coroutine_type not defined
{
  std::cout << "before suspension" << "\n";
  co_await std::suspend_always{};
  std::cout << "resumed" << "\n";
  co_return 42;
}

int main()
{
  auto coro = coroutine();
  coro.resume();  // error: ‘struct coroutine_type’ has no member named ‘resume’
  std::cout << coro.get() << "\n";
}

/*
Output:
  before suspension
  coro.resume(); >resumed
  42
*/
