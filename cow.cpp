// cow = copy on write
// see http://en.wikipedia.org/wiki/Copy-on-write

// GB prints added to illustrate COW mechanism in practise

#include <iostream>
#include <string>
using namespace std;

int main()
{
  std::string x("Hello");
  std::cout << "&x=" << &x 
            << ", (void*) x.c_str()=" << (void*) x.c_str()
            << ", x-->" << x << "<--\n";
 
  std::string y = x;  // x and y use the same buffer,  GB this is good
  std::cout << "&y=" << &y 
            << ", (void*) y.c_str()=" << (void*) y.c_str()
            << ", y-->" << y << "<--\n";

 
  y += ", World!";    // now y uses a different buffer,  GB this is good
                      // x still uses the same old buffer
  std::cout << "&y=" << &y 
            << ", (void*) y.c_str()=" << (void*) y.c_str()
            << ", y-->" << y << "<--\n";

  x[0] = 'h';    // change x, COW changes string,  GB this deep copy might be a surprise
  std::cout << "&x=" << &x 
            << ", (void*) x.c_str()=" << (void*) x.c_str()
            << ", x-->" << x << "<--\n";

}
