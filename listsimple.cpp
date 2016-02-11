#include <iostream>
#include <string>
using namespace std;

#include "stacksimple.h"
#include "queuesimple.h"

int main(int argc, char**argv)
{
  Stack<string> s;

  s.push("stack first push: hello");
  s.push("stack second push: there");
  s.push("stack third push: C++");

  // s.print(); // or walk with a print function
  s.walk( [] (string d) { cout << d << "\n"; } );

cout << "\n";
cout << "-----------------------------\n";
cout << "\n";

  Queue<string> q;

  q.push("queue first push: hello");
  q.push("queue second push: there");
  q.push("queue third push: C++");

  // q.print(); // or walk with a print function
  s.walk( [] (string d) { cout << d << "\n"; } );
}
