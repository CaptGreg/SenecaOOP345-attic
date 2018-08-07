 // Algorithms - Execution Policies - C++17
 // policies.cpp

 #include <iostream>
 #include <numeric>
 #include <vector>
 #include <execution>

 // GB std::reduce is an out-of-order threaded std::accumulate

 int main() {
     std::vector<double> x(10000000, 0.5);

     double s = std::reduce(std::execution::par, 
      x.begin(), x.end());
     std::cout << "sum = " << s <<  std::endl;
 }
