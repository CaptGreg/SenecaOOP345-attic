// Multiple ways of calling a function:-
// 1. just call it by the name of function
// 2. call a pointer to the name of function - c stlye
// 3. call a pointer to the name of function - c++ stlye
// 4. function objects or functors:  overload operator() of a class
// 5. lambda
// 6. std::bind
// 7. threads are functions. std::thread
// 8. threads are functions. std::async
// 9. how to code functions that are called with a variable number of arguments

// Functions can have a variable number of arguments.  Think about printf.  How does it work?

#include <cmath>      // M_PI, sin, cos, tan, sqrt, abs, ...
                      // NOTE M_PI is PI=3.1415962..., the M_ means 'machine precision' (best value of PI)
#include <cstdarg>    // vararg
#include <functional> // std::bind + std::function
#include <future>     // std::async returns future
#include <iostream>
#include <iomanip>    // setprecision for printing doubles
#include <cstdlib>    // random
#include <thread>     // std::async
#include <string>

using namespace std;

int main(int argc, char**argv)
{
  double arg;

  // 1. just call the function
  std::cout <<  "sqrt(1000) = " << sqrt(1000) << "\n";

  // 2. C pointer to a function - // 1977 or OOP144
  double (*f)(double arg);  // a pointer to a function that returns a double and takes a double as an argument

  f = sqrt;
  arg = 250000;
  std::cout <<  "sqrt(" << arg << ") = " << f(arg) << "\n"; 

  #define P(FUN,ARG) f=FUN; std::cout <<  "C pointer: " #FUN "(" #ARG ")=" << f(ARG) << "\n"; 
    P(sqrt, 81.)
    P(fabs, -123.)
    P(tan, 45*M_PI/180.)
  #undef P


  // 3. C++ pointer to a function
  std::function<double(double)> g; // a pointer to a function that returns a double and takes a double as an argument
  extern double log10(double arg); // compiler complaining can't figure out function, so prototype it
  g = log10;
  arg = 1000;
  std::cout <<  "log10(" << arg << ") = " << g(arg) << "\n"; 

  #define P(FUN,ARG) extern double FUN(double arg); g=FUN; std::cout << "CPP pointer "  #FUN "(" #ARG ")=" << g(ARG) << "\n"; 
    P(sqrt, 81.)
    P(fabs, -123.)
    P(tan, 45*M_PI/180.)
  #undef P


  // 4. function objects or functors  (overload operator() for a class)
  class MyFunctionObject {
  public:
    int operator() (int arg) { return 2*arg; }
  };
  MyFunctionObject mfo;
  std::cout << "mfo(99) = " << mfo(99) << ", mfo returns 2*arg\n";
  MyFunctionObject mfo2;
  std::cout << "mfo2(150) = " << mfo2(150) << ", mfo returns 2*arg\n";

  class my_function_object  // another example, pass a function to the constructor
  {
    std::function<double(double)> f;  // a C++ style function pointer
  public :
    my_function_object(std::function<double(double)> function) : f(function) {}
    double operator () (double d) const { return f(d); }
  };
  std::cout << "m_f_o returns the the result of calling the constructor supplied function with the argument\n";
  my_function_object  m_f_o(tan);
  std::cout << "constructor m_f_o(tan), call m_f_o(45*M_PI/180) = " << m_f_o(45*M_PI/180) << "\n";

  class my_function_objectCStyle  // same thing but with C-style function pointers
  {
    double (*f) (double);  // a C style (1977) function pointer
  public :
    my_function_objectCStyle(double (*function) (double)) : f(function) {}
    double operator () (double d) const { return f(d); }
  };
  std::cout << "same thing but with C-style function pointers\n";
  my_function_objectCStyle  m_f_oCStyle(tan);
  std::cout << "constructor m_f_oCStyle(tan), call m_f_oCStyle(45*M_PI/180) = " << m_f_oCStyle(45*M_PI/180) << "\n";



  // 5. lambda functions
  auto lambdaTest1   = [] () { };  // a test lambda function, no capture, no args, no return type specified, empty function body

  lambdaTest1();

  auto lambdaTest2   = [] { };  // a test lambda function, no capture, no args, no return type specified, empty function body

  lambdaTest2();

  auto lambdaSimple = [] () { cout << "hello from a simple lambda function, no capture, no args, no return type specified\n"; };

  lambdaSimple();

  auto lambda = [] (int arg) { return 3*arg; };   // no capture [], one arg

  std::cout << "lambda(99)=" << lambda(99) << "\n";

  f  = fabs;
  arg = -123;
  auto lambda1 = [f] (double a) -> double { return f(a); };  // capture [], arg (), and return type ->

  cout << "lambda1 capturing f, f=fabs (" << arg << ")=" << lambda1(arg) << "\n";

  auto evalFunction = [] ( double (*f) (double), double arg) -> double { return 3*(*f)(arg); };

  cout << "evalFunction(f," << arg << ")=" << evalFunction(f,arg) << ", (f=fabs), result=3*f\n";

  // 6. std::bind
  extern double sin(double arg);             // compiler complained bind can't figure out sin
  auto foo = std::bind(sin, 30*M_PI/180.);   // works with our DIY prototype for 'sin', <cmath> should of taken care of this
  cout << "foo=bind of sin of 30 degrees: foo() =" << foo() << "\n";

  // bind act like a define macro, sort of
  #define FOO sin(30*M_PI/180.)
  cout << "'#define FOO sin(30*M_PI/180.)' : FOO =" << FOO << "\n";
  // Why have bind, if a #define macro can do the same thing?
  // Bind objects can be passed as arguments to other functions.
  // #define macros objects cannot be passed as arguments to other functions.
  // The compiler will evaluate the macro code and pass the value as the argument to the other function.

   auto printArg = [] (double arg) { cout << "printArg: arg=" << arg << "\n"; };
   printArg(FOO);

   auto callArg = [] (std::function<double()> arg) { cout << "callArg: function returned=" << arg() << "\n"; };
   callArg(foo);

   // assign 'foo' to another variable
   std::function<double()> fooFuncPtr = foo;
   // use it:
   callArg(fooFuncPtr);

   // (Cannot do that with a macro)

  // 7. threads are functions. std::thread
  // They might be running on another core, if there is another core.
  auto threadFunction = [] (int foo) 
    { 
      std::this_thread::sleep_for( std::chrono::milliseconds ( foo )); 
      std::cout << std::string("Threadfunction slept for ") + to_string(foo) + " ms.\n";
    };

  cout << "launching thread ... \n";
  std::thread t ( threadFunction, 1500); // run function in a separate thread. sleep for 1.5 sec (1500 milliseconds)
  t.join(); // wait for thread to finish
  cout << "thread complete\n";

  cout << " ---- one liner: ";
  std::thread( threadFunction, 1500 ).join();
  cout << "\n";

  cout << "do it again, launch thread but time it ... \n";
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;

  // call thread
  start = std::chrono::high_resolution_clock::now(); 
  t = std::thread( threadFunction, 1500); // run function in a separate thread. sleep for 1.5 sec (1500 milliseconds)
  t.join(); // wait for thread to finish
  stop = std::chrono::high_resolution_clock::now(); 

  cout << "thread complete\n";
  // calculate and print elapsed time
  typedef std::chrono::duration<int,std::micro> microsecs_t ;
  microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
  uint64_t tWait = duration_get.count();
  std::cout << "thread completed after " << tWait << " microsecs or about " 
            << setprecision(2) << tWait * 1e-6 << " seconds\n";

  // create a job table of function pointers, use bind to initialize the table, launch each job as a thread, time everything
    const int NUMTHREADS = 10;
    std::function<void()> jobTable[NUMTHREADS];
    for(int i = 0; i < NUMTHREADS; i++)                    // initialise the job table
      jobTable[i] = std::bind (threadFunction, 150*(i+1));

    std::thread tidTable[NUMTHREADS];
    start = std::chrono::high_resolution_clock::now(); 
    for(int i = 0; i < NUMTHREADS; i++) {                  // initialize the thred id table (launch thread)
      // std::function<void()> job = jobTable[i];
      // tidTable[i] = std::thread( job );
      tidTable[i] = std::thread( jobTable[i] );
    }

    for(int i = 0; i < NUMTHREADS; i++)
      tidTable[i].join();
      
    stop = std::chrono::high_resolution_clock::now(); 
    tWait = duration_get.count();
    std::cout << NUMTHREADS << " threads completed after " << tWait << " microsecs or about " 
              << setprecision(2) << tWait * 1e-6 << " seconds\n";

  // thread return values
    int result;
    auto threadRandom = [] (int* argPtr) { *argPtr = random() % 1000; };
    t = std::thread( threadRandom, &result);
    t.join(); // wait for thread to finish
    cout << "threadRandom returned " << result << "\n";

    // More than one arg? use a pass an address to a structure with the parameters and return value
    struct parms_s{
      int m;
      int x;
      int b;
      int y;
    } parms ={ 1,2,3 };

    auto threadParms = [] (parms_s* p) -> void { p->y = p->m * p->x + p->b; };
    t = std::thread( threadParms, &parms);  // run the thread
    t.join(); // wait for thread to finish
    cout << "threadParms returned " << parms.y << ", y=mx+b, 1*2+3=5\n";


  // 8. threads are functions. std::async - threads - use async to launch, wait for completion, and get value returned from a thread
    auto async = std::async(sin, 30*M_PI/180.);
    cout << "async(sin, 30 degrees) =" << async.get() << "\n";

    async = std::async(std::launch::deferred, sin, 30*M_PI/180.);
    cout << "async(std::launch::deferred, sin, 30 degrees) =" << async.get() << "\n";

    async = std::async(std::launch::async, sin, 30*M_PI/180.);
    cout << "async(std::launch::async, sin, 30 degrees) =" << async.get() << "\n";

  // 9. how to code functions that are called with a variable number of arguments
    void SimpleDIYPrintf(const char *fmt, ...);  // a variable argument function (variadic function)
    SimpleDIYPrintf("Hello from our DIY home-made printf function s: >%s<, c: >%c<, f: >%f<, d: >%d<\n", 
                    "QWERTY", '@', 1.23e10, 42);
}

void SimpleDIYPrintf(const char *fmt, ...)  // a variable argument function (variadic function)
{
  va_list va;           // allocate a va_list variable 'va'
  va_start(va, fmt);    // initialize 'va' to start after 'fmt'

  for(const char *f=fmt; *f; f++) { // process fmt string, one char at a time
    if(*f == '%') { // process format character
      f++;  // want character after %
      double d;
      int    i;
      char   c;
      char  *s;
      if(*f) {
        switch(*f) {
        case 'f': d = va_arg(va, double); std::cout << d; break;
        case 's': s = va_arg(va, char*);  std::cout << s; break;
        case 'c': c = va_arg(va, int);    std::cout << c; break;  // use 'int' to fetch 'char' variable
        case 'd': i = va_arg(va, int);    std::cout << i; break;
        default: std::cout << '%' << *f; break; // don't understand format character, print it along with %
        }
      } else {
        std::cout << '%'; // print the %.  The % was followed by '\0', end-of-string
      }
   } else { // not a format character, just print it
      std::cout << *f;
    }
  }
  va_end(va); // clean up
}
