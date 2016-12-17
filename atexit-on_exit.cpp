#include <iostream>
#include <string>
using namespace std;

// QUESTION:
// Global class CTOR/DTOR are called before and after main executes.
// If atexit or on_exit callbacks are active, who goes first: the callbacks or the DTORs?

// ANSWER - A DTOR goes last

class X {
  string name;
  uint8_t memBlock[1024*1024];  // 1 MByte
public:
  X(const std::string& n) { name = n; cout << "X ctor " << name << "\n"; }
  ~X() { cout << "X dtor " << name << "\n"; }
};

X xgbefore("global xgbefore before main");

int main(int argc, char**argv)
{
  cout << "Main starting up\n";

  atexit( [] { cout << "Main is over, atexit callback function has control\n";} );
  char*p=new char[1000*1000*1000];
  cout << "p = " << (void*)p << "\n";
  cout << "registering on_exit callback\n";
  on_exit([] (int i,void*p) 
    { 
      cout << "on_exit callback active\n";
      cout << "i = " << i << "\n";
      cout << "p = " << p << "\n";
    },
    p // nullptr
  );

  cout << "Main exiting\n";

  return 99;
}

X xgafter("global xgafter after main");
