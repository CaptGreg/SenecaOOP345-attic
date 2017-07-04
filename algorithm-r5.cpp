#include <iostream>
#include <cstring>    /// memcpy
#include <vector>
#include <algorithm>  /// misc algorithms

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
    cout << "X destructor, size=" << size << " freeing up " << 1.* size * sizeof(T) << " BYTES\n";
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
        

        cout << "assignment operator DEEP COPY " << 1.* size * sizeof(T) << " BYTES!\n";
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
    cout << (void*) this << ": size=" << size << " (" << size*sizeof(T) << " BYTES)\n";
  }

  X&& operator= (X&& rhs) // C++11 move assignment operator
  {
    cout << "move assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      delete [] data;

      size = rhs.size; // Steal brains (copy size+pointer)
      data = rhs.data;

      rhs.size = 0;    // Zombie
      rhs.data = nullptr;
    } else {
      cout << "move assignment operator called on itself\n";
    }
    return std::move(*this);
  }

  X(X&& rhs) // C++11 move constructor
  {
    cout << "move constructor rhs.size=" << rhs.size << "\n";
    data = nullptr;
    *this = std::move(rhs);      // Let move assignment operator do the work.
  }
};

int main(int argc, char**argv)
{
 cout << "program " << argv[0] << "\n\n";

 vector<X<double>> v;

 cout << "\n\nLINE " << __LINE__ << ": v.push_back(X<double> (100));\n";
 v.push_back(X<double> (100000000));
 v.back().print();

 cout << "\n\nLINE " << __LINE__ << ": v.push_back(X<double> (200));\n";
 v.push_back(X<double> (200000000));
 v.back().print();

 cout << "\n\nLINE " << __LINE__ << ": v.push_back(X<double> (300));\n";
 v.push_back(X<double> (300000000));
 v.back().print();

 cout << "\n\nLINE " << __LINE__ << ": v.emplace_back(X<double> (300));\n";
 v.emplace_back(400000000);
 v.back().print();

 cout << "\n\nLINE " << __LINE__ << ": index based for:\n";
 for(size_t i = 0; i <  v.size(); i++) {
   cout << "LINE " << __LINE__ << ": index based for v[i].print(); ";
   v[i].print();
 }

 cout << "\n\nLINE " << __LINE__ << ": iterator based for:\n";
 for(auto it = v.begin(); it != v.end(); it++) {
   cout << "LINE " << __LINE__ << ": iterator based for it->print(); ";
   it->print();
 }

 cout << "\n\nLINE " << __LINE__ << ": range based for:\n";
 for(auto e: v) {
   cout << "\nLINE " << __LINE__ << ": range based for e.print(); ";
   e.print();
 }

 cout << "\n\nLINE " << __LINE__ << ": range based for by reference:\n";
 for(auto& e: v) {
   cout << "LINE " << __LINE__ << ": range based for by reference e.print(); ";
   e.print();
 }

 cout << "\n\nLINE " << __LINE__ << ": for_each algorithm:\n";
 for_each(v.begin(), v.end(), // sequence 
   [] (X<double>& e) { 
     cout << "LINE " << __LINE__ << ": for_each lambda e.print(); ";
     e.print();
   });

 cout << "\n\nLINE " << __LINE__ << ": transform algorithm:\n";
 transform(
   v.begin(), v.end(), // input sequence
   v.begin(),          // output sequence
   [] (X<double>& e) { // transform function
     cout << "LINE " << __LINE__ << ": transform lambda e.print(); ";
     e.print();
     return e;
   });


 cout << "\n\nLINE " << __LINE__ << ": MAIN OVER\n\n";
}
