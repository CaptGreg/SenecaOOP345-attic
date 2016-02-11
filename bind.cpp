 // Functional - bind a function to its arguments
 // bind.cpp

int main1 ();

 #include <iostream>
 #include <functional>



 // GB bind does not like function templates, even specialized function templates
 // template<typename T>
 // T multiply(T x, T y) { return x * y; } 
 // template<>
 // double multiply(double x, double y) { return x * y; } 

 double multiply(double x, double y) { return x * y; } 

 int main() {
     auto p = std::bind(multiply, 10, 3);
     std::cout << "Product = " << p() << "\n";

     // GB code follows:-
     double x, y;
     x=1, y = 2;
     auto q = std::bind(multiply, x, y);
     std::cout << "bind multiply("<<x<<","<<y<<") q=" << q() << "\n";
     x=3, y=4;
     std::cout << "change x,y to ("<<x<<","<<y<<")\n";
     std::cout << "bind multiply("<<x<<","<<y<<") q=" << q() << "\n";
     // GB OK -	bind captures the arguments at the time of the bind call

     // what happens with lambdas?
     x=5, y=6;
     auto l  = [ x, y] () { return multiply(x,y); };
     auto l2 = [&x,&y] () { return multiply(x,y); };
     std::cout << "l  [ x, y] lambda Product = " << l() << "\n";
     std::cout << "l2 [&x,&y] lambda Product = " << l2() << "\n";
     x=7, y=8;
     std::cout << "change x,y to ("<<x<<","<<y<<")\n";
     std::cout << "l  [ x, y] lambda Product = " << l() << "\n";
     std::cout << "l2 [&x,&y] lambda Product = " << l2() << "\n";
     // NOTE lambda [&x,&y] captures values at time of the lambda call

     // clearly no surprise with a function object:-
     class MyFunctionObject {
       double x,y;
     public:
       void set(double x, double y) {this->x = x, this->y = y; }
       double operator() () { return multiply(x,y); }
     };
     MyFunctionObject mfo;
     mfo.set(x,y);
     std::cout << "MyFUnctionObject mfo Product = " << mfo() << "\n";

    main1();
    return 0;
 }
// http://www.cplusplus.com/reference/functional/bind/
// bind example
#include <iostream>     // std::cout
#include <functional>   // std::bind

// a function: (also works with function object: std::divides<double> my_divide;)
double my_divide (double x, double y) {return x/y;}

struct MyPair {
  double a,b;
  double multiply() {return a*b;}
};

int main1 () {
  using namespace std::placeholders;    // adds visibility of _1, _2, _3,...

  // GB bind parameters are function, args...
  // GB _1, _2, ... are parm 1, parm 2, ...

  // binding functions:
  auto fn_five = std::bind (my_divide,10,2);               // returns 10/2
  std::cout << fn_five() << '\n';                          // 5

  auto fn_half = std::bind (my_divide,_1,2);               // returns x/2
  std::cout << fn_half(10) << '\n';                        // 5

  auto fn_invert = std::bind (my_divide,_2,_1);            // returns y/x
  std::cout << fn_invert(10,2) << '\n';                    // 0.2

  auto fn_rounding = std::bind<int> (my_divide,_1,_2);     // returns int(x/y)
  std::cout << fn_rounding(10,3) << '\n';                  // 3

  MyPair ten_two {10,2};

  // binding members:
  auto bound_member_fn = std::bind (&MyPair::multiply,_1); // returns x.multiply()
  std::cout << bound_member_fn(ten_two) << '\n';           // 20

  auto bound_member_data = std::bind (&MyPair::a,ten_two); // returns ten_two.a
  std::cout << bound_member_data() << '\n';                // 10

  return 0;
}
