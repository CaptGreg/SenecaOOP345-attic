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

 MACROSTACK( MacroStack, double ) 

 MacroStack ms;

 ms.push(1.1);
 ms.push(2.2);
 ms.push(3.3);

 ms.print();

 MACROSTACK( MacroStackString, string ) 

 MacroStackString mss;

 mss.push("first");
 mss.push("second");
 mss.push("last");

 mss.print();
}

