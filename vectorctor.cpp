// explore 
//   vector push_back (X())
//   vector emplace_back ()
//   vector push_back ( *new X() )                   <-- but why?
//   vector push_back ( *unique_ptr<X> (new X()) )   <-- but why?

#include <iostream>
#include <map>
#include <memory>     // unique_ptr
#include <vector>
using namespace std;

class X { // a rule-of-five class
public:
  static map<X*,string> instanceMap;   // track how instances were created
  static int            instanceCount; // how many instances were created
  X()  { cout << "X()  this=" << (void*) this << "\n";
         instanceMap.insert(make_pair(this,string("ctor"))); 
         instanceCount++;
  }
  ~X() { cout << "~X() dtor this=" << (void*) this << " created by " << instanceMap[this] << "\n";
         instanceMap.erase(this);
  }
  void print(const char* arg)  { cout << "print(arg)  this=" << (void*) this << " arg=" << arg << "\n"; }
  X(const X& rhs) { 
    cout << "X(const X& rhs) cp ctor this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
    instanceMap.insert(make_pair(this,string("cp ctor")));
    instanceCount++;
  }
  X& operator= (const X& rhs) {
    cout << "X& operator= (const X& rhs) this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
    if(this!=&rhs) *this = rhs;
    return *this;
  }
  X(X&& rhs) { 
    cout << "X(X&& rhs) mv ctor this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
    instanceMap.insert(make_pair(this,string("mv ctor")));
    instanceCount++;
  }
  X&& operator= (X&& rhs) {
    cout << "X&& operator= (X&& rhs) this=" << (void*) this << ", rhs=" << &rhs << "\n"; 
    if(this!=&rhs) *this = move(rhs);
    return move(*this);
  }
};
map<X*,string> X::instanceMap;                // track how active instances were created
int            X::instanceCount;              // how many instances were created
   
int main()
{
  atexit([] { 
    cout << "\n+++++ atexit +++++\n"; 
    cout << "instanceCount   = " << X::instanceCount << "\n";
    cout << "activeInstances = " << X::instanceMap.size() << "\n";
    for(auto e : X::instanceMap) 
      std::cout << "  " << (void*) e.first << " -> " << e.second << "\n";
  } );

  vector<X> v;

  cout << "\nv.push_back( X() );\n";
  v.push_back( X() );  // calls ctor, move ctor, dtor

  cout << "\nv.emplace_back();\n";
  v.emplace_back();  // calls ctor, copy ctor, dtor

  cout << "\nv.push_back( move(X()) );\n";
  v.push_back( move(X()) ); // calls ctor, move ctor, dtor (deletes zombie)

  cout<<"\nv.push_back( *new X() ); <-- HORRIBLE, HORRIBLE BROKEN CODE: DTOR NEVER CALLED\n";
  v.push_back( *new X() );  // calls ctor, copy ctor bad idea: DTOR NEVER CALLED

  cout <<"\nv.push_back( *unique_ptr<X>(new X()) );\n";
  v.push_back( *unique_ptr<X>(new X()) ); // calls ctor, copy ctor, dtor
  // (identical to v.push_back(X()); )

  cout << "\nv.push_back( move(*unique_ptr<X>(new X())) );\n";
  v.push_back( move(*unique_ptr<X>(new X())) ); // calls ctor, move ctor, dtor (deletes zombie)
  // (identical to v.push_back( move(X()) ); )

  v.back().print("v.back()");

  cout << "\nX x = move(v.back());\n";
  X x = move(v.back());
  x.print("x");

  v.pop_back();

  cout << "\n+++++ END-OF-MAIN +++++\n";

  return 0;
}
