
// Lambda Expressions
 // lambda.cpp

 #include <iostream>

 int main() {
     // lambda expressions
     // auto hello = []() { std::cout << "Hello World" << std::endl; }; // GB added missing terminating ;
     // auto add4  = [](int i) { return i + 4; };
     // auto sub4  = [](int i) { return i - 4; };

     // calls on the lambda expressions
     // std::cout << hello()  << std::endl;
     // std::cout << add4(10) << std::endl;
     // std::cout << sub4(10) << std::endl;

     std::cout << []() { std::cout << "Hello World" << std::endl; }  << std::endl;
     auto r = []() { std::cout << "Hello World" << std::endl; };

     return 42;
 }
