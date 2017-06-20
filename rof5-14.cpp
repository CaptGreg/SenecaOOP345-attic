#include <exception>
#include <iostream>
#include <iomanip>      // setprecision(9) for printing nsec to 9 decimal places
#include <string>
#include <chrono>      //  timers

using namespace std;

#ifdef __WIN
// windows needs max - what header? write our own
template <typename T>
T max(T a, T b) { return a < b? b: a; }
#endif


bool verbose = false;
// bool verbose = true;

class X {
       char*   b = nullptr;   // c++14 initialize buffer
       size_t  s = 0;         // c++14 initialize size
static int     instanceCount;
       int     instanceSerialNumber;
public:
static int     instancesActive;
static size_t  memActive;
static size_t  maxMem;
public:
  void print() const { cout << "#" << instanceSerialNumber 
                            // <<  ", this=" << (void*) this 
                            << ", s=" << 1e0*s 
                            << ", b=" << (void*) b << "\n"; }
  void print(const char* label)   const { cout << label << ": "; print(); }
  void print(const string& label) const { cout << label << ": "; print(); }

  X()  // C++98 constructor 
    : instanceSerialNumber(instanceCount++)
  {
    instancesActive++;
    if(verbose) print( "default ctor" ); 
  }

  X(size_t size) // C++98 constructor 
    : s(size), b(new char[size]),  instanceSerialNumber(instanceCount++)
  {
    instancesActive++;
    memActive += size;
    maxMem = max(maxMem, memActive);
    if(verbose) print( "ctor" );
  }

  ~X()  // C++98 destructor
  { 
    instancesActive--;
    memActive -= s;
    if(verbose) print( "dtor" );
    delete [] b; 
  }

  X(const X& rhs)  // C++98 copy constructor
    : instanceSerialNumber(instanceCount++)
  { 
    instancesActive++;
    if(verbose) rhs.print( "cp ctor: 'rhs' in" );
    *this = rhs; 
    if(verbose)     print( "cp ctor: 'this' OUT" );
  }

  X& operator= (const X& rhs) // C++98 copy assignment operator
  {
    if(verbose)     print( "cp = op: 'this' in" );
    if(verbose) rhs.print( "cp = op: 'rhs'  in" );
    if(this != &rhs) {
      delete [] b;
      memActive -= s;

      s = rhs.s;
      b = new char[s];
      memActive += s;
      maxMem = max(maxMem, memActive);
      for(size_t i = 0; i < s; i++) {
        b[i] = rhs.b[i];       // deep copy
      }
    } else {
      cout << "X cp = operator --- self assignment\n";
    }
    if(verbose) print( "cp = op: 'this' OUT" );
    return *this;
  }


  X(X&& rhs)   // C++11 move constructor
    : instanceSerialNumber(instanceCount++)
  {
    instancesActive++;
    if(verbose) rhs.print( "mv ctor: 'rhs'  in" );
  #if 1
    s = rhs.s;        // steal rhs 'brains'
    b = rhs.b;

    rhs.s = 0;        // turn rhs into a 'zombie'
    rhs.b = nullptr;
  #else
    cout << "mv ctor calling mv assignment operator\n";
    *this = move(rhs);
  #endif
    if(verbose) print( "mv ctor: 'this' OUT" );
  }

  X&& operator= (X&& rhs) // C++11 move assignment operator
  {
    if(verbose)     print( "mv = op: 'this' in" );
    if(verbose) rhs.print( "mv = op: 'rhs'  in" );
    if(this != &rhs) {
      delete [] b;
      memActive -= s;

      // steal 'brains' from rhs
      s = rhs.s;
      b = rhs.b;

      // convert rhs into a 'zombie'
      rhs.s = 0;
      rhs.b = nullptr;
    } else {
      cout << "X mv = operator --- self assignmant\n";
    }
    if(verbose) print( "mv = op: 'this' OUT" );
    return move(*this);
  }


  X& operator+= (const X& rhs) // C++98 += operator
  {
    if(verbose)     print( "+= op: 'this' in" );
    if(verbose) rhs.print( "+= op: 'rhs'  in" );
    char* bResult = new char[s + rhs.s];
    memActive += s + rhs.s;
    maxMem = max(maxMem, memActive);


    if(b) {
      for(size_t i = 0; i < s; i++)
        bResult[i] = b[i];       // deep copy
    }

    if(rhs.b) {
      for(size_t i = 0; i < rhs.s; i++)
        bResult[s+i] = rhs.b[i];       // deep copy
    }

    delete [] b;
    memActive -= s;

    b  = bResult;
    s += rhs.s;

    if(verbose) print( "+= op: 'this' OUT" );

    return *this;
  }

  X operator+ (const X& rhs) // C++98 + operator
  {
    if(verbose)     print( "+ op: 'this' in" );
    if(verbose) rhs.print( "+ op: 'rhs'  in" );

    return X(*this) += rhs; // one line
  }
};

int       X::instanceCount   = 0;
int       X::instancesActive = 0;
size_t    X::memActive       = 0;
size_t    X::maxMem          = 0;

class Timer { // use C++11 std::chrono features to create a stop-watch timer class
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop () { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t nsecs() {
    typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    uint64_t ns = duration_get.count();
    return ns;
  }
  uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t us = duration_get.count();
    return us;
  }
  uint64_t msecs() {
    typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    uint64_t ms = duration_get.count();
    return ms;
  }
 };


