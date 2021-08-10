// g++-10 -fcoroutines -std=c++2a lazyGenerator.cpp -o lazyGenerator -ltbb -pthread && ./lazyGenerator
//  --- works

// lazyGenerator.cpp
/* 
  needs C++20 defined:

    coroutine_handle<P>

    coroutine_traits<Ts...>

    generator<int> 

    co_wait            a c++20 keyword
    co_return          a c++20 keyword
    co_yield           a c++20 keyword

    suspend_always     defined in coroutine
      struct suspend_always {
        bool await_ready()                     const { return false; }
        void await_suspend(coroutine_handle<>) const {}
        void await_resume()                    const {}
      };
    suspend_never      defined in coroutine
      struct suspend_never {
        bool await_ready()                     const { return true; }
        void await_suspend(coroutine_handle<>) const {}
        void await_resume()                    const {}
      };

    egrep -R "suspend_always|suspend_never" /usr 2> /dev/null
      /usr/lib/llvm-9/include/c++/v1/coroutine:struct suspend_never;
      /usr/lib/llvm-9/include/c++/v1/coroutine:struct suspend_always;
      /usr/lib/llvm-9/include/c++/v1/coroutine:struct suspend_never {
      /usr/lib/llvm-9/include/c++/v1/coroutine:struct suspend_always {

*/

// clang++ compile with -fcoroutines-ts -stdlib=libc++ -std=c++2a
// clang++-11 -fcoroutines-ts -stdlib=libc++ -std=c++2a lazyGenerator.cpp -o lazyGenerator -pthread  && ./lazyGenerator
//   error: no template named 'generator' in namespace 'std::did you mean 'std::generate'?

// g++-10 -fcoroutines -std=c++2a lazyGenerator.cpp -o lazyGenerator -pthread  && ./lazyGenerator
// g++-10 works,  g++-9 does not work


/*
  Coroutines

  28 February 2017

  Coroutines are functions that can suspend and resume their execution while keeping their state. 
  The evolution in C++20 goes one step further.

  What I present in this post as a new idea in C++20 is quite old. 
  The term coroutines is coined by Melvin Conway. 
  He used it in his publication to compiler construction in 1963. 
  Donald Knuth called procedures a special case of coroutines. 
  Sometimes, it just takes a bit longer.

  Although I know coroutines from Python, it was quite challenging for me to understand the new 
  concept in C++20. Hence, before I dive into the details, here is the first contact.
  A first contact

  With the new keywords co_await and co_yield C++20 will extend the concept of a function.

  Thanks to  co_await expression it is possible to suspend and resume the execution of the  expression. 
  If you use co_await expression in a function func, 
    the call  auto getResult = func() has not to be blocking, 
    if the result of the function is not available. 
  Instead of a resource-consuming blocking, you have a resource-friendly waiting.

  co_yield expression enables it to write a generator function. 
  The generator function returns on request each time a new value. 
  A generator function is a kind of data stream, from which you can pick values. 
  The data stream can be infinite; therefore, we are in the centre of lazy evaluation with C++.
*/

#if 0
// https://www.modernescpp.com/index.php/coroutines
#include <iostream>
#include <vector>
#include <coroutine>
#include "generator.h"

// using coro_exp::generator;
// generator<double> fibonacci(const double ceiling)  // compiles
coro_exp::generator<int> generatorForNumbers(int begin, int inc= 1) // error: ‘generator’ does not name a type
{
  
  for (int i= begin;; i += inc){
    std::co_yield i;
  }
  
}

int main5()
{
  std::cout << "\n";

  auto numbers= generatorForNumbers(-10);
  
  for (int i= 1; i <= 20; ++i) 
    std::cout << numbers << " ";
  std::cout << "\n\n";

  for (auto n: generatorForNumbers(0, 5)) // error: ‘generatorForNumbers’ was not declared in this scope
    std::cout << n << " ";
  std::cout << "\n\n";

  return EXIT_SUCCESS;
}
#endif

#if 0
// coroutine/docs/ppt/[Eng]ExploringTheCppCoroutine.pdf
#include <coroutine>
// auto my_first_coroutine()  // FACTORY
// error: 'co_await' cannot be used in a function with a deduced return type
void my_first_coroutine() 
// error: this function cannot be a coroutine: 'std::coroutines_v1::coroutine_traits<void>' has no member named 'promise_type'

{
  co_await std::suspend_never{};
}

int main4(int, char* []) 
{
  my_first_coroutine();
  return EXIT_SUCCESS;
}
#endif

#if 0
// coroutine/docs/ppt/[Eng]ExploringTheCppCoroutine.pdf
#include <iostream>
#include <coroutine>
namespace coro = std;

// auto example() -> return_ignore  // error: unknown type name 'return_ignore'
// error: this function cannot be a coroutine: 'std::coroutines_v1::coroutine_traits<int>' has no member named 'promise_type

// auto example() 
// error: 'co_await' cannot be used in a function with a deduced return type

// void example() 
// error: this function cannot be a coroutine: 'std::coroutines_v1::coroutine_traits<void>' has no member named 'promise_type' 

// return_ignore  example() 
// g++-10: error: ‘return_ignore’ does not name a type
// error: unknown type name 'return_ignore'
// error: this function cannot be a coroutine: 'std::coroutines_v1::coroutine_traits<int>' has no member named 'promise_type' 
void  example() 
{
  std::puts("step 1");
  // co_await coro::suspend_always{}; // error: unable to find the promise type for this coroutine

  // co_await suspend_always{}; // error: ‘suspend_always’ was not declared in this scope; did you mean ‘std::__n4861::suspend_always’?

  co_await std::suspend_always{}; // g++-10 error: unable to find the promise type for this coroutine
  std::puts("step 2");
}

int main3(int, char*[]) 
{
  example();
  std::puts("step 3");
  return 0;
}
#endif

#if 0
// coroutine/docs/ppt/[Eng]ExploringTheCppCoroutine.pdf
#include <coroutine> // /usr/lib/llvm-9/include/c++/v1/coroutine
#include <future>
using namespace std;

// Coroutine: suspend and wait for resume
auto async_get_zero() -> future<int> 
{
  co_await suspend_always{};
  // g++-10 error: unable to find the promise type for this coroutine
  co_return 0;
}

int main2(int argc, char*argv[]) {
  auto fz = async_get_zero();
  return fz.get();
}
#endif

#include <coroutine>
#include "generator.h"
#include <iostream>

using coro_exp::generator;

static const double demo_ceiling = 
  1000
  // 10E44
  ;

// #pragma clang diagnostic push
// #pragma clang diagnostic ignored "-Wunused-variable"
// #pragma clang diagnostic ignored "-Wuninitialized"
// #pragma clang diagnostic ignored "-Wfor-loop-analysis"
generator<double> fibonacci(const double ceiling) 
{
  double j = 0;
  double i = 1;
  co_yield j;
  if (ceiling > j) {
    do {
      co_yield i;
      double tmp = i;
      i += j;
      j = tmp;
    } while (i <= ceiling);
  }
}
// #pragma clang diagnostic pop

int main1(int argc, char*argv[]) 
{
  std::cout << "Example program using C++20 coroutine to implement a Fibonacci Sequence generator" << '\n';
  auto iter = fibonacci(demo_ceiling);
  while(iter.next()) {
    const auto value = iter.getValue();
    std::cout << value << '\n';
  }
  return EXIT_SUCCESS;
}

int main(int argc, char*argv[]) 
{
  main1(argc,argv);
  // main2(argc,argv);
  // main3(argc,argv);
}
