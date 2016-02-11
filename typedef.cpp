#include <iostream>
using namespace std;


#include <typeinfo>

// a student thought typedef(var).name() returned whether 'var' was a l or r value.
// does it?
// NO - syntax error (as expected)
// but does typeid(var).name() return r or l value?
// NO!
template<typename T>
// void print(T& i) { cout << typedef(i).name() << "\n"; }
void print(T& i)  { cout << i << " is an l-value, typeid(t).name() = " << typeid(i).name() << "\n"; }

template<typename T>
// void print(T&& i) { cout << typedef(i).name() << "\n"; }
void print(T&& i) { cout << i << " is an r-value, typeid(t).name() = " << typeid(i).name() << "\n"; }

int main(int argc, char**argv)
{
#ifdef DEBUG
  typedef float REAL;
#else
  typedef double REAL;
#endif

typedef class XXX { };

  REAL x = 9.0;

  cout << " sizeof(REAL) = " << sizeof(REAL) << "\n";

  cout << x << "\n";
  
  print(x);
  print(move(x));

  int y = 99;
  print(y);
  print(move(y));


  int* pi = & y;
  print(pi);
  print(move(pi));

  short s = 99;
  print(s);
  print(move(s));

  unsigned short us = 99;
  print(us);
  print(move(us));
}
