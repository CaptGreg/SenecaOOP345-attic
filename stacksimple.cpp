#include <iostream>
#include <string>
using namespace std;

#include "stacksimple.h"

int main(int argc, char**argv)
{
  Stack<string> s;

  s.push("stack first push: hello");
  s.push("stack second push: there");
  s.push("stack third push: C++");

  // s.print(); // or walk with a print function
  s.walk( [] (string d) { cout << d << "\n"; } );
}
