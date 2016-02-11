#include <iostream>
using namespace std;

class A
{
    public:
      void display()
      {
          cout<<"Base class content.\n";
      }
};

class B : public A { };

class C : public B { };

int main (int argc, char**argv)
{
    cout << "\n" << argv[0] << "\n" << "\n";
    C c;
    c.display();
    return 0;
}
