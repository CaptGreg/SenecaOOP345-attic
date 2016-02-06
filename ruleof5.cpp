#include <iostream>
#include <cstring>    /// memcpy

using namespace std;

template <class T>
class X {
  size_t size;
  T *data;
public:
  X() : size(0), data(nullptr) 
  { cout << "X ctor()\n"; }

  X(size_t s) : size(s), data(new T[size]) 
  { cout << "X (" << s << ") ctor\n"; }

  ~X() { 
    cout << "X dtor, size=" << size << "\n";
    if(data) // if(data) not needed but good practice. 'delete [] nullptr' handled correctly in C++.
      delete [] data;
  }

  X& operator= (const X& rhs) // C++
  {
    cout << "assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      if(data) // if(data) not needed but good practice. 'delete [] nullptr' handled correctly in C++.
        delete [] data;

      data = nullptr;
      size = 0;

      if(rhs.data) {
        size = rhs.size;
        data = new T[size];
        memcpy( data, rhs.data, size * sizeof(T) );
        // data = memcpy( new T[size], rhs.data, size * sizeof(T) );  // 1 line
      }
    } else {
      cout << "assignment operator  called on itself\n";
    }
    return *this;
  }

  X(const X& rhs) // C++
  {
    cout << "copy ctor rhs.size=" << rhs.size << "\n";
    data = nullptr;
    *this = rhs;      // let assignment operator do the work.
  }

  X&& operator= (X&& rhs) // C++11
  {
    cout << "move assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      if(data) // if(data) not needed but good practice. 'delete [] nullptr' handled correctly in C++.
        delete [] data;

      size = rhs.size; // steal brains (copy size+pointer)
      data = rhs.data;

      rhs.size = 0;    // Zombie
      rhs.data = nullptr;
    } else {
      cout << "move assignment operator  called on itself\n";
    }
    return std::move(*this);
  }

  X(X&& rhs) // C++11
  {
    cout << "move ctor rhs.size=" << rhs.size << "\n";
    data = nullptr;
    *this = std::move(rhs);      // let assignment operator do the work.
  }


  void print() 
  {
    cout << "size=" << size << " (" << size*sizeof(T) << " BYTES)\n";
  }
};

int main(int argc, char**argv)
{
 cout << "program " << argv[0] << "\n\n";

 X<double> x, y(125 * 1000 * 1000);  // 8 * 125 * 1000 * 1000 = 1,000,000,000 BYTES

 cout << "LINE " << __LINE__ << ": x = y;\n";
 x = y;

 cout << "LINE " << __LINE__ << ": X<double> z(x);\n";
 X<double> z(x);

 cout << "LINE " << __LINE__ << ": y = std::move(x);\n";
 y = std::move(x);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": x = std::move(y);\n";
 x = std::move(y);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": y = std::move(x);\n";
 y = std::move(x);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": x = std::move(y);\n";
 x = std::move(y);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": X<double> A(std::move(x));\n";
 X<double> A(std::move(x));
 cout << "x:"; x.print();
 cout << "A:"; A.print();

 cout << "LINE " << __LINE__ << ": MAIN OVER\n\n";
}
