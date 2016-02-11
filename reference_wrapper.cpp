 // Functional - reference wrapper
 // reference_wrapper.cpp

 #include <iostream>
 #include <functional>

 int main() {
     long v1 = 10L, v2 = 20L, v3 = 30L;

     std::reference_wrapper<long> r1 = v1;
     std::reference_wrapper<long> r2 = v2;
     std::reference_wrapper<long> r3 = v3;

     std::cout << "r1 = " << r1 << "\n"; 
     std::cout << "r2 = " << r2 << "\n";
     std::cout << "r3 = " << r3 << "\n";

     v1++, v2++, v3++;
     std::cout << "r1 = " << r1 << "\n"; 
     std::cout << "r2 = " << r2 << "\n";
     std::cout << "r3 = " << r3 << "\n";

     r1++, r2++, r3++;
     std::cout << "r1 = " << r1 << "\n"; 
     std::cout << "r2 = " << r2 << "\n";
     std::cout << "r3 = " << r3 << "\n";
 }
