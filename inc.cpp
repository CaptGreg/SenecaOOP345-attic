// Everyone understands the ++ operator.  That is everyone kind-of understands the ++ operator:
//    ++x: increment x and return the new value.
//    x++: increment x and return the old value.

// No one understands user defined operator precidence.
// For example, many classes define an operator overload for '<<'.

// We all think we understand 'cout << x << y << z'  but do we?

#include <iostream>

int main()
{
  int x;
  int X[10];

  x = 1;
  std::cout << x++ << x++ << x++ << x++ << "\n";  // surprise!
  std::cout << x << "\n"; // everyone agrees on this

  x = 1;
  std::cout << ++x << ++x << ++x << ++x << "\n";  // a bigger surprise!
  std::cout << x << "\n"; // everyone agrees on this

#ifdef LAMBDA_EXPERIMENT
  // What?  Adding the following lambda code alters what happens above
  auto pre  = [](int& arg) { return ++arg; };
  auto post = [](int& arg) { return arg++; };

  x = 1;
  std::cout << post(x) << post(x) << post(x) << post(x) << "\n";
  std::cout << x << "\n";
  x = 1;
  std::cout << pre(x) << pre(x) << pre(x) << pre(x) << "\n";
  std::cout << x << "\n";
#endif

  x = 1;
  x = x++;
  std::cout <<"after x=1; x = x++; x is " << x << "\n";

  x = 1;
  x = ++x;
  std::cout << "after x=1; x = ++x; x is " << x << "\n";

  x = 1;
  x = x--;
  std::cout <<"after x=1; x = x--; x is " << x << "\n";

  x = 1;
  x = --x;
  std::cout << "after x=1; x = --x; x is " << x << "\n";

  x = 1;
  x = (++x)--;
  std::cout << "after x=1; x = (++x)--; x is " << x << "\n";

  x = 1;
  x = (--x)++;
  std::cout << "after x=1; x = (--x)++; x is " << x << "\n";

  x = 1;
  X[x] = X[x++];
  std::cout << "after x=1; X[x] = X[x++] x is " << x << "\n";

  x = 1;
  X[x] = X[++x];
  std::cout << "after x=1; X[x] = X[++x] x is " << x << "\n";

  x = 1;
  X[x] = X[(++x)++];
  std::cout << "after x=1; X[x] = X[(++x)++] x is " << x << "\n";

  x = 1;
  X[x] = X[(--x)--];
  std::cout << "after x=1; X[x] = X[(--x)--] x is " << x << "\n";

#ifdef SHOW_ERRORS
  x = 1;
  X[x] = X[++(x++)];
  std::cout << "after x=1; X[x] =X[++(x++)] x is " << x << "\n";
#endif
}
