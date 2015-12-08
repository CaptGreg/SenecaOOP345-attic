#include <vector>  // any of VECTOR, DEQUE, LIST, ... also work
#include <string>
#include <iostream>
using namespace std;

int main ()
{
  class X {
    string n;
  public:
  X() { cout << "ctor() " << n << "\n"; }
  X(string&& name) : n(move(name))  { cout << "ctor(&&) " << n << "\n"; }
  X(string& name)  : n(name)        { cout << "ctor(&)  " << n << "\n"; }

  ~X() { cout << "dtor " << n << "\n"; }

  X(const X& rhs)  { cout << "copy ctor " << n << " rhs=" << rhs.n << "\n"; n = rhs.n; }
  X(X&& rhs)       { cout << "MOVE ctor " << n << " rhs=" << rhs.n << "\n"; n = move( rhs.n); rhs.n.clear();}

  X&  operator= (const X& rhs)  { cout << "copy = op rhs=" << rhs.n << "\n"; if(this!=&rhs) n=rhs.n; return *this; }
  X&& operator= (X&& rhs) { cout << "MOVE = op rhs=" << rhs.n << "\n"; if(this!=&rhs) {n=rhs.n; rhs.n.clear();} return move(*this); }
  };

  class Y {
    vector<X> v;
  public:
    void push_back(const X&  x)                    { v.push_back(x);       }
    void push_back(X&& x)                          { v.push_back(move(x)); }
    vector<X>::iterator begin()                    { return v.begin();     }
    vector<X>::iterator end()                      { return v.end();       }
    const vector<X>::const_iterator cbegin() const { return v.cbegin();    }
    const vector<X>::const_iterator cend()   const { return v.cbegin();    }
  };

  class Z : public vector<X> {
  };

  {
    cout << "\n++++++++++++++++ Y block scope start\n";
    X x1("x1"), x2("x2");
    Y y; 
    cout << "y.push_back(x1);\n";
    y.push_back(x1); 
    cout << "y.push_back(move(x2));\n";
    y.push_back(move(x2));
    cout << "++++++++++++++++ Y block scope over\n";
    cout << "\n";
  }
  

  {
    cout << "\n++++++++++++++++ Z block scope start\n";
    X x1("x1"), x2("x2");
    Z z; 
    cout << "z.push_back(x1);\n";
    z.push_back(x1); 
    cout << "z.push_back(move(x2));\n";
    z.push_back(move(x2));
    cout << "++++++++++++++++ Z block scope over\n";
    cout << "\n";
  }

  cout << "main over\n";
}
