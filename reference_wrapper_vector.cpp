 // Functional - reference wrapper
 // reference_wrapper_vector.cpp

 #include <iostream>
 #include <vector>
 #include <functional>

 int main() {
     std::vector<double> v(5, 10.3);

     std::vector<std::reference_wrapper<double>> r(v.begin(), v.end());

     for (auto& i : v)
         i *= 3;

     for (auto i : r)
        std::cout << i << " ";

     std::cout << "\n"; 
 }
