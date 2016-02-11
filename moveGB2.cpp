#include <iostream>
#include <memory.h> // memcpy
using namespace std;

class X {
const int ALLOC_SIZE = 1000000;
double *d;
public:

  X() { 
    d = new double [ALLOC_SIZE]; 
    cout << "X this=" << (void*)d << "\n";
  }
  ~X() { 
    cout << "X destructor " << (void*) this << "\n";
    if(d) { delete [] d; } // move operations may have deleted the data
    else { cout << "(Zombie)\n"; }
  }
  X(X& that) // copy constructor
  {
    cout << "copy constructor this=" << (void*)this << "\n";
    d = new double [ALLOC_SIZE]; 
    cout << "copy constructor: calling memcpy\n";
    memcpy (d, that.d, sizeof(double) * ALLOC_SIZE);
  }
  X(X&& that) // move constructor
  { 
    cout << "move constructor this,that.d=" << (void*)this <<"," <<(void*)d << "\n";
    cout << "moving pointer\n";
    // d = std::move(that.d);
    // or
    d = that.d;
    that.d = nullptr;
  }
  X& operator=(X& that)   // assignment operator
  {
    cout << "assignment op: this,that=" << (void*) this << "," << (void*) &that << "\n";
    if(this != &that) {
      cout << "assignment op: calling memcpy\n";
      memcpy (d, that.d, sizeof(double) * ALLOC_SIZE);
    } else {
      cout << "assignment op: this == that\n";
    }
    return *this;
  }
  X& operator=(X&& that)   // move assignment operator
  {
    cout << "move assignment op: this,that=" << (void*) this << "," << (void*) &that << "\n";
    if(this != &that) {
      cout << "moving pointer\n";
      if(d) delete [] d;
      d = that.d;
      that.d = nullptr;
    } else {
      cout << "move assignment op: this == that\n";
    }
    return *this;
  }
  void print(const char *label) {
    cout << label << ": " << (void*) this << " d=" << (void*) d << "\n";
  }
};


int main(int argc, char**argv)
{
  X x1;
  X x2;
  X x3;
  x1.print("x1");
  x2.print("x2");
  x3.print("x3");

  cout << "\nx1 = x2;                   // assignment operator\n";
  x1 = x2;                   // assignment operator
  x1.print("x1");
  x2.print("x2");
  x3.print("x3");

  cout << "\nx3 = std::move( x1 );      // move assignment operator\n";
  x3 = std::move( x1 );      // move assignment operator
  x1.print("x1");
  x2.print("x2");
  x3.print("x3");

  cout << "\nx3 = std::move( x3 );      // move assignment operator\n";
  x3 = std::move( x3 );      // move assignment operator

  cout << "\nX x4(std::move(x2));       // move constructor\n";
  X x4(std::move(x2));       // move constructor
  x1.print("x1");
  x2.print("x2");
  x3.print("x3");
  x4.print("x4");

  cout << "\nX x5(x4);                 // copy constructor\n";
  X x5(x4);                 // copy constructor
  x1.print("x1");
  x2.print("x2");
  x3.print("x3");
  x4.print("x4");
  x5.print("x5");
 
  cout << "\nx5 = x5;\n";
  x5 = x5;

  cout << "\nall done: main terminating\n";
}
