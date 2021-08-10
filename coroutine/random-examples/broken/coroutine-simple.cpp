// https://gist.github.com/yizhang82/28842f7dbae34b59fcd7b4d74b4a19d4

// /usr/bin/clang++-10  -fcoroutines-ts -std=c++2a   coroutine-simple.cpp -o coroutine-simple -pthread
// /usr/local/gcc-10.2/bin/g++-10.2  -fcoroutines -std=c++2a   coroutine-simple.cpp -o coroutine-simple -pthread

// NOTE -fcoroutines-ts for clang++ but -fcoroutine for g++

// https://en.cppreference.com/w/cpp/keyword 
// https://en.cppreference.com/w/cpp/compiler_support

// g++-10 -fcoroutines -std=c++2a coroutine-simple.cpp -o coroutine-simple -ltbb -pthread
// g++-10       coroutine-simple.cpp:35:9: error: unable to find the promise type for this coroutine
// g++-10          35 |         co_return 1;
// g++-10       coroutine-simple.cpp: In function ‘std::future<void> test_async_fib()’:
// g++-10       coroutine-simple.cpp:58:19: error: unable to find the promise type for this coroutine
// g++-10          58 |         int ret = co_await async_fib(i);


// TODO: convert int to uint64_t
#include <future>
#include <iostream>
#include <coroutine> // clang++-10 found
// #include <experimental/coroutine> // clang++-10 fatal error: 'experimental/coroutine' file not found


using namespace std;

future<int> async_add(int a, int b)
{
    auto fut = std::async([=]() {
        int c = a + b;
        return c;
    });

    return fut;
}

future<int> async_fib(int n)
{
    if (n <= 2)
        co_return 1;
        // clang++-10 error: std::experimental::coroutine_traits type was not found; include <experimental/coroutine> before defining a coroutine
        // g++-10.2 error: unable to find the promise type for this coroutine


    int a = 1;
    int b = 1;

    // iterate computing fib(n)
    for (int i = 0; i < n - 2; ++i)
    {
        int c = co_await async_add(a, b);
        a = b;
        b = c;
    }

    co_return b;
}

future<void> test_async_fib()
{
    for (int i = 1; i < 10; ++i)
    {
        int ret = co_await async_fib(i);
        // g++-10.2 error: unable to find the promise type for this coroutine
        cout << "async_fib(" << i << ") returns " << ret << endl;
    }
}

int main()
{
    auto fut = test_async_fib();
    fut.wait();

    return 0;
}



coroutine-simple.cpp:41:9: error: unable to find the promise type for this coroutine
   41 |         co_return 1;
      |         ^~~~~~~~~
coroutine-simple.cpp: In function ‘std::future<void> test_async_fib()’:
coroutine-simple.cpp:64:19: error: unable to find the promise type for this coroutine
   64 |         int ret = co_await async_fib(i);

