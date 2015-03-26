// Question?  Can you use auto to make a function template like function?

#include <iostream>
using namespace std;

template <typename T>
void swapTemplate(T& a, T& b) { T x = a; a = b; b = x; }

// void swapAuto(auto a, auto b) { auto x = a; a = b; b = x; } // doesn't compile

int main(int argc, char ** argv)
{
  int a = 1;
  int b = 2;

  cout << "a,b=" << a << "," << b << "\n";
  swapTemplate(a,b);
  cout << "a,b=" << a << "," << b << "\n";
}
