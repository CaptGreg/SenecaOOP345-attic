// http://www.cplusplus.com/doc/tutorial/inheritance/

// constructors and derived classes
#include <iostream>
using namespace std;

class Mother {
  public:
    Mother ()
      { cout << "Mother: no parameters\n"; }
    Mother (int a)
      { cout << "Base class Mother: int parameter a=" << a << "\n"; }
};

class Daughter : public Mother {
  public:
    Daughter (int a)
      { cout << "derived class Daughter: int parameter a=" << a << "\n\n"; }
};

class Son : public Mother {
  public:
    Son (int a) : Mother (a)
      { cout << "derived class Son: int parameter a=" << a << "\n\n"; }
};

int main () {
  Daughter kelly(123);
  Son bud(456);
  
  return 0;
}
