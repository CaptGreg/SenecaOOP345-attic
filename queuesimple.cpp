#include <iostream>
#include <string>
using namespace std;

#include "queuesimple.h"

int main(int argc, char**argv)
{
  Queue<string> q;

  q.push("queue first push: hello");
  q.push("queue second push: there");
  q.push("queue third push: C++");

  // q.print(); // or walk with a print function
  q.walk( [] (string d) { cout << d << "\n"; } );
}
