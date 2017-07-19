 // Algorithms - Execution Policies - C++17
 // policies.cpp
 // Taken from https://scs.senecac.on.ca/~oop345/pages/content/multi.html on 2017-07-18 

 #include <iostream>
 #include <numeric>
 #include <vector>
   #include <execution> 
   // g++ -std=c++17 thread-policy.cpp -o thread-policy -pthread
   // 2017-07-18 g++ (Ubuntu 5.4.1-2ubuntu1~16.04) 5.4.1 20160904 
   // fatal error: execution: No such file or directory
   //
   // /usr/bin/g++-6 -std=c++17 thread-policy.cpp -o thread-policy -pthread
   // g++-6 (Ubuntu/Linaro 6.3.0-18ubuntu2~16.04) 6.3.0 20170519
   // fatal error: execution: No such file or directory
   //
   // running "locate -r  '/execution$'" returns nothing


 int main() {
     std::vector<double> x(10000000, 0.5);

     double s = std::reduce(std::execution::par, 
      x.begin(), x.end());
     std::cout << "sum = " << s <<  std::endl;
 }
