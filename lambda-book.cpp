
// Lambda Expressions
 // lambda.cpp

 #include <iostream>

 int main() {
     // lambda expressions
     // auto hello = []() { std::cout << "Hello World" << "\n"; }; // GB added missing terminating ;
     // auto add4  = [](int i) { return i + 4; };
     // auto sub4  = [](int i) { return i - 4; };

     // calls on the lambda expressions
     // std::cout << hello()  << "\n";
     // std::cout << add4(10) << "\n";
     // std::cout << sub4(10) << "\n";

     std::cout << []() { std::cout << "Hello World" << "\n"; }  << "\n";
     auto r = []() { std::cout << "Hello World" << "\n"; };

     return 42;
 }
