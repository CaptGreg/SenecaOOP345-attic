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
    cout << endl << argv[0] << endl << endl;
    C c;
    c.display();
    return 0;
}
