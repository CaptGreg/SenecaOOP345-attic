#include <iostream>
#include <fstream>

using namespace std;

class X {
  int x;
public:
  X() :x(0) { cout << "X ctor " << this << "\n"; }
  X(const int xx) :x(xx) { cout << "X ctor " << this << xx << "\n"; }
  X(const X& rhs) {x = rhs.x; cout << "X copy ctor " << this << " rhs=" << &rhs << "\n"; }
  ~X() { cout << "X dtor " << this << "\n"; }
  X& operator= (const X& rhs) { if(this!=&rhs) x=rhs.x; cout << "X =operator " << this << " rhs=" << &rhs << "\n"; return *this; }
  int get() { return x; }

  void operator++() { x++; cout << "X prefix++\n"; }
  // void operator++(int rhs) { x++; cout << "X postfix++, rhs=" << rhs << "\n"; }
  void operator++(int) { x++; cout << "X postfix++\n"; }
};

int main(int argc,char**argv)
{
  try {

    cout << "\n->X x1;\n";
    X x1;
    cout << "\n->X x2(x1);\n";
    X x2(x1);
    cout << "\n->X x3 = x2;\n";
    X x3 = x2;
    cout << "\n->X x4;\n";
    X x4(10);
    cout << "\n->x4 = x2;\n";
    x4 = x2;

    cout << "\ncalling prefix ++x1\n";
    ++x1;
    cout << "after prefix ++x1, x1.get=" << x1.get() << "\n";

    cout << "\ncalling postfix x1++\n";
    x1++;
    cout << "after postfix x1++, x1.get=" << x1.get() << "\n";

  } catch (const string& e) {
    cerr << "Threw: " << e << "\n";
  }
}

