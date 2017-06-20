#include <iostream>
#include <cstring>    /// memcpy

using namespace std;

template <class T>
class X {
  size_t size = 0;         // C++14
  T *data     = nullptr;   // C++14
public:
  X() // : size(0), data(nullptr)    <-- initialization done above with C++14
  { cout << "X constructor()\n"; }

  X(size_t s) : size(s), data(new T[size]) 
  { cout << "X (" << s << ") constructor\n"; }

  ~X() { 
    cout << "X destructor, size=" << size << "\n";
    delete [] data;
  }

  X& operator= (const X& rhs) // C++98 copy assignment operator
  {
    cout << "assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      delete [] data;

      data = nullptr;  // we are a zombie
      size = 0;

      if(rhs.data) {
        size = rhs.size;
        data = new T[size];
        // void *memcpy(void *dest, const void *src, size_t n);
        // SYNOPSIS
               // #include <string.h>
               // void *memcpy(void *dest, const void *src, size_t n);
        // DESCRIPTION
               // The  memcpy()  function  copies  n bytes from memory area src to memory
               // area dest.  The memory areas must not overlap.  Use memmove(3)  if  the
               // memory areas do overlap.
        // RETURN VALUE
               // The memcpy() function returns a pointer to dest.
        
        memcpy( data, rhs.data, size * sizeof(T) );  // returns dest (data in our case)
        // data = memcpy( new T[size], rhs.data, size * sizeof(T) );  // 1 line
        // for(size_t i=0; i<size;i++) data[i]=rhs.data[i]; // DEEP COPY using a for loop
      }
    } else {
      cout << "assignment operator called on itself\n";
    }
    return *this;
  }

  X(const X& rhs) // C++98 copy constructor
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
 X<double> x, y(1000 * 1000);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": x = y;\n";
 x = y;
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": X<double> z(x);\n";
 X<double> z(x);   // X<double> z = x;  // same thing
 cout << "z:"; z.print();
 cout << "x:"; x.print();

 x = x;

 cout << "LINE " << __LINE__ << ": MAIN OVER\n\n";
}
