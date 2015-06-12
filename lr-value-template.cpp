#include <iostream>
#include <functional> // std::bind std::function
using namespace std;

template <typename T>
void print(T&  i) { cout << i << " <-- is an L-value (function template)\n"; }
template <typename T>
void print(T&& i) { cout << i << " <-- is an R-value (function template)\n"; }

// can we override a function template that happens to be overloaded (same name)?
// NO!    :-(
// template <>
// void print(int&  i) { cout << i << " <-- is an L-value (function template override - specializion)\n"; }
// template <>
// void print(int&& i) { cout << i << " <-- is an R-value (function template override - specialization)\n"; }

int main()
{
  int i = 99;
  print(i);
  print(199);
  print(9+5);
  print(i+2);
  print(std::move(i));

  double trouble = 1.2;
  print(trouble);
  print(std::move(trouble));
  print("Hi there!");
  char *p = "hey you!";
  print(p);
  print(std::move(p));
 
#define P(val) cout << #val << " has a value of "; print(val);
  P(i);
  P(199);
  P(9+5);
  P(i+2);
  P(std::move(i));

  P(trouble);
  P(std::move(trouble));
  P("Hi there!");
  P(p);
  P(std::move(p));
  auto f = 1.2f;
  P(f);
  P(std::move(f));

  // try a lambda function
  auto l = [] () { return 5; };
  P(l);              // interesting, why does it compile? prints 1, wonder why?
  P(std::move(l));   // interesting, why does it compile? prints 1, wonder why?
  P(l());            // call the lambda function
  P(std::move(l())); // call the lambda function

  int (*n)()             = [] () { return 3; };  // C 1977 style pointer, C++11 lambda
  P(n);             // no error
  P(n());
  std::function<int()> o = [] () { return 3; };   // C++ style pointer, C++11 lambda, syntax error
  // P(o);             // syntax error
  P(o());

  // try a function object
  class MFO {public: int operator() () { return 77; } };
  MFO mfo;
  P(mfo());            // call the function object, returns 99, which is a r-value
  P(std::move(mfo())); // call the function object, returns 99, which is a r-value
}
