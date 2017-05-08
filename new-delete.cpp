#include <iostream>
#include <exception>
#include <malloc.h>

using namespace std;

#if 1
// DIY new + delete
void* operator new (std::size_t n)
{
  cout << "DIY new allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes)\n";
  void *p = malloc(n);
  cout << "DIY new allocate pointer = " << p << "\n";
  if(p) return p;
  cerr << "DIY new failed to allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes) - throwing std::bad_alloc()\n";
  throw std::bad_alloc();
}
void* operator new[] (std::size_t n)
{
  cout << "DIY new[] allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes)\n";
  // return new(n);
  void *p = malloc(n);
  cout << "DIY new[] allocate pointer = " << p << "\n";
  if(p) return p;
  cerr << "DIY new[] failed to allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes) - throwing std::bad_alloc()\n";
  throw std::bad_alloc();
}

void operator delete(void * p)
{
  cout << "DIY delete: pointer=" << p << "\n";
  if(p) free(p);
}
void operator delete[](void * p)
{
  cout << "DIY delete[]: pointer=" << p << "\n";
  // ::delete p;
  if(p) free(p);
}
#endif


template <class T>
class X { // a rule-of-five class
  size_t size;
  T*     data;
public:
  X() : X(0) { cout << "X constructor()\n"; }

  X(size_t s) : size(s), data(new T[size]) { cout << "X (" << s << ") constructor\n"; }

  ~X() { cout << "X destructor, size=" << size << "\n"; delete [] data; }

  void print() { cout << "size=" << size << " (" << size*sizeof(T) << " BYTES)\n"; }

  X& operator= (const X& rhs) // C++
  {
    cout << "assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      if(data) // if(data) is needed.  The copy constructor may have a block of memory with garbage in the 'data' field.
        delete [] data;

      data = nullptr;
      size = 0;

      if(rhs.data) {
        size = rhs.size;
        data = memcpy( new T[size], rhs.data, size * sizeof(T) );
      }
    } else {
      cout << "assignment operator called on itself\n";
    }
    return *this;
  }

  X(const X& rhs) // C++ copy constructor
  {
    cout << "copy constructor rhs.size=" << rhs.size << "\n";
    data = nullptr;
    *this = rhs;      // Let copy assignment operator do the work.
  }

  X&& operator= (X&& rhs) // C++11 move assignment operator
  {
    cout << "move assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      if(data) // if(data) is needed.  The move constructor may have a block of memory with garbage in the 'data' field.
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


X<double> xGlobal0;
X<double> xGlobal1(1000000000);;
int intGlobal;

int main()
{
  X<float> xLocal0;
  X<float> xLocal1(1000000000);
  int intLocal;

  cout << "address of xGlobal0  =" << (void*) &xGlobal0 << ", size=" << sizeof(xGlobal0) << "\n";
  cout << "address of xGlobal1  =" << (void*) &xGlobal1 << ", size=" << sizeof(xGlobal1) << "\n";
  cout << "address of intGlobal =" << (void*) &intGlobal << ", size=" << sizeof(intGlobal) << "\n";
  cout << "address of xLocal0   =" << (void*) &xLocal0 << ", size=" << sizeof(xLocal0) << "\n";
  cout << "address of xLocal1   =" << (void*) &xLocal1 << ", size=" << sizeof(xLocal1) << "\n";
  cout << "address of intLocal  =" << (void*) &intLocal << ", size=" << sizeof(intLocal) << "\n";

  cout << "main starting\n";

  atexit( [] { cout << "atexit callback: main completed\n"; } );

  try {
   uint8_t *m = nullptr;

   operator<< (cout, "freeing nullptr\n"); // call 'operator<<' overload function. Same as 'cout << "freeing nullptr\n";'
   free(m);
   operator<< (cout, "double free of nullptr\n");
   free(m);

   uint8_t *cpp = nullptr;

   operator<< (cout, "deleting nullptr\n");
   delete cpp;
   operator<< (cout, "double nullptr delete\n");
   delete [] cpp; // double delete

// cpp = new uint8_t[1000LL * 1000LL * 1000LL * 1000LL]; // 1 terabyte 1e12 bytes
   cpp = new uint8_t[4 * 1000LL * 1000LL * 1000LL];      // 4 gigabytes
 
   operator<< (cout, "delete 4GB\n");
   delete    cpp;

   // operator<< (cout, "double delete of 4GB\n");
   // delete [] cpp; // double delete

  } catch (std::exception& e) { 
    cerr << e.what() << "\n";
  } catch (decltype(nullptr)) {
    cerr << "threw decltype(nullptr)\n";
  } catch (...) {
    cerr << "threw ...\n";
  }
  cout << "end of main\n";
}
