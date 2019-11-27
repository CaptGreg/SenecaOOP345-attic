// motivated from reduce example at http://en.cppreference.com/w/cpp/algorithm/reduce 
// std::reduce exists in g++9.1 or later

// -Ofast makes a huge difference

// g++-9.1   -Wall -std=c++17 -fopenmp -Ofast reduce.cpp -o reduce -pthread -ltbb && ./reduce
// g++-9.2   -Wall -std=c++17 -fopenmp -Ofast reduce.cpp -o reduce -pthread -ltbb && ./reduce
// clang++-8 -Wall -std=c++17 -fopenmp -Ofast reduce.cpp -o reduce -pthread -ltbb && ./reduce

#include <cmath>             // rand
#include <chrono>
#include <execution>         // execution::par, ::seq, ...
#include <functional>        // bind
#include <future>            // async
#include <iostream>
#include <numeric>           // accumulate, reduce
#include <omp.h>
#include <parallel/numeric>  // __parallel::accumulate (gnu?)
#include <thread>            // hardware_concurrrency
#include <vector>

using namespace std;

int main(int argc, char*argv[])
{
  std::cout <<"+++++++++++++++++++++++++++++++\n";
  std::cout << argv[0] << "\n";
  std::cout << "FILE '" << __FILE__ << "' compiled " 
            << __DATE__ << " at " << __TIME__ 
#ifdef __GNUC__  // either g++ or clang++
  #ifdef __clang__
    << " by clang++ "
  #else
    << " by g++ "
  #endif
   << __VERSION__
#endif
   << "\n\n";

    vector<double> v(1000 * 1000 * 1000, 0.5); // 8 * 1e9 RAM
    transform(begin(v), end(v), begin(v), [] (double d) { return rand() / (RAND_MAX +0.); });
 
  #if 0
    // auto bbb  = bind(std::accumulate, begin(v), end(v), 0.);
    // error: no matching function for call to ‘bind(<unresolved overloaded function type>, ...
    // auto bbb  = std::bind(std::accumulate, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    // same error: no matching function for call to ‘bind(<unresolved overloaded function type>, ...

    // auto timeFunction = [] (decltype(bbb) func, const char* label) 

    auto timeFunction = [] (function<double(void)> func, const char* label)  // no error
    {
        auto tStart = chrono::high_resolution_clock::now();
          double result = func();
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << fixed << label << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    };
    // timeFunction( bind(std::accumulate, begin(v), end(v), 0.), "accumulate result ");  // can't call it.
    // same error: no matching function for call to ‘bind(<unresolved overloaded function type>, ...
  #endif

    {
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        result = accumulate(begin(v), end(v), result);
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << fixed << "accumulate result " << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    };
 
    {
        cout << "Same calculation using __parallel::accumulate(begin(v), end(v), 0.) on a " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        result = __parallel::accumulate(begin(v), end(v), result);
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "__parallel::accumulate result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using indexed for-loop.\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        for(unsigned i = 0; i < v.size(); i++)
          result += v[i];
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "indexed for-loop result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using iterator for-loop.\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        for(auto it = v.begin(); it != v.end(); ++it)
          result += *it;
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "iterator for-loop result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using range-based-for-loop.\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        for(double e :  v)
          result += e;
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "range-based-for-loop result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using OMP PARALLEL FOR for-loop on a " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        #pragma omp parallel for reduction(+:result)
        // #pragma omp simd reduction(+:result) // simd omp is twice as slow 
        for(size_t i = 0; i < v.size(); i++)
          result += v[i];
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "OMP PARALLEL FOR for-loop result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "AGAIN, in case OMP PARALLEL FOR initialized a thread pool which remains active.\n";
        cout << "Same calculation using OMP PARALLEL FOR for-loop on a " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        #pragma omp parallel for reduction(+:result)
        // #pragma omp simd reduction(+:result) // simd omp is twice as slow 
        for(size_t i = 0; i < v.size(); i++)
          result += v[i];
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "OMP PARALLEL FOR for-loop result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using DIY threaded indexed for-loop on a " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        size_t N = thread::hardware_concurrency();
        size_t chunk = (v.size() + N-1) / N;
        vector<future<double>> futs;
        size_t beg = 0;
        for(size_t i = 0; i < N; i++) {
          size_t end = beg + chunk;
          if(i == N) 
            end = v.size();
          futs.emplace_back( 
            async(
                  launch::async,
                  [&v] (size_t beg, size_t end) 
                    { double r = 0.; for(auto i=beg; i!=end; i++) r += v[i]; return r; }, 
                  beg, 
                  end
                )
          );
          beg = end;
        }

        double result = 0.;
        for(auto& e : futs)
          result += e.get();
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "DIY threaded indexed for-loop result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using DIY threaded iterator for-loop on a " << thread::hardware_concurrency() << " core machine:\n";    
        auto tStart = chrono::high_resolution_clock::now();
        size_t N = thread::hardware_concurrency();
        size_t chunk = (v.size() + N-1) / N;
        vector<future<double>> futs;
        auto beg = v.begin();
        for(size_t i = 0; i < N; i++) {
          auto end = beg + chunk;
          if(i == N) 
            end = v.end();
          futs.emplace_back( 
            async(
                      launch::async,
                      [] (decltype(v.begin()) beg, decltype(v.end()) end) 
                        { double r = 0.; for(auto it=beg; it!=end; ++it) r += *it; return r; }, 
                      beg, 
                      end
                    )
            );
          beg = end;
        }

        double result = 0.;
        for(auto& e : futs)
          result += e.get();
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << " DIY threaded iterator for-loop result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using reduce(std::execution::seq, begin(v), end(v), 0.) on a " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        result = reduce(std::execution::seq, begin(v), end(v), result);
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "reduce(std::execution::seq,...) result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using reduce(std::execution::par, begin(v), end(v), 0.) on a " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        result = reduce(std::execution::par, begin(v), end(v), result);
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "reduce(std::execution::par,...) result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }
 
    {
        cout << "AGAIN, in case std::reduce initialized a thread pool which remains active.\n";
        cout << "Same calculation using reduce(std::execution::par, begin(v), end(v), 0.) on a " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        result = reduce(std::execution::par, begin(v), end(v), result);
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "reduce(std::execution::par,...) result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }

 #if 0 // OMP doesn't support range-based for-loops
    {
        // OMP spec (even 5.0) states all for-loops must be canonical.  That is 'for(init; test; increment)'.

        cout << "Same calculation using OMP PARALLEL FOR range-based-for-loop concurrency on " << thread::hardware_concurrency() << " core machine:\n";
        auto tStart = chrono::high_resolution_clock::now();
        double result = 0.;
        #pragma omp parallel for
        for(double e :  v)
          result += e;
        auto tStop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = tStop - tStart;
        cout << "__parallel::accumulate result "
                  << (uint64_t) result << " took " << ms.count() << " ms\n\n";
    }
#endif

    return 0;
}

/*
g++-9.2 -Wall -std=c++17 -fopenmp -Ofast reduce.cpp -o reduce -pthread -ltbb && ./reduce
+++++++++++++++++++++++++++++++
./reduce
FILE 'reduce.cpp' compiled Aug 16 2019 at 11:17:10 by g++ 9.2.0

accumulate result 500000000 took 558.977853 ms

Same calculation using __parallel::accumulate(begin(v), end(v), 0.) on a 16 core machine:
__parallel::accumulate result 500000000 took 251.344152 ms

Same calculation using indexed for-loop.
for-loop result 500000000 took 3456.176756 ms

Same calculation using iterator for-loop.
for-loop result 500000000 took 561.269577 ms

Same calculation using range-based-for-loop.
range-based-for-loop result 500000000 took 561.610121 ms

Same calculation using OMP PARALLEL FOR for-loop on a 16 core machine:
OMP PARALLEL FOR for-loop result 500000000 took 244.699200 ms

AGAIN, in case OMP PARALLEL FOR initialized a thread pool which remains active.
Same calculation using OMP PARALLEL FOR for-loop on a 16 core machine:
OMP PARALLEL FOR for-loop result 500000000 took 243.754653 ms

Same calculation using DIY threaded indexed for-loop on a 16 core machine:
DIY threaded indexed for-loop result 500000000 took 242.540649 ms

Same calculation using DIY threaded iterator for-loop on a 16 core machine:
 DIY threaded iterator for-loop result 500000000 took 241.967781 ms

Same calculation using reduce(std::execution::seq, begin(v), end(v), 0.) on a 16 core machine:
reduce(std::execution::seq,...) result 500000000 took 573.393391 ms

Same calculation using reduce(std::execution::par, begin(v), end(v), 0.) on a 16 core machine:
reduce(std::execution::par,...) result 500000000 took 242.351666 ms

AGAIN, in case std::reduce initialized a thread pool which remains active.
Same calculation using reduce(std::execution::par, begin(v), end(v), 0.) on a 16 core machine:
reduce(std::execution::par,...) result 500000000 took 240.733932 ms


clang++-8 -Wall -std=c++17 -fopenmp -Ofast reduce.cpp -o reduce -pthread -ltbb && ./reduce
+++++++++++++++++++++++++++++++
./reduce
FILE 'reduce.cpp' compiled Aug 16 2019 at 11:24:51 by clang++ 4.2.1 Compatible Clang 8.0.0 (tags/RELEASE_800/final)

accumulate result 500000000 took 353.601189 ms

Same calculation using __parallel::accumulate(begin(v), end(v), 0.) on a 16 core machine:
__parallel::accumulate result 500000000 took 284.073083 ms

Same calculation using indexed for-loop.
for-loop result 500000000 took 362.430558 ms

Same calculation using iterator for-loop.
for-loop result 500000000 took 352.819760 ms

Same calculation using range-based-for-loop.
range-based-for-loop result 500000000 took 351.791070 ms

Same calculation using OMP PARALLEL FOR for-loop on a 16 core machine:
OMP PARALLEL FOR for-loop result 500000000 took 286.831452 ms

AGAIN, in case OMP PARALLEL FOR initialized a thread pool which remains active.
Same calculation using OMP PARALLEL FOR for-loop on a 16 core machine:
OMP PARALLEL FOR for-loop result 500000000 took 299.921981 ms

Same calculation using DIY threaded indexed for-loop on a 16 core machine:
DIY threaded indexed for-loop result 500000000 took 250.032659 ms

Same calculation using DIY threaded iterator for-loop on a 16 core machine:
 DIY threaded iterator for-loop result 500000000 took 241.922787 ms

Same calculation using reduce(std::execution::seq, begin(v), end(v), 0.) on a 16 core machine:
reduce(std::execution::seq,...) result 500000000 took 355.609328 ms

Same calculation using reduce(std::execution::par, begin(v), end(v), 0.) on a 16 core machine:
reduce(std::execution::par,...) result 500000000 took 240.568686 ms

AGAIN, in case std::reduce initialized a thread pool which remains active.
Same calculation using reduce(std::execution::par, begin(v), end(v), 0.) on a 16 core machine:
reduce(std::execution::par,...) result 500000000 took 241.841865 ms


+++++++++++++++++++++++++++++++
Above timings in a table

                                        g++ 9.2.0        clang++ 4.2.1 Compatible 
                                                         Clang 8.0.0

accumulate                              558.977853 ms    353.601189 ms
__parallel::accumulate                  251.344152 ms    284.073083 ms
indexed for-loop.                       3456.176756 ms   362.430558 ms
iterator for-loop.                      561.269577 ms    352.819760 ms
range-based-for-loop.                   561.610121 ms    351.791070 ms
OMP PARALLEL FOR                        244.699200 ms    286.831452 ms
AGAIN, OMP PARALLEL FOR                 243.754653 ms    299.921981 ms
DIY threaded indexed for-loop           242.540649 ms    250.032659 ms
DIY threaded iterator for-loop          241.967781 ms    241.922787 ms
reduce(std::execution::seq,...)         573.393391 ms    355.609328 ms
reduce(std::execution::par,...)         242.351666 ms    240.568686 ms
AGAIN, reduce(std::execution::par,...)  240.733932 ms    241.841865 ms

*/
