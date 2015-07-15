#include <iostream>
#include <string>
using namespace std;

class X {
string name;
public:
  X(const std::string& n) { name = n; cout << "X ctor " << name << "\n"; }
  ~X() { cout << "X dtor " << name << "\n"; }
  void hello() const { cout << "hello from " << name << "\n\n"; }
};

static X xgbefore("global xgbefore before main");

int main(int argc, char**argv)
{
  cout << "main starting...\n";
  atexit( [] () { cout << "atexit function\n"; } );
  X x(std::string("main x"));
  X xx("main xx");

  cout << "main terminating...\n";
}

X xgafter("global xgafter after main");
