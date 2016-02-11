// g++ access.cc -o access; ./access

// http://www.aristeia.com/sdnotes_frames.html
// Scott Meyers
// 10 Cool Things in C++ -- Plus Something Wicked 


#include <iostream>
using namespace std;

class X;
      
class dispatch
{
  X& xref;
public:
  dispatch(X& x);
  ~dispatch();
  X* operator->();
};

class X
{
  char name;
public:
  X(char c) : name(c) { }
  void doSomething() { cout << "doSomething with " << name << "\n"; }
  void lock() { cout << "lock " << name << "\n"; }
  void unlock() { cout << "unlock " << name << "\n"; }
  dispatch operator->() { return *this; }
};

dispatch::dispatch(X& x) : xref(x) { xref.lock(); }
dispatch::~dispatch() { xref.unlock(); }
X* dispatch::operator->() { return &xref; }

int main(int argc, char**argv)
{
  X x('x');
  cout << "++++++\n";
  x->doSomething();  // lock/unlock calls
  cout << "------\n";
  x.doSomething();   // no lock/unlock calls
  return 0;
}     
