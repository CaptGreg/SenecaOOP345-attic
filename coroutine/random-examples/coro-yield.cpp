// https://stackoverflow.com/questions/62564094/c20-coroutines-need-a-function-that-uses-co-yield

// g++-10.1 -ocoro-yield -g -O0 -Wall -Werror -Wextra -std=c++2a -fcoroutines -fno-exceptions coro-yield.cpp && ./coro-yield

// #include <coroutine>
#include <iostream>

#if 0
int f() { for (int i = 0; i < 10; ++i) co_yield i; }
// error: unable to find the promise type for this coroutine

int main ()
{
    for (int i : f())
        std::cout << i << ' ';

    return EXIT_SUCCESS;
}
#endif

// coroutine.cpp

// g++ -ocoroutine -g -O0 -Wall -Werror -Wextra
// -std=c++20 -fcoroutines -fno-exceptions coroutine.cpp

#include <stdio.h>
#include <coroutine>

using namespace std;

struct Generator {
  struct Promise;

// compiler looks for promise_type
  using promise_type=Promise;
  coroutine_handle<Promise> coro;
  Generator(coroutine_handle<Promise> h): coro(h) {}

  ~Generator() {
    if(coro)
      coro.destroy();
  }
// get current value of coroutine
  int value() {
    return coro.promise().val;
  }

// advance coroutine past suspension
  bool next() {
    coro.resume();
    return !coro.done();
  }
  struct Promise {
// current value of suspended coroutine
    int val;

// called by compiler first thing to get coroutine result
    Generator get_return_object() {
      return Generator{coroutine_handle<Promise>::from_promise(*this)};
    }
// called by compiler first time co_yield occurs
    suspend_always initial_suspend() {
      return {};
    }

// required for co_yield
    suspend_always yield_value(int x) {
      val=x;
      return {};
    }
// called by compiler for coroutine without return
    suspend_never return_void() {
      return {};
    }

// called by compiler last thing to await final result
// coroutine cannot be resumed after this is called
    suspend_always final_suspend() {
      return {};
    }
  };

};

///////////////////////////////////////////////////////////////////

Generator myCoroutineFunction(int n) { for(int i = 0; i < n; ++i) co_yield i; }

Generator f() { for (int i = 0; i < 21; ++i) co_yield i; }

// Couroutine_Example_Generator<int> f2() { for (int i = 0; i < 10; ++i) co_yield i; }
Generator f2() { for (int i = 1; i <= 15; ++i) co_yield i; }

int main ()
{
  const int n=10;

  Generator myCoroutineResult = myCoroutineFunction(n);
  for(int i=0; i < n; ++i) {
    myCoroutineResult.next();
    //  bool next() { coro.resume(); return !coro.done(); }

    std::cout << myCoroutineResult.value() << " ";
    // int value() { return coro.promise().val; }
  }
  std::cout << "\n";

  Generator fResult = f();
  fResult.next();
  do
    std::cout << fResult.value() << ' ';
  while( fResult.next() );
  std::cout << "\n";

  for (auto i = f2(); i.next(); )
    std::cout << i.value() << ' ';
  std::cout << "\n";

  return EXIT_SUCCESS;
}

