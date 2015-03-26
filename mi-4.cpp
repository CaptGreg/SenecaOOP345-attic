// convert a pointer to a derived class to a pointer to a base class

#include <iostream>
using namespace std;

class Base {
public:
  const char* name;
  void display() {
    cout << name << endl;
  }
};

class Derived: public Base {
public:
  const char* name;
  void display() {
    cout << name << ", " << Base::name << endl;
   }
};

int main (int argc, char**argv)
{
  cout << endl << argv[0] << endl << endl;
  Derived d;
  d.name = "Derived Class";
  d.Base::name = "Base Class";

  Derived* dptr = &d;

  // standard conversion from Derived* to Base*
  Base* bptr = dptr;

  // call Base::display()
  bptr->display();
}

