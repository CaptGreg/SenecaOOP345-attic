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
 
  // MACROSTACK(NAME,T)

  MACROSTACK(MacroStack,string)

  MacroStack ms;

  ms.push("MacroStack: hello");
  ms.push("MacroStack: there");
  ms.push("MacroStack: C++");

  ms.print();
}