int app(int argc, char* argv[])
{
  Timer timer;

  // motivation
  int i = 1, j = 2, k = i + j;
  // i + j is a r-value that dies immediately after the k = i + j statement executes

  // the same thing using a class that supports move semantics
  // X I(1), J(2), K = I + J;
  // I + J is a r-value that is destroyed immediately after the k = i + j statement executes
  // with move ctor, I + J created and mv ctor called for K
  // without move ctor, I + J created and cp ctor called, complete with deep copy, for K

 // out << fixed << setprecision(9);  // print nanoseconds to 9 decimal places (requires <iomanip>)
  cout.precision(9);  // print nanoseconds to 9 decimal places (requires <iomanip>)

// Windows likes macros on one line --- yuk!
#define RUN(CMD) cout << #CMD " START\n"; timer.Start(); CMD; timer.Stop(); cout <<  #CMD " FINISH " << 1e-9*timer.nsecs() << " sec\n\n";
// cout <<  #CMD " FINISH " + to_string(1e-9*timer.nsecs()) + " sec\n\n";


// to stress test a program, use large numbers.
// small number prove nothing
#ifdef __WIN
  size_t baseSize = 81.25 * 1000 * 1000;       // runs fine in Windows with this size
#else
  size_t baseSize = 200 * 1000 * 1000;         // too big for windows
  // baseSize   max Mem
  // 100 MB       2.1 GB
  // 200 MB       4.2 GB
  // 300 MB       6.3 GB
  // 300 MB       8.4 GB
  // 500 MB      10.5 GB     
  // 600 MB      12.6 GB   
  // 700 MB      14.7 GB      
  // 800 MB      16.8 GB      
#endif

  RUN(X  I(1 * baseSize))
  // timer.Start();
  // X  I(100000000);
  // timer.Stop();
  // I.print(string("I ") + to_string(1e-9*timer.nsecs()) + " sec");

  RUN(X  J(2 * baseSize))
  // timer.Start();
  // X  J(200000000);
  // timer.Stop();
  // J.print(string("J ") + to_string(1e-9*timer.nsecs()) + " sec");

// Windows likes macros on one line --- yuk!
#define RUN_PRINT(CMD,INSTANCE) cout << #CMD " START\n"; timer.Start(); CMD; timer.Stop(); INSTANCE.print(string(#CMD " ") + "FINISH " + to_string(1e-9*timer.nsecs()) + " sec"); cout<<"\n";


  RUN_PRINT( X  K = I + J, K)
  // timer.Start();
  // X  K = I + J;
  // timer.Stop();
  // K.print(string("K = I + J ") + to_string(1e-9*timer.nsecs()) + " sec");
  // cout << "\n";

  RUN_PRINT( X  L = K,  L)
  // timer.Start();
  // X L = K;
  // timer.Stop();
  // L.print(string("X L = K ") + to_string(1e-9*timer.nsecs()) + " sec");
  // cout << "\n";

  RUN_PRINT( X  M( I + J ), M)
  // timer.Start();
  // X  M( I + J );       // C++11 move constructor
  // timer.Stop();
  // M.print(string("X  M( I + J ) ") + to_string(1e-9*timer.nsecs()) + " sec");
  // cout << "\n";

  RUN_PRINT( X  N( move(I + J) ), N)       // C++11 move constructor
  // timer.Start();
  // X  N( move(I + J) );       // C++11 move constructor
  // timer.Stop();
  // N.print(string("X  N( move(I + J) ) ") + to_string(1e-9*timer.nsecs()) + " sec");
  // cout << "\n";

  RUN_PRINT(X  I2( move(I) ), I2);  // C++11 move ctor
  // timer.Start();
  // X  I2( move(I) );  // C++11 move ctor
  // timer.Stop();
  // I2.print(string("X  I2( move(I) ) ") + to_string(1e-9*timer.nsecs()) + " sec");
  // cout << "\n";

  RUN_PRINT( I = move(I2), I)      // C++11 move assignment operator
  // timer.Start();
  // I = move(I2);      // C++11 move assignment operator
  // timer.Stop();
  // I.print(string("I = move(I2) ") + to_string(1e-9*timer.nsecs()) + " sec");
  // cout << "\n";


  // special case
  cout << "(I + J) " " START\n";
  timer.Start();
  (I + J).print("(I + J)");
  timer.Stop();
  cout <<  "(I + J)" " FINISH " + to_string(1e-9*timer.nsecs()) + " sec\n\n";

  cout << "\n";
  cout << "   active instances = " << X::instancesActive  << "\n";
  cout << "   active  memory   = " << 1e0*X::memActive  << "\n";
  cout << "   maximum memory   = " << 1e0*X::maxMem  << "\n";
  cout << "   base size        = " << 1e0*baseSize << "\n";

  cout << "**********APP RETURNING**********\n";
  return 0;
}

int main(int argc, char* argv[])
{
  int ret = 0; // assume no errors
  try {
    ret = app(argc, argv);
    cout << "   active instances = " << X::instancesActive  << "\n";
    cout << "   active memory    = " << 1e0*X::memActive;
    if(X::memActive == 0) {
      cout << " <-- no memory leaks";
    } else {
      cout << " <-- memory leaks";
    }
    cout << "\n";
    cout << "   maximum memory   = " << 1e0*X::maxMem  << "\n";

  } catch(exception& e) {  // 'new' throws a std::exception::bad_alloc if allocate memory fails
    cerr << "'" << argv[0] << "' threw '" << e.what() << "'\n";
    ret =  -1;  // errors
  }

  if(ret) {
    cerr << "**********ABNORMAL TERMINATION**********\n";
  } else {
    cout << "**********NORMAL TERMINATION**********\n";
  }

  cout << "This program is " << 8 * sizeof(void*) << "-bit code.\n";
  return ret;
}
