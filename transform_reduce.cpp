// https://en.cppreference.com/w/cpp/algorithm/transform_reduce
// Equivalent to 
//  transform_reduce(first1, last1, first2, init, std::plus<>(), std::multiplies<>());
// effectively parallelized version of the default std::inner_product

// http://en.cppreference.com/w/cpp/algorithm/transform_reduce

// g++-8 -Wall -std=c++17   transform_reduce.cpp -o transform_reduce -pthread
// g++-8 -Wall -std=c++1y   transform_reduce.cpp -o transform_reduce -pthread
// g++-8 -Wall -std=c++1z   transform_reduce.cpp -o transform_reduce -pthread
// g++-8 -Wall -std=gnu++17 transform_reduce.cpp -o transform_reduce -pthread
// g++-8 -Wall -std=gnu++2a transform_reduce.cpp -o transform_reduce -pthread

#include <vector>
#include <functional>
#include <iostream>
#include <numeric>
// #include <execution> // error: execution: No such file or directory

#include <parallel/parallel.h> 
// error: ‘std::execution’ has not been declared std::execution::par,
//  reduce.cpp:47:30: error: ‘reduce’ is not a member of ‘std’
//          double result = std::reduce(std::execution::par, v.begin(), v.end());
//                               ^~~~~~
// reduce.cpp:47:42: error: ‘std::execution’ has not been declared
//          double result = std::reduce(std::execution::par, v.begin(), v.end());

#include <algorithm> // http://en.cppreference.com/w/cpp/header/execution says <execution> part of <algorithm>
// #include <algorithm/execution> // error: algorithm/execution: No such file or directory

// error: ‘reduce’ is not a member of ‘std’
//          double result = std::reduce(std::execution::par, v.begin(), v.end());
//                               ^~~~~~
// reduce.cpp:55:30: note: suggested alternative: ‘replace’
//          double result = std::reduce(std::execution::par, v.begin(), v.end());
//                               ^~~~~~
//                               replace
// reduce.cpp:55:42: error: ‘std::execution’ has not been declared
//          double result = std::reduce(std::execution::par, v.begin(), v.end());


// $ find /usr/include  -name execution
// (nothing found)

// https://en.cppreference.com/w/cpp/algorithm/transform_reduce
int main()
{
  int size = 3;
  std::vector<double> X(size, 1.0), Y(size, 1.0);

  std::transform(X.begin(), X.end(), X.begin(), [] (double v) ->double {static double c=0; return c++;}); // GB
  std::transform(Y.begin(), Y.end(), Y.begin(), [] (double v) ->double {static double c=0; return c++;}); // GB

  // double result = std::transform_reduce // error: ‘transform_reduce’ is not a member of ‘std’
  double result = std::inner_product // GB
  (
    // std::execution::par,       // GB someday...
    X.begin(), X.end(),
    Y.begin(), 
    0.0
  );
  std::cout << result << '\n';

  return 0;
}
