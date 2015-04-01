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
  d.name       = "Derived Class";
  d.Base::name = "Base Class";

  // call Derived::display()
  d.display();

  // call Base::display()
  d.Base::display();
}
