 // Functional - create a reference wrapper
 // ref.cpp

 #include <iostream>
 #include <functional>

 void incrementRef(int& x, int& y) { ++x, ++y; }
 void incrementPtr(int* px, int* py) { ++*px, ++*py; }
 void incrementValue(int x, int y) { ++x, ++y; }

 int main() {
     int a = 10, b = 20;

     std::cout << "std::ref" << "\n";
     incrementRef(std::ref(a), std::ref(b));
     std::cout << "a = " << a << "\n";
     std::cout << "b = " << b << "\n";

     std::cout << "compiler generated reference call\n";
     incrementRef(a, b);
     std::cout << "a = " << a << "\n";
     std::cout << "b = " << b << "\n";

     std::cout << "pointer call" << "\n";
     incrementPtr(&a, &b);
     std::cout << "a = " << a << "\n";
     std::cout << "b = " << b << "\n";

     std::cout << "value call" << "\n";
     incrementValue(a, b);
     std::cout << "a = " << a << "\n";
     std::cout << "b = " << b << "\n";
 }
