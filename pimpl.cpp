// Pimpl - pointer to implementation
/////////////////////////////////////////////////
// Uses C++14 features:                        //
// clang++ -Wall -std=c++14 pimpl.cpp -o pimpl //
// or  g++ -Wall -std=c++14 pimpl.cpp -o pimpl //
/////////////////////////////////////////////////

#include <iostream>
#include <memory> // std::unique_ptr

// ------------------------------------------------------------------
// http://herbsutter.com/elements-of-modern-c-style/
// C++11 Pimpl idiom: header file
class widget {
public:
    widget();
    ~widget();              // GB
    // ... (see GotW #100) ...
    void Public_Method();   // GB
private:
    class impl;
    std::unique_ptr<impl> pimpl;
};
 
// implementation file
class widget::impl { 
public:
  int privateData;
};
 
widget::widget() : pimpl{ new impl{ /*...*/ } } { 
  pimpl->privateData=42; 
  std::cout<<"widget::widget(), privateData=" << pimpl->privateData << "\n"; 
  pimpl->privateData++;
}
void widget::Public_Method() { 
  std::cout<<"widget::Public_Method(), privateData=" << pimpl->privateData << "\n"; 
  pimpl->privateData++;
} // GB
widget::~widget() { 
  std::cout<<"widget::~widget(), privateData=" << pimpl->privateData << "\n"; 
} // GB

auto pwscpp11 = std::make_shared<widget>();  // C++11
auto pwucpp14 = std::make_unique<widget>();  // C++14

#if 0
// ------------------------------------------------------------------
// GB syntax errors
// http://stackoverflow.com/questions/17554515/hiding-private-members-of-c-library

// PIMPL - Point to IMPLementaation

// Interface.hpp
#include <memory> // unique_ptr

class Implementation;
class Interface {
    std::unique_ptr<Implementation> pimpl;
    // Implementation* pimpl;  // GB see if raw pointer fixes syntax error, no, it didn't
public:
    Interface() : pimpl(new Implementation()) {}
    //  error: allocation of incomplete type 'Implementation'
    // ~Interface() { delete pimpl; }  // GB need destructor with raw pointer

    void publicMethod();
};

// Interface.cpp
class Implementation {
public:
    void PrivateMember();
};

void Interface::publicMethod() { pimpl->PrivateMember(); }
#endif

// ------------------------------------------------------------------
// http://www.gamedev.net/page/resources/_/technical/general-programming/the-c-pimpl-r1794
// YES, note the /_/ in the URL 
// MyClass.h

class MyClassImp;                    // forward declaration of Pimpl
class MyClass
{
public:
   MyClass ();
   ~MyClass();
   MyClass( const MyClass &rhs );   // undefined for simplicity
   MyClass& operator=( MyClass );
   void  Public_Method();
private:
   // MyClassImp *pimpl;              // the Pimpl, GB compiles fine
   std::unique_ptr<MyClassImp> pimpl; // GB prefer to use unique_ptr
};

// MyClass.cpp
// #include "MyClass.h"
class MyClassImp
{
public:
   MyClassImp();
   ~MyClassImp();
   void   Private_Method()  {}   // dummy private function
   int    private_var;           // a private variable
};
 
MyClassImp::MyClassImp()  { std::cout<<"MyClassImp\n"; }
MyClassImp::~MyClassImp()  { std::cout<<"~MyClassImp\n"; }

MyClass::MyClass()  :  pimpl( new MyClassImp() ) { std::cout<<"MyClass\n"; }
 
MyClass::~MyClass() { std::cout<<"~MyClass\n"; /* delete  pimpl; */ } // GB not needed with unique_ptr
 
void   MyClass::Public_Method()
{
   std::cout<<"MyClass::Public_Method()\n";
   pimpl->Private_Method();      // do some private work
   pimpl->private_var  = 3;
}

