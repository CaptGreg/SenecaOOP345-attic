// Question?  Can you use auto to make a function template like function?

#include <iostream>
using namespace std;

template <typename T>
void swapTemplate(T& a, T& b) { T x = a; a = b; b = x; }

template <typename T> void print(T& a)  { cout << "L-Value: " << a << "\n"; }
template <typename T> void print(T&& a) { cout << "R-Value: " << a << "\n"; }

// void swapAuto(auto a, auto b) { auto x = a; a = b; b = x; } // doesn't compile

int main(int argc, char ** argv)
{
  int a = 1;
  int b = 2;

  cout << "a,b=" << a << "," << b << "\n";
  swapTemplate(a,b);
  cout << "a,b=" << a << "," << b << "\n";

  print(a);
  print(b);
  print(a+b);
  print("sdfghjk");
  print(9);

  int j = 7;
  print(j);
  print(j+9);

}
