// algorithm: sequential inner_product vs parallel transform_reduce (g++9.1/clang++-8 or later with -ltbb)

#include <algorithm> 
#include <chrono>
#include <execution>
#include <iostream>
#include <vector>

int main()
{
  class Timer {
    decltype(std::chrono::high_resolution_clock::now()) start, stop;
  public:
    void Start() { start = std::chrono::high_resolution_clock::now(); }
    void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
    uint64_t nanosecs () { return std::chrono::duration_cast<std::chrono::duration<int,std::nano>  >(stop-start).count(); }
  } t;
  float nsInner_product;
  float nsTransform_reduce;

  const int SIZE = 1'000'000;
  std::vector<double> X(SIZE, 1.0), Y(SIZE, 0.5);

  t.Start();
  double result = std::inner_product ( X.begin(), X.end(), Y.begin(), 0.0);
  t.Stop();
  nsInner_product = t.nanosecs();
  std::cout << "sequential inner_product    " << result << " in " << 1e-3*nsInner_product << " us.\n";

  t.Start();
  result = transform_reduce( std::execution::seq, X.begin(), X.end(), Y.begin(), 0.0 );
  t.Stop();
  nsTransform_reduce = t.nanosecs();
  std::cout << "sequential transform_reduce " << result << " in " << 1e-3*nsTransform_reduce << " us.\n";

  t.Start();
  result = transform_reduce( std::execution::par, X.begin(), X.end(), Y.begin(), 0.0 );
  t.Stop();
  nsTransform_reduce = t.nanosecs();
  std::cout << "parallel   transform_reduce " << result << " in " << 1e-3*nsTransform_reduce << " us.\n";

  t.Start();
  result = transform_reduce( std::execution::par, X.begin(), X.end(), Y.begin(), 0.0 );
  t.Stop();
  nsTransform_reduce = t.nanosecs();
  std::cout << "parallel   transform_reduce " << result << " in " << 1e-3*nsTransform_reduce << " us.\n";

  std::cout << "once the thread pool starts up, parallel transform_reduce is about " << (0.+nsInner_product)/(0.+nsTransform_reduce)  << " times faster than sequential inner_product\n";

  return EXIT_SUCCESS;
}
