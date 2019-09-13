// map-reduce LISP circa late 60's
// C++ calls map transform

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
  uint64_t millisecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::milli> >(stop-start).count(); }
  uint64_t microsecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::micro> >(stop-start).count(); }
  uint64_t nanosecs () { return std::chrono::duration_cast<std::chrono::duration<int,std::nano>  >(stop-start).count(); }
};

int main()
{
    Timer timer;
    double result;

    const int SIZE = 100'000'000;
    std::vector<double> X(SIZE, 2.5), Y(SIZE, 1.5);

    timer.Start();
    result = std::inner_product(X.begin(), X.end(), Y.begin(), 0.0, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << result << ", " << 1e-6*timer.microsecs() << " s. inner_product\n";
    // no parallel execution support for inner_product
 
    // map-reduce inner_product
    timer.Start();
    result = std::transform_reduce(std::execution::seq, X.begin(), X.end(), Y.begin(), 0.0);
    timer.Stop();
    std::cout << result << ", " << 1e-6*timer.microsecs() << " s. transform_reduce seq\n";

    // map-reduce parallel inner_product
    // transform_reduce(first1, last1, first2, init, std::plus<>(), std::multiplies<>())
    timer.Start();
    result = std::transform_reduce( std::execution::par, X.begin(), X.end(), Y.begin(), 0.0, std::plus<>(), std::multiplies<>());
    timer.Stop();
    std::cout << result << ", " << 1e-6*timer.microsecs() << " s. transform_reduce plus/multipies par\n";

    // map-reduce parallel inner_product with DIY plus/multiplies
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
    std::cout << result << ", " << 1e-6*timer.microsecs() << " s. transform_reduce DIY plus/multipies par\n";

    // map-reduce parallel inner_product with default plus/multiplies
    timer.Start();
    result = std::transform_reduce(std::execution::par, X.begin(), X.end(), Y.begin(), 0.0);
    timer.Stop();
    std::cout << result << ", " << 1e-6*timer.microsecs() << " s. transform_reduce par\n";
}
