#include <iostream>
using namespace std;

class Base {
public:
  const char* name;
  void display() { cout << name << "\n"; }
};

class Derived: public Base {
public:
  const char* name;
  void display() { cout << name << ", " << Base::name << "\n"; }
};

int main (int argc, char**argv)
{
  cout << "\n" << argv[0] << "\n" << "\n";
  Derived d;
  d.name       = "Derived Class";
  d.Base::name = "Base Class";

  // call Derived::display()
  d.display();

  // call Base::display()
  d.Base::display();
}
