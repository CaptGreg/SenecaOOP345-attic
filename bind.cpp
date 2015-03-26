 // Functional - bind a function to its arguments
 // bind.cpp

 #include <iostream>
 #include <functional>

 double multiply(double x, double y) { return x * y; } 

 int main() {
     auto p = std::bind(multiply, 10, 3);
     std::cout << "Product = " << p() << std::endl;
 }