// ------------------------------------------------------------------
// http://www.cppsamples.com/common-tasks/pimpl.html
// GB where is base class impl defined?
// foo.h - header file
#include <memory>
class foo
{
public:
  foo();
  ~foo();
  foo(foo&&);
  foo& operator=(foo&&);
private:
  class impl;
  std::unique_ptr<impl> pimpl;
};

// foo.cpp - implementation file
class foo::impl
{
  int internal_data = 0;
public:
  void do_internal_work() { 
    internal_data = 5; 
    std::cout<<"private (hidden) internal function foo::do_internal_work()\n"; 
  }
};

foo::foo() : pimpl{std::make_unique<impl>()} 
        // Note: std::make_unique was introduced in C++14. For C++11, you can roll your own or use make_shared
   { std::cout<<"foo: calling do_internal_work()\n"; pimpl->do_internal_work(); }

// foo::~foo() = default;
foo::~foo() { std::cout<<"~foo\n"; }

foo::foo(foo&&) = default;
foo& foo::operator=(foo&&) = default;

// ------------------------------------------------------------------
// C++14
// http://coliru.stacked-crooked.com/a/36ead4897a83c4fc
class bar
{
  class impl;
  struct impl_deleter
  {
      void operator()( impl * ) const;
  };
  std::unique_ptr<impl,impl_deleter> pimpl;
public:
  bar();
  ~bar(); // GB
};
// bar.cpp - implementation file
class bar::impl
{
  int internal_data = 0;
public:
  void do_internal_work()
  {
    std::cout<<"private (hidden) internal function bar::do_internal_work()\n"; 
    internal_data = 5;
  }
};
void bar::impl_deleter::operator()( impl * ptr ) const
{
  delete ptr;
}

bar::bar() : pimpl{new impl}
{
  std::cout<<"bar: calling do_internal_work()\n";
  pimpl->do_internal_work();
}

bar::~bar() { std::cout<<"~bar\n"; } // GB
// ------------------------------------------------------------------
// http://anthony-arnold.com/2012/04/05/three-fun-cpp-techniques/
// 1. The Pimpl Idiom

// Frankly, if you don’t know about the Pimpl idiom, you’re not a real C++ programmer. Pimpl stands for “Pointer to implementation”, sometimes referred to as the Cheshire Cat. It’s a way to hide the implementation of a class from the user by defining all private members in the class definition (.cpp) file.

// Here is an example:
//car.hpp
class car {
public:
    car();                          //constructor
    ~car();                         //destructor // GB
    car(const car &c);              //copy constructor
    car &operator=(const car &rhs); //copy assignment operator
    car(const car &&c);             //move constructor      
    car &operator=(const car &&rhs);//move assignment operator
    /* Other public methods */
    void start();      // GB
    bool isRunning();    // GB
    void stop();       // GB
 
private:
    class under_the_hood; //Not defined here
    std::shared_ptr<under_the_hood> pImpl; //POINTER TO IMPLEMENTATION
};
//car.cpp
 
class car::under_the_hood {
public:
    /* Implement car stuff here */
    bool  bEngineOn;   // GB
};
 
car::car() : pImpl(std::make_shared<under_the_hood>()) { std::cout<<"car\n"; pImpl->bEngineOn = false; }
car::~car() { std::cout<<"~car\n"; } // GB
void car::start() { pImpl->bEngineOn = true; std::cout<<"CMD: start car\n"; } // GB
void car::stop() { pImpl->bEngineOn = false; std::cout<<"CMD: stop car\n"; } // GB
bool car::isRunning() { return pImpl->bEngineOn; } // GB
 
car::car(const car &c) : pImpl(std::make_shared<under_the_hood>(*c.pImpl)) {}
 
car &car::operator=(const car &rhs) {
    pImpl = std::make_shared<under_the_hood>(*rhs.pImpl);
    return *this;
}
 
car::car(const car &&c) : pImpl(c.pImpl) { }
 
car &car::operator=(const car &&rhs) {
    pImpl = rhs.pImpl;
    return *this;
}

