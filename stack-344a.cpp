#include <iostream>
#include <string>
using namespace std;

#include "stack.h"

int main(int argc, char**argv)
{
  Stack<string> s;

  s.push("hello");
  s.push("there");
  s.push("C++");

  s.print();

  MACROSTACK(StackString,string);

  StackString ss;

  ss.push("StackString: hello");
  ss.push("StackString: there");
  ss.push("StackString: C++");

  ss.print();
}
