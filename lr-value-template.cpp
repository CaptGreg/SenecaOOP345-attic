#include <iostream>
using namespace std;

template <typename T>
void print(T&  i) { cout << i << " <-- is an L-value\n"; }
template <typename T>
void print(T&& i) { cout << i << " <-- is an R-value\n"; }

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
  P(l);              // interesting, prints 1, wonder why?
  P(std::move(l));   // interesting, prints 1, wonder why?
  P(l());            // call the lambda function
  P(std::move(l())); // call the lambda function

  // try a function object
  class X {public: int operator() () { return 77; } };
  X x;
  P(x());            // call the function object
  P(std::move(x())); // call the function object
}
