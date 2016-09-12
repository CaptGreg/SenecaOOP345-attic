#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// explore 
//   vector push_back (X())
//   vector push_back ( *new X() )
//   vector push_back ( *unique_ptr<X> (new X()) )

int main()
{
  atexit([] { cout << "atexit called\n"; } );

  class X {
  public:
    X()  { cout << "X()  this=" << (void*) this << "\n"; }
    ~X() { cout << "~X() this=" << (void*) this << "\n"; }
    void print(const char* arg)  { cout << "print(arg)  this=" << (void*) this << " arg=" << arg << "\n"; }
    X(const X& rhs) { 
      cout << "X(const X& rhs) this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
    }
    X& operator= (const X& rhs) {
      cout << "X& operator= (const X& rhs) this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
      // if(this!=&rhs) *this = rhs;
      return *this;
    }
    X(const X&& rhs) { 
      cout << "X(const X&& rhs) this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
    }
    X&& operator= (X&& rhs) {
      cout << "X&& operator= (X&& rhs) this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
      // if(this!=&rhs) *this = rhs;
      return move(*this);
    }
  };
   
  vector<X> v;

  // v.push_back( X() );  // calls ctor, copy ctor, dtor

  // v.push_back( move(X()) ); // calls ctor, move ctor, dtor (deletes zombie)

  // v.push_back( *new X() );  // calls ctor, copy ctor bad idea: dtor never called for first instance

  // v.push_back( *unique_ptr<X>(new X()) ); // calls ctor, copy ctor, dtor
  // (identical to v.push_back(X()); )

  v.push_back( move(*unique_ptr<X>(new X())) ); // calls ctor, move ctor, dtor (deletes zombie)
  // (identical to v.push_back( move(X()) ); )

  v.back().print("v.back()");

  cout << "X x = move(v.back());\n";
  X x = move(v.back());
  x.print("x");

  v.pop_back();

  cout << "END-OF-MAIN\n";

  return 0;
}

