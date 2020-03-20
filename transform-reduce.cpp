// C++17 transform_reduce delivers LISP, circa late 60's, map-reduce

// -Ofast compiling makes an incredible difference
//    C++11: g++-9.2 -Wall -std=c++17 -Ofast  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  transform-reduce.cpp -o transform-reduce -lrt -pthread -ltbb && ./transform-reduce
//    C++2a: g++-9.2 -Wall -std=c++2a -Ofast  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  transform-reduce.cpp -o transform-reduce -lrt -pthread -ltbb && ./transform-reduce

#include <chrono>
#include <cmath>      // fabs
#include <execution>
#include <functional> // std::plus, std::multiplies
#include <iostream>
#include <numeric>
#include <vector>

class Timer {
  decltype(std::chrono::high_resolution_clock::now()) start, stop;
public:
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t microsecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::micro> >(stop-start).count(); }
};

// typedef double FLOAT;           // works
typedef float FLOAT;         // not enough bits in 32-bit float to hold increments is sum is large and increment is small
FLOAT EPS = 1e-6;

int main(int argc, char* argv[])
{
  std::cout <<"+++++++++++++++++++++++++++++++\n";
  std::cout << argv[0] << "\n";
  std::cout << "FILE '" << __FILE__ << "' compiled " << __DATE__ << " at " << __TIME__ 
#ifdef __GNUC__  // either g++ or clang++
  #ifdef __clang__
    << " by clang++ "
  #else
    << " by g++ "
  #endif
   << __VERSION__
#endif
   << "\n\n";

    Timer timer;
    double result = 0;

    std::cout << "sizeof(FLOAT)=" << sizeof(FLOAT) << "\n";

    const int SIZE = 1'000'000;   // 1 million WORKS, 100 million exhibits addition truncation error
    FLOAT xValue = 2.5;
    FLOAT yValue = 1.5;
    std::vector<FLOAT> X(SIZE, xValue), Y(SIZE, yValue);
    FLOAT RESULT = SIZE * xValue * yValue;                      // 3.75e+08
    std::cout << "expected result=" << RESULT << "\n";

    auto check = [&result,RESULT] {
        if(fabs(result - RESULT) > EPS) {
          std::cout << "INCORRECT RESULT\n";
          // return EXIT_FAILURE;
        }
    };

    std::cout << "\n";

  // C++ for int index, forward loop
    timer.Start();
    result = 0;
    for(size_t i = 0; i < X.size(); i++) // expensive size_t i vs X.size() comparison
      result += X[i] * Y[i];
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++ for, forward index i expensive size_t i vs X.size() comparison\n";
    check();


  // C++ for int index, reverse loop
    timer.Start();
    result = 0;
    for(int i = X.size()-1; i >= 0; i--) // inexpensive int comparison
      result += X[i] * Y[i];
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++ for, reverse index i inexpensive int comparison\n";
    check();

  // C++ for iterators
    timer.Start();
    result = 0;
    auto yIt = Y.begin();
    for(auto xIt = X.begin(); xIt != X.end(); xIt++)
      result += *xIt * *yIt++;
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++ for, iterators\n";
    check();

  // C++ while iterators
    timer.Start();
    result = 0;
    auto itX = X.begin(); 
    auto itY = Y.begin();
    while(itX != X.end())
      result += *itX++ * *itY++;
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++ while, iterators\n";
    check();


  // C++ for_each
    timer.Start();
    result = 0;
    auto yIter = Y.begin();
    std::for_each(X.begin(), X.end(), [&result,&yIter] (const FLOAT &x) { result += x * *yIter++; }); 
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++11 for_each\n";
    check();

  // base C++ inner_product
    std::cout << "\n";
    timer.Start();
    result = 0.f;
    result = std::inner_product(X.begin(), X.end(), Y.begin(), result);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++11 inner_product\n";
    check();
    // no parallel execution support for inner_product

 // base C++ inner_product
    timer.Start();
    result = 0;
    result = std::inner_product(X.begin(), X.end(), Y.begin(), result, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++11 inner_product FACTORY plus/multiplies\n";
    // no parallel execution support for inner_product
 
 
  // map-reduce transform_reduce
    std::cout << "\n";
    timer.Start();
    result = 0;
    result = std::transform_reduce(std::execution::seq, X.begin(), X.end(), Y.begin(), result);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce execution::seq\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce(std::execution::par, X.begin(), X.end(), Y.begin(), result);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce execution::par\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce(std::execution::par_unseq, X.begin(), X.end(), Y.begin(), result);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce execution::par_unseq\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce(std::execution::unseq, X.begin(), X.end(), Y.begin(), result);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++20 transform_reduce execution::unseq\n";



  // map-reduce parallel transform_reduce
    std::cout << "\n";
    // transform_reduce(first1, last1, first2, init, std::plus<>(), std::multiplies<>())
    timer.Start();
    result = 0;
    result = std::transform_reduce( std::execution::seq, X.begin(), X.end(), Y.begin(), result, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce FACTORY plus/multipies execution::seq\n";


    timer.Start();
    result = 0;
    result = std::transform_reduce( std::execution::par, X.begin(), X.end(), Y.begin(), result, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce FACTORY plus/multipies execution::par\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce( std::execution::par_unseq, X.begin(), X.end(), Y.begin(), result, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce FACTORY plus/multipies execution::par_unseq\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce( std::execution::unseq, X.begin(), X.end(), Y.begin(), result, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++20 transform_reduce FACTORY plus/multipies execution::unseq\n";

  // map-reduce parallel inner_product with DIY plus/multiplies
    std::cout << "\n";
    timer.Start();
    result = 0;
    result = std::transform_reduce(
        std::execution::seq,
        X.begin(), 
        X.end(),
        Y.begin(), 
        result,
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x+y; },
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce DIY lambda plus/multipies execution::seq\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce(
        std::execution::par,
        X.begin(), 
        X.end(),
        Y.begin(), 
        result,
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x+y; },
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce DIY lambda plus/multipies execution::par\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce(
        std::execution::par_unseq,
        X.begin(), 
        X.end(),
        Y.begin(), 
        result,
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x+y; },
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce DIY lambda plus/multipies execution::par_unseq\n";

    timer.Start();
    result = 0;
    result = std::transform_reduce(
        std::execution::unseq,
        X.begin(), 
        X.end(),
        Y.begin(), 
        result,
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x+y; },
        [](FLOAT x,FLOAT y) -> FLOAT { return (FLOAT) x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++20 transform_reduce DIY lambda plus/multipies execution::unseq\n";

    return EXIT_SUCCESS;
}

/*

  C++11: g++-9.2 -Wall -std=c++17 -Ofast  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  transform-reduce.cpp -o transform-reduce -lrt -pthread -ltbb && ./transform-reduce
  C++2a: g++-9.2 -Wall -std=c++2a -Ofast  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  transform-reduce.cpp -o transform-reduce -lrt -pthread -ltbb && ./transform-reduce
  clang: clang++ -Wall -std=c++2a -Ofast  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  transform-reduce.cpp -o transform-reduce -lrt -pthread -ltbb && ./transform-reduce

    +++++++++++++++++++++++++++++++
    ./transform-reduce
    FILE 'transform-reduce.cpp' compiled Mar 20 2020 at 10:58:46 by g++ 9.2.0

    sizeof(FLOAT)=8
    expected result=3.75e+06

    dot product=3.75e+06, time=0.00127s. C++ for, forward index i expensive size_t i vs X.size() comparison
    dot product=3.75e+06, time=0.00116s. C++ for, reverse index i inexpensive int comparison
    dot product=3.75e+06, time=0.001159s. C++ for, iterators
    dot product=3.75e+06, time=0.001153s. C++ while, iterators
    dot product=3.75e+06, time=0.001145s. C++11 for_each

    dot product=3.75e+06, time=0.001135s. C++11 inner_product
    dot product=3.75e+06, time=0.001192s. C++11 inner_product FACTORY plus/multiplies

    dot product=3.75e+06, time=0.000864s. C++17 transform_reduce execution::seq
    dot product=3.75e+06, time=0.001186s. C++17 transform_reduce execution::par
    dot product=3.75e+06, time=0.000502s. C++17 transform_reduce execution::par_unseq
    dot product=3.75e+06, time=0.00087s. C++20 transform_reduce execution::unseq

    dot product=3.75e+06, time=0.000707s. C++17 transform_reduce FACTORY plus/multipies execution::seq
    dot product=3.75e+06, time=0.000444s. C++17 transform_reduce FACTORY plus/multipies execution::par
    dot product=3.75e+06, time=0.000345s. C++17 transform_reduce FACTORY plus/multipies execution::par_unseq
    dot product=3.75e+06, time=0.00071s. C++20 transform_reduce FACTORY plus/multipies execution::unseq

    dot product=3.75e+06, time=0.000695s. C++17 transform_reduce DIY lambda plus/multipies execution::seq
    dot product=3.75e+06, time=0.000693s. C++17 transform_reduce DIY lambda plus/multipies execution::par
    dot product=3.75e+06, time=0.000307s. C++17 transform_reduce DIY lambda plus/multipies execution::par_unseq
    dot product=3.75e+06, time=0.000729s. C++20 transform_reduce DIY lambda plus/multipies execution::unseq

BEST DOUBLE is about 3x faster than C++11 inner_product
    dot product=3.75e+06, time=0.000307s. C++17 transform_reduce DIY lambda plus/multipies execution::par_unseq
VS
    dot product=3.75e+06, time=0.001135s. C++11 inner_product

    +++++++++++++++++++++++++++++++
    ./transform-reduce
    FILE 'transform-reduce.cpp' compiled Mar 20 2020 at 10:59:23 by g++ 9.2.0

    sizeof(FLOAT)=4
    expected result=3.75e+06

    dot product=3.75e+06, time=0.000386s. C++ for, forward index i expensive size_t i vs X.size() comparison
    dot product=3.75e+06, time=0.000358s. C++ for, reverse index i inexpensive int comparison
    dot product=3.75e+06, time=0.00028s. C++ for, iterators
    dot product=3.75e+06, time=0.000295s. C++ while, iterators
    dot product=3.75e+06, time=0.000379s. C++11 for_each

    dot product=3.75e+06, time=0.000368s. C++11 inner_product
    dot product=3.75e+06, time=0.000334s. C++11 inner_product FACTORY plus/multiplies

    dot product=3.75e+06, time=0.007072s. C++17 transform_reduce execution::seq
    dot product=3.75e+06, time=0.001603s. C++17 transform_reduce execution::par
    dot product=3.75e+06, time=0.000202s. C++17 transform_reduce execution::par_unseq
    dot product=3.75e+06, time=0.00073s. C++20 transform_reduce execution::unseq

    dot product=3.75e+06, time=0.000351s. C++17 transform_reduce FACTORY plus/multipies execution::seq
    dot product=3.75e+06, time=0.000193s. C++17 transform_reduce FACTORY plus/multipies execution::par
    dot product=3.75e+06, time=0.000118s. C++17 transform_reduce FACTORY plus/multipies execution::par_unseq
    dot product=3.75e+06, time=0.000304s. C++20 transform_reduce FACTORY plus/multipies execution::unseq

    dot product=3.75e+06, time=0.00611s. C++17 transform_reduce DIY lambda plus/multipies execution::seq
    dot product=3.75e+06, time=0.000569s. C++17 transform_reduce DIY lambda plus/multipies execution::par
    dot product=3.75e+06, time=0.000134s. C++17 transform_reduce DIY lambda plus/multipies execution::par_unseq
    dot product=3.75e+06, time=0.000624s. C++20 transform_reduce DIY lambda plus/multipies execution::unseq
    
BEST FLOAT is about 2x faster than C++ iterators
    dot product=3.75e+06, time=0.000118s. C++17 transform_reduce FACTORY plus/multipies execution::par_unseq
VS
    dot product=3.75e+06, time=0.00028s. C++ for, iterators
*/
