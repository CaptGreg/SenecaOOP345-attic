#include <iostream>
#include <cstring>    /// memcpy

using namespace std;

template <class T>
class X {
  size_t size;
  T *data;
public:
  X() : size(0), data(nullptr) 
  { cout << "X constructor()\n"; }

  X(size_t s) : size(s), data(new T[size]) 
  { cout << "X (" << s << ") constructor\n"; }

  ~X() { 
    cout << "X destructor, size=" << size << "\n";
    if(data) // if(...) not needed but good practice. 'delete [] nullptr' handled correctly in C++.
      delete [] data;
  }

  X& operator= (const X& rhs)
  {
    cout << "assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      if(data) // If(data) is needed.  The copy construconstructor may have a block of memory with garbage in the 'data' field.
        delete [] data;

      data = nullptr;  // we are a zombie
      size = 0;

      if(rhs.data) {
        size = rhs.size;
        data = new T[size];
        memcpy( data, rhs.data, size * sizeof(T) );  // returns dest (data in our case)
        // data = memcpy( new T[size], rhs.data, size * sizeof(T) );  // 1 line
      }
    } else {
      cout << "assignment operator called on itself\n";
    }
    return *this;
  }

  X(const X& rhs)
  {
    cout << "copy constructor rhs.size=" << rhs.size << "\n";
    data = nullptr;
    *this = rhs;      // let assignment operator do the work.
  }

  void print() 
  {
    cout << "size=" << size << " (" << size*sizeof(T) << " BYTES)\n";
  }
};

int main(int argc, char**argv)
{
 cout << "program " << argv[0] << "\n\n";

 cout << "LINE " << __LINE__ << ": X<double> x, y(1000);\n";
 X<double> x, y(1000);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": x = y;\n";
 x = y;
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": X<double> z(x);\n";
 X<double> z(x);
 cout << "z:"; z.print();
 cout << "x:"; x.print();

 cout << "LINE " << __LINE__ << ": MAIN OVER\n\n";
}
