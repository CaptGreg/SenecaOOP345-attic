 // Functional - create a reference wrapper
 // ref.cpp

 #include <iostream>
 #include <functional>

 void incrementRef(int& x, int& y) { ++x, ++y; }
 void incrementPtr(int* px, int* py) { ++*px, ++*py; }
 void incrementValue(int x, int y) { ++x, ++y; }

 int main() {
     int a = 10, b = 20;

     std::cout << "std::ref" << std::endl;
     incrementRef(std::ref(a), std::ref(b));
     std::cout << "a = " << a << std::endl;
     std::cout << "b = " << b << std::endl;

     std::cout << "compiler generated reference call" << std::endl;
     incrementRef(a, b);
     std::cout << "a = " << a << std::endl;
     std::cout << "b = " << b << std::endl;

     std::cout << "pointer call" << std::endl;
     incrementPtr(&a, &b);
     std::cout << "a = " << a << std::endl;
     std::cout << "b = " << b << std::endl;

     std::cout << "value call" << std::endl;
     incrementValue(a, b);
     std::cout << "a = " << a << std::endl;
     std::cout << "b = " << b << std::endl;
 }
