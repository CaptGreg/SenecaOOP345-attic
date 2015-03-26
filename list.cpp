// GB some confusion with respect to list push_back.
// Our bison list prints in reverse order.  Why?
// This code prints the expected order.

#include <iostream>
#include <list>
#include <string>

int main ()
{
  std::list<std::string> l;
  l.push_back ("1");
  l.push_back ("2");
  l.push_back ("3");

  std::cout << "l contains:";
  for (auto e : l) std::cout << ' ' << e; std::cout << '\n';

  return 0;
}

