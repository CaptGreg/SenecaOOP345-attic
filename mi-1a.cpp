#include <iostream>
using namespace std;

class A
{
    public:
      void display()
      {
          cout<<"Base class A display - no msg.\n";
      }
      void display(const char *msg)
      {
          cout<<"Base class A display: " << msg << endl;
      }
      int  Protect() const { return protect; }
      void Protect(int p)  { protect = p; }
    protected:
      int protect;
};

class B : public A
{
    public:
      int  Protect() const { return protect; }
      void Protect(int p)  { protect = p; }
    protected:
      int protect;

};

class C : public B
{
    public:
      int  Protect() const { return protect; }
      void Protect(int p)  { protect = p; }
    protected:
      int protect;

};

int main (int argc, char**argv)
{
    cout << endl << argv[0] << endl << endl;
    C c;

    c.A::Protect(1);
    c.B::Protect(2);
    c.Protect(3);

    c.display();

    c.display("called from an instance of C");

    cout << "c.A::protect = " << c.A::Protect() << endl;
    cout << "c.B::protect = " << c.B::Protect() << endl;
    cout << "c.C::protect = " << c.Protect() << endl;

    return 0;
}
