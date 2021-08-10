// g++-10 -fcoroutines -std=c++20 -Wall -ggdb foo-bar.cpp -o foo-bar -lcppcoro -pthread

#include <coroutine>
#include <future>

#define HELPERS
#ifdef HELPERS
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

  using HDL = std::coroutine_handle<promise_type>;
  coroutine_type() = default;
  coroutine_type(const coroutine_type&) = delete;
  ~coroutine_type() { _coro.destroy(); }
private:
  coroutine_type(promise_type& p) : _coro(HDL::from_promise(p)) {}
  std::coroutine_handle<promise_type> _coro;
};


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

class task {
public:
  class promise_type; //  { /* see below */ };

  task(task&& t) noexcept : coro_(std::exchange(t.coro_, {})) {}

  ~task() { if (coro_) coro_.destroy(); }

  class awaiter { /* see next line */ };

  awaiter operator co_await() && noexcept;

private:
  explicit task(std::coroutine_handle<promise_type> h) noexcept : coro_(h) {}

  std::coroutine_handle<promise_type> coro_;
};

class task::promise_type {
public:
  task get_return_object() noexcept { return task{std::coroutine_handle<promise_type>::from_promise(*this)}; }

  void return_void () { return; } // GB 
  void initial_suspend () { return; } // GB
};

task foo() {
  co_return; 
}

task bar() {
  co_await foo();
}

int main()
{
  bar();
}
