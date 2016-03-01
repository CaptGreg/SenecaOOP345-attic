// ctor throws implies the ctor failed, C++ does not need to call dtor, and it doesn't

#include <iostream>
#include <memory>

using namespace std;

int main()
{
  class X {
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
  try { p = new X; } 
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
