// ctor throws implies the ctor failed, C++ does not need to call dtor, and it doesn't

// The actual mechanics are:
// 1. The ctor allocates space for any static sized private data members.
// 2. the ctor code executes.
// 3. If it throws, the provate data elelments are deallocated.

#include <exception>   // for std::bad_alloc
#include <cstdlib>     // for malloc() and free()
#include <memory>      // unique_ptr/smart_ptr
#include <new>         // prototypes for new/delete
#include <iostream>

using namespace std;

void* operator new (size_t size)
{
  void *p=malloc(size); 
  cout << "global new(" << size << ") = " <<p << "\n";
  if (p==0) // did malloc succeed?
    throw std::bad_alloc(); // ANSI/ISO compliant behavior
  return p;
}

void operator delete (void *p)
{
  cout << "global delete(" << p << ")\n";
  if(p) 
    free(p); 
}

int main()
{
  class X {
  uint8_t buf1MB [1000000];
  public:
    X()  { cout << "X() - will now throw.\n"; throw nullptr; }
    ~X() { cout << "~X()\n"; };
  };

  cout << "\nnormal instance\n";
  try { X x; }
  catch (...) { cout << "It threw.\n"; }
  cout << "Note: The dtor is never called\n";


  cout << "\nraw pointer to instance.\n";
  X* p = nullptr;
  try { p = new X; cout<< "'new X' succeeded - calling 'delete'\n"; delete p; } 
  catch (...) { cout << "It threw.\n"; }
  cout << "p=" << (void*) p << "\n";
  cout << "Note: The dtor is never called\n";

  cout << "\nunique_ptr to instance.\n";
  try { unique_ptr<X> up ( new X ); } 
  catch (...) { cout << "It threw.\n"; }
  cout << "Note: The dtor is never called\n";

  cout << "\nshared_ptr to instance.\n";
  shared_ptr<X> sp(nullptr);
  cout << "sp=" << (void*) sp.get() << ", use_count=" << sp.use_count() << "\n";
  try { sp = shared_ptr<X>(new X); } 
  catch (...) { cout << "It threw.\n"; }
  cout << "sp=" << (void*) sp.get() << ", use_count=" << sp.use_count() << "\n";
  cout << "Note: The dtor is never called\n";
}
