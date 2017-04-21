// QUESTION?  Is there a way to exit a program and have the destructors called?
// experiment with exception methods from http://www.cplusplus.com/reference/exception/

// The only way appears to be to allocate global classes.  
// The destructors are called after 'exit()' calls, but are not called after 'terminate' or 'abort'
// Smart pointers do not help.

// However, if the 'abort (SIGABRT) ' and 'terminate' handlers call 'exit(...)', the global class destructors are called.

#include <iostream>
#include <string>
#include <csignal>     // SIGABRT + signal()
#include <exception>   // set_terminate, set_unexpected, make_exception_ptr
#include <stdexcept>   // std::logic_error
#include <cstdlib>     // abort, on_exit               <-- compiles fine without this header
#include <memory>      // unique_ptr

#ifdef __unix
  #include <unistd.h> // _exit
#endif

using namespace std;

class X {
  string name;
public:
  X () : name("?") { cout << "ctor X()\n"; }
  X (string s) : name(s) { cout << "ctor X(" << name << ")\n"; }
  ~X() { cout << "dtor ~X() on " << name << "\n"; }
};

X xGlobal("**** xGlobal ****"); // The dtor is called on normal exits and 'exit()' 
                                // The dtor is not called on 'terminate()' or 'abort()' calls.
                                // However, if 'terminate' or 'abort' handlers call 'exit', the dtor is called

int main(int argc,char*argv[])
{
  set_terminate(   []           { cout << "set_terminate handler calling exit(97)\n"; exit(97); } );
  set_unexpected(  []           { cout << "set_unexpected handler called\n"; } );
  signal( SIGABRT, [] (int sig) { cout << "caught signal " << sig << " (SIGABRT) calling exit(98)\n"; exit(98); });
  atexit(          []           { cout << "atexit handler calling exit(99)\n"; exit(99); });
  auto exceptionPtr = std::make_exception_ptr(std::logic_error("logic_error"));

#ifdef __unix
  // on_exit prototyped in cstdlib, so might be available in non-unix environments.
  on_exit( [](int code, void*p) { cout << "on_exit called with exit code " << code << "\n"; } , nullptr);
#endif

  X x("x");
  X* rp = new X("raw pointer");
  unique_ptr<X> usp(new X("unique smart pointer"));

  // if that is all 
  // prints (as expected)
  //   ctor X(**** xGlobal ****)
  //   ctor X(x)
  //   ctor X(raw pointer)
  //   ctor X(unique smart pointer)
  //   dtor ~X() on raw pointer
  //   dtor ~X() on unique smart pointer
  //   dtor ~X() on x
  //   on_exit called with exit code 0
  //   atexit handler calling exit(99)
  //   dtor ~X() on **** xGlobal ****


  // cout << "calling exit(3):\n"; exit(3);  // OOP345 
  // NOTE: ~X(x) not called
  // prints
  //   ctor X(**** xGlobal ****)
  //   ctor X(x)
  //   ctor X(raw pointer)
  //   ctor X(unique smart pointer)
  //   calling exit(3):
  //   on_exit called with exit code 3
  //   atexit handler calling exit(99)
  //   dtor ~X() on **** xGlobal ****


#ifdef __unix
  // cout << "calling _exit(4):\n"; _exit(4); // The function _Exit() is equivalent to _exit().
  // exit_group - exit all threads in a process
  //      void exit_group(int status);
  // This system call is equivalent to exit(2) except that it terminates not only the calling 
  // thread, but  all  threads  in  the  calling  process's thread group.

  // NOTE: ~X(x) not called
  // prints
  //   ctor X(**** xGlobal ****)
  //   ctor X(x)
  //   ctor X(raw pointer)
  //   ctor X(unique smart pointer)
  //   calling _exit(4):
#endif

  // cout << "calling terminate:\n"; terminate();  // OOP345  
  // By default, the terminate handler calls abort. But this behavior can be redefined by calling set_terminate.
  // NOTE: ~X(x) not called
  // prints
  //   ctor X(**** xGlobal ****)
  //   ctor X(x)
  //   ctor X(raw pointer)
  //   ctor X(unique smart pointer)
  //   calling terminate:
  //   set_terminate handler calling exit(97)
  //   on_exit called with exit code 97
  //   atexit handler calling exit(99)
  //   dtor ~X() on **** xGlobal ****

  // cout << "calling abort:\n"; abort();  // OOP345 // generates a SIGABRT signal
  // NOTE:  ~X(x) not called
  // prints
  //   ctor X(**** xGlobal ****)
  //   ctor X(x)
  //   ctor X(raw pointer)
  //   ctor X(unique smart pointer)
  //   calling abort:
  //   caught signal 6 (SIGABRT) calling exit(98)
  //   on_exit called with exit code 98
  //   atexit handler calling exit(99)
  //   dtor ~X() on **** xGlobal ****

  delete rp;
  return 0;
}
