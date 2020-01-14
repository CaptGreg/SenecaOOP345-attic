// C++17 transform_reduce delivers LISP, circa late 60's, map-reduce

#include <chrono>
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

int main(int argc, char* argv[])
{
  #ifdef __GNUC__
    #ifdef __clang__
      std::cout << argv[0] << " compiled with clang++ " << __VERSION__ << "\n";
    #else
      std::cout << argv[0] << " compiled with g++ " << __VERSION__ << "\n";
    #endif
  #endif

    Timer timer;
    double result;

    const int SIZE = 100'000'000;
    std::vector<double> X(SIZE, 2.5), Y(SIZE, 1.5);

  // base C++ inner_product
    std::cout << "\n";
    timer.Start();
    result = std::inner_product(X.begin(), X.end(), Y.begin(), 0.0, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++11 inner_product\n";
    // no parallel execution support for inner_product
 
  // map-reduce transform_reduce
    std::cout << "\n";
    timer.Start();
    result = std::transform_reduce(std::execution::seq, X.begin(), X.end(), Y.begin(), 0.0);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce execution::seq\n";

    timer.Start();
    result = std::transform_reduce(std::execution::par, X.begin(), X.end(), Y.begin(), 0.0);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce execution::par\n";

    timer.Start();
    result = std::transform_reduce(std::execution::par_unseq, X.begin(), X.end(), Y.begin(), 0.0);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce execution::par_unseq\n";

    timer.Start();
    result = std::transform_reduce(std::execution::unseq, X.begin(), X.end(), Y.begin(), 0.0);
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++20 transform_reduce execution::unseq\n";



  // map-reduce parallel transform_reduce
    std::cout << "\n";
    // transform_reduce(first1, last1, first2, init, std::plus<>(), std::multiplies<>())
    timer.Start();
    result = std::transform_reduce( std::execution::seq, X.begin(), X.end(), Y.begin(), 0.0, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce plus/multipies execution::seq\n";


    timer.Start();
    result = std::transform_reduce( std::execution::par, X.begin(), X.end(), Y.begin(), 0.0, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce plus/multipies execution::par\n";

    timer.Start();
    result = std::transform_reduce( std::execution::par_unseq, X.begin(), X.end(), Y.begin(), 0.0, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce plus/multipies execution::par_unseq\n";

    timer.Start();
    result = std::transform_reduce( std::execution::unseq, X.begin(), X.end(), Y.begin(), 0.0, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++20 transform_reduce plus/multipies execution::unseq\n";

  // map-reduce parallel inner_product with DIY plus/multiplies
    std::cout << "\n";
    timer.Start();
    result = std::transform_reduce(
        std::execution::seq,
        X.begin(), 
        X.end(),
        Y.begin(), 
        0.0,
        [](double x, double y){ return x+y; },
        [](double x,double y){ return x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce DIY lambda plus/multipies execution::seq\n";

    timer.Start();
    result = std::transform_reduce(
        std::execution::par,
        X.begin(), 
        X.end(),
        Y.begin(), 
        0.0,
        [](double x, double y){ return x+y; },
        [](double x,double y){ return x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce DIY lambda plus/multipies execution::par\n";

    timer.Start();
    result = std::transform_reduce(
        std::execution::par_unseq,
        X.begin(), 
        X.end(),
        Y.begin(), 
        0.0,
        [](double x, double y){ return x+y; },
        [](double x,double y){ return x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++17 transform_reduce DIY lambda plus/multipies execution::par_unseq\n";

    timer.Start();
    result = std::transform_reduce(
        std::execution::unseq,
        X.begin(), 
        X.end(),
        Y.begin(), 
        0.0,
        [](double x, double y){ return x+y; },
        [](double x,double y){ return x*y; } 
    );
    timer.Stop();
    std::cout << "dot product=" << result << ", time=" << 1e-6*timer.microsecs() << "s. C++20 transform_reduce DIY lambda plus/multipies execution::unseq\n";
}

/*

  g++-9.2 -Wall -std=c++17 -Ofast  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  transform-reduce.cpp -o transform-reduce -lrt -pthread -ltbb && ./transform-reduce

  transform-reduce compiled with g++ 9.2.0

  dot product=3.75e+08, time=0.069189s. C++11 inner_product

  dot product=3.75e+08, time=0.069685s. C++17 transform_reduce execution::seq
  dot product=3.75e+08, time=0.051035s. C++17 transform_reduce execution::par
  dot product=3.75e+08, time=0.050008s. C++17 transform_reduce execution::par_unseq
  dot product=3.75e+08, time=0.071572s. C++20 transform_reduce execution::unseq

  dot product=3.75e+08, time=0.069602s. C++17 transform_reduce plus/multipies execution::seq
  dot product=3.75e+08, time=0.050666s. C++17 transform_reduce plus/multipies execution::par
  dot product=3.75e+08, time=0.050117s. C++17 transform_reduce plus/multipies execution::par_unseq
  dot product=3.75e+08, time=0.064429s. C++20 transform_reduce plus/multipies execution::unseq

  dot product=3.75e+08, time=0.070757s. C++17 transform_reduce DIY lambda plus/multipies execution::seq
  dot product=3.75e+08, time=0.050153s. C++17 transform_reduce DIY lambda plus/multipies execution::par
  dot product=3.75e+08, time=0.05007s. C++17 transform_reduce DIY lambda plus/multipies execution::par_unseq
  dot product=3.75e+08, time=0.064817s. C++20 transform_reduce DIY lambda plus/multipies execution::unseq

  clang++-8 -Wall -std=c++17 -Ofast  -ggdb -fopenmp -Wfatal-errors  transform-reduce.cpp -o transform-reduce -lrt -pthread -ltbb && ./transform-reduce

  ./transform-reduce compiled with clang++ 4.2.1 Compatible Clang 8.0.0 (tags/RELEASE_800/final)

  dot product=3.75e+08, time=0.065322s. C++11 inner_product

  dot product=3.75e+08, time=0.065412s. C++17 transform_reduce execution::seq
  dot product=3.75e+08, time=0.050205s. C++17 transform_reduce execution::par
  dot product=3.75e+08, time=0.049625s. C++17 transform_reduce execution::par_unseq
  dot product=3.75e+08, time=0.065081s. C++20 transform_reduce execution::unseq

  dot product=3.75e+08, time=0.065323s. C++17 transform_reduce plus/multipies execution::seq
  dot product=3.75e+08, time=0.049833s. C++17 transform_reduce plus/multipies execution::par
  dot product=3.75e+08, time=0.04981s. C++17 transform_reduce plus/multipies execution::par_unseq
  dot product=3.75e+08, time=0.072151s. C++20 transform_reduce plus/multipies execution::unseq

  dot product=3.75e+08, time=0.065138s. C++17 transform_reduce DIY lambda plus/multipies execution::seq
  dot product=3.75e+08, time=0.049942s. C++17 transform_reduce DIY lambda plus/multipies execution::par
  dot product=3.75e+08, time=0.049884s. C++17 transform_reduce DIY lambda plus/multipies execution::par_unseq
  dot product=3.75e+08, time=0.069785s. C++20 transform_reduce DIY lambda plus/multipies execution::unseq

*/