// This idiom has quite a few advantages, especially when authoring libraries. Firstly, you can maintain secrecy regarding your implementation; the client gets the public interface to your class and can use it freely without being able to see the gritty details of how it works “under the hood”.

// Secondly, the client gets a compile-time performance increase by not having to parse the private declarations of your class; it is all safely hidden in the implementation file.

// Another advantage of the Pimpl idiom is that it allows you to use third-party, header-only libraries in your implementation without requiring that the client has access to these same libraries. A great example is the use of Boost. If you want to use any of the Boost header-only libraries, like Boost.Signals2 or Boost.Geometry, then great! You should use Boost where it’s appropriate. But what if you don’t want your users to require Boost in order to use your library? Easy; use the Pimpl idiom and hide all of your private Signals or Polygons or what have you in the implementation file. Your user need never know. Well, actually they do need to know if you’re using open source libraries, but you get my point.

// ------------------------------------------------------------------
// https://msdn.microsoft.com/en-us/library/hh438477.aspx

// Pimpl For Compile-Time Encapsulation (Modern C++)
// Visual Studio 2015
// Other Versions

// The pimpl idiom is a modern C++ technique to hide implementation, to minimize coupling, and to separate interfaces. Pimpl is short for "pointer to implementation." You may already be familiar with the concept but know it by other names like Cheshire Cat or Compiler Firewall idiom.
// Why use pimpl?

// Here's how the pimpl idiom can improve the software development lifecycle:
// 
//     Minimization of compilation dependencies.
// 
//     Separation of interface and implementation.
// 
//     Portability.
// 
// Pimpl header
// C++

// my_class.h
class my_class {
public:
   my_class();   // GB added this ctor specification to fix the error down below
   ~my_class();  // GB
   //  ... all public and protected stuff goes here ...
   void print();
private:
   class impl; std::unique_ptr<impl> pimpl; // opaque type here
};

// The pimpl idiom avoids rebuild cascades and brittle object layouts. It's well suited for (transitively) popular types.
// Pimpl implementation

// Define the impl class in the .cpp file.
// C++

// my_class.cpp
class my_class::impl {  // defined privately here

  // ... all private data and functions: all of these
  //     can now change without recompiling callers ...
public:        // GB
  int count;   // GB
};
// GB above my_class::my_class(); ctor specification fixes this error
// error: definition of implicitly declared default constructor
// my_class::my_class(): pimpl( new impl )
//           ^

my_class::my_class(): pimpl( new impl )
{
  // ... set impl values ... 
  pimpl->count = 1; // GB
}
void my_class::print() { std::cout << "count=" << pimpl->count << "\n"; }
my_class::~my_class() { std::cout << "~my_class\n"; }

// ------------------------------------------------------------------


int main()
{
  std::cout << "\nwidget pwscpp11\n";
  pwscpp11->Public_Method();
  pwscpp11->Public_Method();
  std::cout << "\nwidget pwscpp14\n";
  pwucpp14->Public_Method();
  pwucpp14->Public_Method();
  pwucpp14->Public_Method();

  std::cout << "\nwidget w\n";
  widget w;
  w.Public_Method();

  std::cout << "\nMyClass mc\n";
  MyClass mc;
  mc.Public_Method();

  std::cout << "\nfoo fighter\n";
  foo fighter;
  // fighter.do_internal_work();   // <-- hidden function, cannot call from instance of foo

  std::cout << "\nbar stool mc\n";
  bar stool;

  std::cout << "\ncar wreck\n";
  car wreck;
  std::cout << "car is running: " << wreck.isRunning() << "\n";
  wreck.start();
  std::cout << "car is running: " << wreck.isRunning() << "\n";
  wreck.stop();
  std::cout << "car is running: " << wreck.isRunning() << "\n";

  std::cout << "\nmy_class room\n";
  my_class room;
  room.print();

  std::cout << "\nend-of-main\n\n";
}
