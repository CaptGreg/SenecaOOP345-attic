// g++-5 -Wall -std=c++11 atexit.cpp -o atexit -lrt -pthread && ./atexit 
// g++   -Wall -std=c++11 atexit.cpp -o atexit -lrt -pthread && ./atexit 
// 
// Problem, how to pass a class member function to atexit?
// 'atexit' expects a void(*)() c-style function pointer
// Can we coerce a function object operator() or a member function to look like a void(*)()?

// NOTE: c++ style std::function(void(void)> fcpp;  atexit( fcpp ); does not compile.

// Solution: use g++ and capture FO with a lambda. 

#include <iostream>
#include <chrono>        // c++11 timers
#include <typeinfo>      // use typeid to help sort out problem
using namespace std;

#define TYPEID(X) cout << "typeid(" #X ").name()=" << typeid(X).name() << "\n";

namespace atexit_namespace { // wrap globals in a namespace
  chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
  void atexitCStyleFunction()
  {
    chrono::time_point<chrono::high_resolution_clock> stop = chrono::high_resolution_clock::now();

    typedef chrono::duration<int,nano> nanosecs_t ;
    nanosecs_t duration_get( chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    uint64_t ns = duration_get.count();

    cout << "atexitCStyleFunction(): program was active for " << ns << " nsecs.\n";
  }
} // namespace atexit_namespace

class Timer { // C++11 chrono stop-watch timer class. Needs "#include <chrono>".
  chrono::time_point<chrono::high_resolution_clock> start;
  chrono::time_point<chrono::high_resolution_clock> stop;
public:
 void Start() { start  = chrono::high_resolution_clock::now(); }
 void Stop()  { stop   = chrono::high_resolution_clock::now(); }
 uint64_t nsecs() {
    typedef chrono::duration<int,nano> nanosecs_t ;
    nanosecs_t duration_get( chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    uint64_t ns = duration_get.count();
    return ns;
 }
};


class MFO {  // include instance of Timer as private variable
  Timer t;
public:
  MFO() { t.Start(); cout << "MFO::MFO() CTOR\n"; }
  ~MFO() {
    t.Stop();
    cout << "MFO::~MFO() DTOR: program was active for " << t.nsecs() << " nsecs.\n";
  }
  void operator() (void) {
    cout << "MFO::operator()\n";
    if(!this) { cout << "this is nullptr\n"; return; }
    t.Stop();
    cout << "MFO::operator(): program was active for " << t.nsecs() << " nsecs.\n";
  }
  void printThis (void) { cout << " this:" << (void*)this; }
};

class MFO2 : public Timer { // derive from Timer class
public:
  MFO2() { Start(); cout << "MFO2::MFO2() CTOR, "; printThis(); cout <<"\n"; }
  ~MFO2() {
    Stop();
    cout << "MFO2::~MFO2() DTOR: "; printThis();
    if(this) { cout << " program was active for " << nsecs() << " nsecs."; }
    cout <<"\n";
  }
  void operator() (void) {
    cout << "MFO2::operator():"; printThis();
    if(this) { Stop(); cout << " program was active for " << nsecs() << " nsecs."; }
    cout <<"\n";

  }
  void printThis (void) { cout << " this:" << (void*)this; }
};

// MFO mfo;   // global instance works!
MFO2 mfo;   // global instance works!

int main()
{
  cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<< Start of MAIN.\n";
  // MFO2 mfo;           // error
  // static MFO2 mfo;    // g++ compiles w/non-automatic storage variable warning + executes fine

  cout << "mfo: "; mfo.printThis(); cout << "\n";
  atexit( atexit_namespace::atexitCStyleFunction );

  #if 0
    atexit( [] () { cout << "TEST: atexit calling lambda\n"; } );  // works!
  #endif

  #if 1
    atexit( [mfo] () { mfo(); } ); 
    // g++ warning: capture of variable ‘mfo’ with non-automatic storage duration
    // clang++ error: 'mfo' cannot be captured because it does not have automatic storage duration
  #endif

  #if 0
    void (*fc) (void) = [mfo] () { mfo(); };  // assign lambda to a 'void(*)()' variable
    // g++ warning: capture of variable ‘mfo’ with non-automatic storage duration
    // clang++ error: 'mfo' cannot be captured because it does not have automatic storage duration
    atexit(fc);
  #endif

  cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<< End of MAIN.\n";
}
