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
#ifdef __MSVC__   // one of the many, many, many Microsoft visual studio c compilers
  cout << R"msc(
  Everytime I go near Windows, Windows bites me. 
  
  Windows programs execute with a tiny stack.

  This code generates a class with data elements that is s million bytes.
  For encapsulation, these classes are created local to a function, that is, on the stack.

  This causes a segfault if this code is run under windows. :-( :-( :-(

  One can specify the stack size for a thread under windows.  See 
  http://msdn.microsoft.com/en-us/library/windows/desktop/ms686774%28v=vs.85%29.aspx

  One can request Windows to run a program with a realistic stack size.
 
  Here's how:
    1. if using the MSC compiler try the /F <size> compiler option
           to set the stack to 10 MB:
           CL ctor_throw.cpp /F 10000000
    2. set the stack size at run time in the .exe binary file using editbin.exe

    http://www.cs.nyu.edu/exact/core/doc/stackOverflow.txt
    
    Stack Overflow Problems
    =======================
    This file gives some hints on addressing this problem on different platforms.
    Windows (during compilation):
    =============================
    	1. Select "Project->Setting".
    	2. Select "Link" page.
    	3. Select "Category" to "Output".
    	4. Type your preferred stack size in "Reserve:" field under "Stack 
    	   allocations". eg, 32768 in decimal or 0x20000 in hexadecimal.
    
    Windows (to modify the executable file):
    =======================================
    There are two programs included in Microsoft Visual Studio, "dumpbin.exe"
    and "editbin.exe".  
    
    Run "dumpbin /headers executable_file", to see the "size of stack reserve" information 
    in the "optional header values".  
    
    Run "editbin /STACK:size" to change the default stack size.

    I like windows.  One can charge a lot of money to make things work in a windows environment.

    Hope this helps.  Greg
  
  )msc";
  #endif

  class X {
  uint8_t buf1MB [1000*1000];
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
