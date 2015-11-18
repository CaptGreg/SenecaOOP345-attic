// https://akrzemi1.wordpress.com/2013/10/10/too-perfect-forwarding/

#include <iostream>
using namespace std;

struct Wrapper
{
  Wrapper(Wrapper const&) { cout << "copy\n"; }
  Wrapper(Wrapper &&) { cout << "move\n"; }
 
  template <typename T>
  Wrapper(T&&) { cout << "forward\n"; }
};

// We have a normal copy constructor, a normal move constructor and a perfect-forwarding constructor. Probably a typical situation for a wrapper class. This T&& is exactly what Scott Meyers calls a universal reference. Now imagine the code that uses our type:
	
Wrapper w = 1; // forwarding constructor
Wrapper v = w; // which constructor?


int main()
{
  return -0;
}
