// OOP345 FastForward.cpp - sumarize entire course in one program
// Author Greg Blair
// Date   January 11, 2015
// Date   May, 2015, updated some things, added composition, association, aggregation examples.

#ifdef __GNUC__
  // turn off most compiler warnings
  // #pragma GCC diagnostic warning "-w"
  // turn off 'variable not used' compiler warnings.
  #pragma GCC diagnostic ignored "-Wunused-variable"
#endif
#ifdef __clang__
  // turn off CLANG++ 'unused private field' compiler warnings.
  #pragma GCC diagnostic ignored "-Wunused-private-field"
#endif

#include <algorithm> // accumulate, inner_product, ...
#include <cmath>     // sqrt, abs, fmod, trig functions, ...
#include <cstdarg>   // vararg
#include <iostream>  // cout
#include <future>    // future, async, ...
#include <iomanip>   // setw,left,right for cout
#include <list>      // STL Container
#include <memory.h>  // memcpy used for deep copies
#include <queue>     // STL Container
#include <string>    // C++ strings
#include <thread>    // thread, join, thread ids, ..
#include <vector>    // STL Container
#include <csignal>   // trap segmentation fault

// using namespace std;
// week01
// namespaces can only be defined in global or namespace scope

// week1a namespace in same file (this file):
namespace myNamespaceA {  // create a namespace
  int global;
  class X {
    int x;
  public:
    X() : x(20) {global = 2.5 * x;}
    void print(std::string s) { std::cout << "week1a " << s << " x=" << x << ", global=" << global << "\n"; }
  };
}

// same thing again, but create separate .h and .cpp files for global and class X

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// week1b.h header file for week1b + class X
namespace myNamespaceB {  // create a namespace
  extern int global;
  class X {
    int x;
  public:
    X();
    void print(std::string s);
  };
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// week1b.cpp header file for week1b + class X
namespace myNamespaceB {  // create a namespace
  int global;
  X::X() : x(10) {global = 1.5 * x;}
  void X::print(std::string s) { std::cout << "week1b " << s << " x=" << x << ", global=" << global << "\n"; }
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void NamespaceCompilation()
{
  // namespace and compile multiple source files into a run-time program

  myNamespaceA::X xa;
  xa.print("xa");

  myNamespaceB::X xb;
  xb.print("xb");

  using namespace myNamespaceA;
  X xx;
  xx.print("xx");
  // once you use a namespace with a 'using namespace foo;' statement
  // there is no undoing it.  There is no way to start using a different 
  // namespace with a name conflict with the name you want to select with 
  // the new namespace.
  // EG.
  using namespace myNamespaceB;  // no syntax error
  // 'class X xInstance;' causes a syntax error because the compiler cannot 
  // sort out which class X to use.
  
} // NamespaceCompilation

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FundamentalTypes()
{
  // fundamental types
  bool          flag                 = true;
  char          broiled              = 'a';
  int           integer              = ~0;
  short         pants                = 27;
  signed        contract             = 19;
  unsigned      cheque               = 1u;
  long          johns                = 67L;
  long long     longLongJohns        = 670LL;
  float         downTheRiver         = 19.2f;
  double        trouble              = 3.14159627;
  long double   bigDoubleTrouble     = 3.14159627;


  uint8_t       u8  = 1 << (8-1);
  uint16_t      u16 = 1 << (16-1);
  uint32_t      u32 = 1 << (32-1);
  uint64_t      u64 = 1uLL << (64-1); // or u64 = 1; u64 <<= 63;  
  int8_t        i8  = 1 << (8-1);
  int16_t       i16 = 1 << (16-1);
  int32_t       i32 = 1 << (32-1);
  int64_t       i64 = 1LL << (64-1);

  // #define Macro(parameter)  #parameter is the 'stringify' operator: expand parameter as "parameter"
  #define P(x) std::cout << "var " #x " " << 8*sizeof(x) << "-bit MSB value=" << x << "\n";
  P(u8)   std::cout << " - tried to print 128 as a character, which is a non-printing character.\n";
  P(u16)
  P(u32)
  P(u64)
  P(i8)   std::cout << " - tried to print 128 as a character, which is a non-printing character.\n";
  P(i16)
  P(i32)
  P(i64)
  #undef P

  std::cout << "Fundamental types - c-code OOP144\n";
  #define M(var)  std::cout << std::setw(22) << std::left << "sizeof(" #var ") = " << std::setw(2) << std::right << sizeof(var) << " bytes, " << std::setw(3) << 8*sizeof(var)<< "-bits\n";

  M(void*)
  M(size_t)
  M(int)
  M(char)
  M(long)
  M(long long)
  M(float)
  M(double)
  M(long double)
  M(short)
  M(bool)
  M(size_t)
  M(uint8_t)
  M(int8_t)
  M(uint16_t)
  M(int16_t)
  M(uint32_t)
  M(int32_t)
  M(uint64_t)
  M(int64_t)
  #undef M
} // FundamentalTypes

// function templates to print l-value or r-value
template<typename T>
void print(std::string s, const T&  x) { std::cout << s << ", value=" << x << " is a l-value\n"; }
template<typename T>
void print(std::string s, const T&& x) { std::cout << s << ", value=" << x << " is a r-value\n"; }

void CompoundTypes()
{
  std::cout << "Compound types:\n";
  std::cout << "pointers c-code OOP144\n";
  std::cout << "references(lvalues,rvalues) - C++11 OOP345!\n";

  std::cout << "l-values = r-values,    l=left of equal sign, r=right of equal sign\n";
  std::cout << "eg j = 27  j is l-value, 27 is r-value\n";
  std::cout << "eg j = 6+7  j is l-value, 6+7, or 13 is r-value\n";
  std::cout << "r-values, eg 6+7, are about to die, or more formally\n";
  std::cout << "l-values refer to an accessible region of memory\n";
  std::cout << "r-values refer to an object near the end of its lifetime, a temporary, a value not associated with a an object\n";

  std::cout << "std library\n";
  std::cout << "std::ref() - returns an l-value reference to its argument\n";
  std::cout << "std::move() - returns an r-value reference to its argument\n";

  std::cout << "enumerations - c-code OOP144\n";
  enum TrafficLightStates { GREEN, YELLOW, RED, FLASHING_GREEN};
  std::cout << "arrays - c-code OOP144\n";
  std::cout << "classes - C++ OOP244\n";
  std::cout << "structs - c-code OOP144, struct with member functions C++ OOP244\n";
  std::cout << "bit-fields - c-code OOP144\n";
  struct bits_s {
    uint64_t bit0     : 1; // least significant bit, or LSB
    uint64_t bit1     : 1;
    uint64_t bit2and3 : 2;
    uint64_t filler   : 59;
    uint64_t MSB      : 1; // most significate bit, or MSB
  } bits;

  std::cout << "unions - c-code OOP144\n";
  union myUnion_u {
    float    Float;
    uint32_t UInt32;
    int32_t  Int32;
    uint16_t UInt16[2];
    uint8_t  UInt8[4];
  } myUnion;

  myUnion.Float = -1.5;
  std::cout << "Unions Float,UInt32,Int32, UIntr16[0], UInt16[1],UInt8[0],UInt8[1],UInt8[2], UInt8[3]="
            << myUnion.Float<<","
            << myUnion.UInt32<<","
            << myUnion.Int32<<","
            << myUnion.UInt16[0]<<","<< myUnion.UInt16[1]<<","
            << (unsigned)myUnion.UInt8[0]<<","<<(unsigned)myUnion.UInt8[1]<<","<<(unsigned)myUnion.UInt8[2]<<","<< (unsigned)myUnion.UInt8[3]
            << "\n";

  myUnion.UInt8[0] = 192;
  myUnion.UInt8[1] = 168;
  myUnion.UInt8[2] = 2;
  myUnion.UInt8[3] = 101;
  std::cout << "Unions Float,UInt32,Int32, UIntr16[0], UInt16[1],UInt8[0],UInt8[1],UInt8[2], UInt8[3]="
            << myUnion.Float<<","
            << myUnion.UInt32<<","
            << myUnion.Int32<<","
            << myUnion.UInt16[0]<<","<< myUnion.UInt16[1]<<","
            << (unsigned)myUnion.UInt8[0]<<","<<(unsigned)myUnion.UInt8[1]<<","<<(unsigned)myUnion.UInt8[2]<<","<< (unsigned)myUnion.UInt8[3]
            << "\n";

  myUnion_u abc;
  abc.Int32 = -1;

  std::cout << "week 03 compound type\n";
  std::cout << "Move-constructor, Move-Assignment Operator - C++11 code OOP345!\n";
  class LRValue {
  public:
    void print(std::string s, const char*&  x)         { std::cout << s << ", value=" << x << " is a l-value\n"; }
    void print(std::string s, const char*&&  x)        { std::cout << s << ", value=" << x << " is a r-value\n"; }
    // void print(std::string s, const int   x)           { std::cout << s << ", value=" << x << " is a call-by-value parameter\n"; }
    // confuses compiler - syntax error
    void print(std::string s, const int&  x)           { std::cout << s << ", value=" << x << " is a l-value\n"; }
    void print(std::string s, const int&& x)           { std::cout << s << ", value=" << x << " is a r-value\n"; }
    void operator()(std::string s, const int&  x)      { std::cout << s << ", value=" << x << " is a l-value\n"; }
    void operator()(std::string s, const int&& x)      { std::cout << s << ", value=" << x << " is a r-value\n"; }
  };
  LRValue lrv;
  int x = 99;
  
  // print out l or r value with the class print method
  lrv.print("x: int x = 99;", x);
  lrv.print("9", 9);
  lrv.print("9+6", 9+6);

  {
    const int constInt = 99;
    lrv.print("constInt", constInt);
    lrv.print("\"xyz\"", "xyz" );
  }

  // now use function template print
  {
    const int constInt = 99;
    print("function template: const int constInt = 99;", constInt);
    print("function template: \"xyz\"", "xyz" );
  }

  // do the same thing with a macro
  #define P(param) lrv.print(#param, param);
  P(x)
  P(9)
  P(9+6)
  #undef P

  // print out l or r value with the operator() method
  lrv("x", x);
  lrv("9", 9);
  lrv("9+6", 9+6);

  // do the same thing with a macro
  #define P(param) lrv(#param, param);
  P(x);
  P(9);
  P(9+6);
  #undef P

  class MoveClass { // rule of three C++ OOP244    rule of five C++11 OOP345
    int *b;
    const int size = 1000000;
  public: 
    MoveClass() // constructor OOP244
      { 
        b = new int[size]; 
        std::cout << "ctor (allocate memory) this=" << (void*)this << ", b=" << (void*) b << "\n";
      }
    MoveClass(const MoveClass& rhs)  // copy constructor (deep copy of data) OOP244
      { 
        b = new int[size]; memcpy(b,rhs.b,sizeof(*b)*size); 
        std::cout << "copy ctor (deep copy) this=" << (void*)this << ", b=" << (void*) b << "\n";
      }
    MoveClass& operator=(const MoveClass& rhs)  // assignment operator (deep copy of data) OOP244
      { 
        if(this != &rhs) { if(!b) b = new int[size]; memcpy(b,rhs.b,sizeof(*b)*size); }
        std::cout << "assignment operator (deep copy) this=" << (void*)this << ", b=" << (void*) b << "\n";
        return *this; 
      }
    ~MoveClass() // destructor OOP244
      { 
        std::cout << "dtor this=" << (void*)this << ", b=" << (void*) b << "\n";
        if(b) delete [] b;
      }
  
    MoveClass(MoveClass&& rhs)  // move constructor   (no copy, move pointer) C++11
      { 
        b = rhs.b; rhs.b = nullptr; 
        std::cout << "assignment operator (move pointers) this=" << (void*)this << ", b=" << (void*) b << "\n";
      }
  
    MoveClass& operator=(MoveClass&& rhs)  // move assignment operator   (no copy, move pointer) C++11
      { 
        if(this != &rhs) 
          {if(b) delete [] b; b = rhs.b; rhs.b = nullptr;} 
        std::cout << "move assignment operator (move pointers) this=" << (void*)this << ", b=" << (void*) b << "\n";
        return *this; 
      }
  };

  MoveClass mc1, mc2;
  mc1 = mc2;                        // assignment operator         C++ OOP244
  mc2 = std::move(mc1);             // move assignment operator    C++11
  MoveClass mc3(mc2);               // copy constructor            C++ OOP244
  MoveClass mc4(std::move(mc2));    // move constructor            C++11

} // CompoundTypes

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Inheritance()
{
  class B {
    uint8_t b[1000];
  public:
    void print() { std::cout << "base class B size=" << sizeof(*this) << "\n"; }
  };
  class D : public B {
    uint8_t b[10000];
  public:
    void print() { std::cout << "derived class D size=" << sizeof(*this) << "\n"; }
  };
  B b;
  b.print();
  D d;
  d.print();

  class D1 : public B {
    uint8_t b[10000];
  public:
    void print() { std::cout << "derived class D1 size=" << sizeof(*this) << "\n"; }
  };
  class D2 : public B {
    uint8_t b[100000];
  public:
    void print() { std::cout << "derived class D2 size=" << sizeof(*this) << "\n"; }
  };
  class Diamond : public D1, public D2 {
    uint8_t b[1000000];
  public:
    void print() { std::cout << "derived class Diamond size=" << sizeof(*this) << "\n"; }
  };

  std::cout << "\n";
  D1 d1;
  d1.print();
  D2 d2;
  d2.print();
  Diamond diamond;
  diamond.print();

  class D1virt : public virtual B {
    uint8_t b[10000];
  public:
    void print() { std::cout << "derived class D1virt size=" << sizeof(*this) << "\n"; }
  };
  class D2virt : public virtual B {
    uint8_t b[100000];
  public:
    void print() { std::cout << "derived class D2virt size=" << sizeof(*this) << "\n"; }
  };
  class Diamondvirt : public D1virt, public D2virt {
    uint8_t b[1000000];
  public:
    void print() { std::cout << "derived class Diamondvirt size=" << sizeof(*this) << "\n"; }
  };

  std::cout << "\n";
  D1virt d1virt;
  d1virt.print();
  D2virt d2virt;
  d2virt.print();
  Diamondvirt diamondvirt;
  diamondvirt.print();
} // Inheritance

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CANNOT DEFINE TEMPLATES INSIDE A FUNCTION
// TEMPLATES MUST BE GLOBAL
  template <typename T>
  T myMin(T a, T b) { return a < b? a : b; }

  // function template specialization
  template <> // (tell compiler to turn off template for the next function)
  const char *myMin(const char* a, const char*b) { return strcmp(a,b) < 0 ? a: b; }

  template <class T,int SIZE>   // C++ OOP244 class template with non-class parameter
  class templateType {
    T data[SIZE];
  };

  // variadic templates C++11 OOP345
  // Variable argument (variadic) templates work by defining two or more templates, 
  // that is:
  //  *  one or more specializations 
  //  *  the actual template with the variable arguments.
  // the compiler generates multiple recursive calls to the variadic template function 
  // each time calling one of the viable specialized template until all args are processed.

  // NOTE: uncomment the 'std::cout << __PRETTY_FUNCTION__ << "\n";' to see what is going on

  template <typename T> // a C++ OOP244 function template
  void print(const T& t) { // function template print for single argument
    // std::cout << __PRETTY_FUNCTION__ << "\n";
    std::cout << t << "\n";
  }

  template <typename First, typename... Rest>  // a C++11 variable argument (variadic) template
  void print(const First& first, const Rest&... rest) {
    // std::cout << __PRETTY_FUNCTION__ << "\n";
    print(first); // calls above function template
    print(rest...); // recursively call this variadic template using 'pack expansion' '(const Rest&...)'
  }

void Templates()
{
  std::cout << "templates OOP244\n";

  // C++ OOP244 function template
   // templates were motivated from #define macros
  #define myMinMacro(a,b) (a < b? a: b)
  std::cout << "myMinMacro(-100.,99.)=" << myMinMacro(-100.,99.) << "\n";
  double a=1, b = 2;
  std::cout <<"a,b=" << a << "," << b << "\n";
  std::cout << "myMinMacro(a,b))=" << myMinMacro(a,b) << "\n";
  a=1, b=2;
  std::cout << "myMinMacro(a++,b++))=" << myMinMacro(a++,b++) << "\n";
  std::cout <<"a,b=" << a << "," << b << "\n";
  a=1, b=2;
  std::cout << "myMinMacro(2*a+9,3*b+5))=" << myMinMacro(2*a+9,3*b+5) << "\n";
  std::cout <<"a,b=" << a << "," << b << "\n";
  a=1, b=2;

  #define myMinMacro2(a,b) ((a) < (b)? (a): (b))
  std::cout << "myMinMacro2(2*a+9,3*b+5))=" << myMinMacro2(2*a+9,3*b+5) << "\n";
  std::cout <<"a,b=" << a << "," << b << "\n";

  double trouble = myMin(-100., 99.);        // calls templated myMin
  const char*  broiled = myMin("chicken", "pig");  // calls 'specialized' myMin

  std::cout << "myMin(-100., 99.) = " << trouble << "\n";

  // if we use the C++11 R"LABEL( ... )LABEL" syntax for the string, we don't have 
  // to insert \'s all over the place to escape the embedded double-quotes in the string.
  std::cout << R"ESCAPE_QUOTES(myMin("chicken", "pig"); = )ESCAPE_QUOTES"  << broiled << "\n";

  // C++ OOP244 class templates
  class intType {   // C++ OOP244
    int data[100];
  };
  intType it;  // it=int type

  templateType<int, 100> itt; // itt = int template type

  print(1); // calls first overloaded print, the function template.

  // these call the second overloaded print, the variadic template, 
  // The compiler generates recursive calls as needed.
  print(10, 20);
  print(100, 200, 300);
  print("one", 2, "three", 3.14159);

} // Templates

void Composition()
{
  // Composition: 
  // The parent contains or is composed of the children.
  // A 'death' relationship.  Destroying the parent, destroys the children.

  // A house contains rooms.
  // Run over a house with a bulldozer and the rooms are destroyed
  class House { 
    class Room {std::string name; public:Room(std::string n) : name(n){}};
    std::list<Room*> rooms; 
  public:
    void AddRoom(std::string name) {rooms.push_back(new Room(name));}
    ~House() { for(auto e : rooms) delete e; } 
  }; 

  House myHouse;
  myHouse.AddRoom("kitchen");
  myHouse.AddRoom("dining room");
  myHouse.AddRoom("bedroom 1");
  myHouse.AddRoom("bedroom 2");
  myHouse.AddRoom("bedroom 3");
  myHouse.AddRoom("bathroom");
  myHouse.AddRoom("basement");
}

void Association()
{
  // Association: A relationship between two or more objects where all objects 
  // have their own lifecycle and there is no owwner.

  // Clubs and people are independent.  The club can fold or a person can die 
  // without destroying the other.
  class Person; class Club;
  class Person { std::list<Club> clubs; };     // people can belong to multiple clubs
  class Club   { std::list<Person> members; }; // clubs have more than one member

  class Student; class College;
  class Student {
    std::list<College*> colleges;  // can enroll in more than one college
  public:
    void AddCollege(College* college) { colleges.push_back(college); }
  };
  class College {
    std::list<Student*> students;
  public:
    void AddStudent(Student* student) { students.push_back(student); }
  };

  College Seneca;
  College Centennial;
  College Sheridan;

  Student Tom;
  Student Dick;
  Student Jane;
  Student Mary;
  
  Tom.AddCollege(&Seneca);
  Tom.AddCollege(&Sheridan);

  Seneca.AddStudent(&Tom);
  Sheridan.AddStudent(&Tom);

  Dick.AddCollege(&Seneca);
  Seneca.AddStudent(&Dick);
  
  Jane.AddCollege(&Seneca);
  Seneca.AddStudent(&Jane);
  
  Mary.AddCollege(&Centennial);
  Centennial.AddStudent(&Mary);
  
}

void Aggregation()
{
  // Aggregation: Specialized form of Association where all objects have their 
  // own lifecycle but there is ownership.  
  // This a "whole-part" or "a-part-of" relationship.
  // There is one parent but no lifetime dependency of the child.
  // Child object cannot belong to another parent object.
  // "has-a" relationship

  // A motor can exist without a car.
  // A car can exist without a motor
  // A motor can only be in one car.
  class Motor{ };
  class Car{
    Motor* motor;  // A car "has-a" motor.
  public:
    Car()         : motor(nullptr) {}
    Car(Motor* m) : motor(m)       {}
    void newMotor(Motor* m) { if(motor) delete motor; motor = m; }
    Motor* pullMotor() { Motor* ret = motor; motor = nullptr; return ret; }
    ~Car() { if(motor) delete motor; }
  };

  // A duck can exist without a pond
  // A pond can exist without a duck
  // A duck can only be in one pond.
  class Duck {};
  class Pond {
    std::list<Duck> ducks;
    public:
    void AddDuck(Duck duck) { ducks.push_back(duck); }
  };

  Pond golden;
  Pond oxbow;

  Duck donald;
  Duck ronald;
  Duck daffy;
  Duck black;
  Duck bad;
  Duck good;
  Duck evil;
  Duck neutral;

  golden.AddDuck(donald);
  golden.AddDuck(ronald);
  golden.AddDuck(daffy);
  golden.AddDuck(black);
  golden.AddDuck(good);
  golden.AddDuck(neutral);

  oxbow.AddDuck(bad);
  oxbow.AddDuck(evil);
}

void Expressions()
{
   int i = 2;
   std::cout << "i=" << i << "\n";
   i = i++; // favorite question on JAVA final exam
   std::cout << "after 'i = i++;',  the value of i is " << i << "\n";

   std::cout << "what happens if we have an array 'a[i] = a[i++];'?\n";
   double trouble[] = { 1,2,3,4,5 };
   std::cout << "double trouble:"; for(auto e : trouble) std::cout << e; std::cout << "\n";

   i = 2;
   std::cout << "i=" << i << "\n";
   trouble[i] = trouble[i++];
   std::cout << "i=" << i << "\n";

   std::cout << "double trouble:"; for(auto e : trouble) std::cout << e; std::cout << "\n";

 #if 1
   // char *ref = "123456789";
   char ref[] ={ '1','2','3','4','5','6','7','8','9','\0'};
   char *p = ref;
   std::cout << "ref=" << ref << "\n";
   std::cout << "*p = " << *p << "\n";
   std::cout << " before : *p = *p++;\n";
   std::cout << " segmentation fault?\n";
   signal(SIGSEGV,  // SIGSEGV = segment violation, a 'segfault' 
     [] (int sig) { 
       std::cout << "caught signal " << sig << " (SIGSEGV=" << SIGSEGV << ") - YES! it SEGFAULTED\n"; 
       exit(sig); 
     } );
   *p = *p++;
   std::cout << " after : *p = *p++;\n";
   std::cout << "*p = " << *p << "\n";
   std::cout << "*p offset from ref = " << p-ref << "\n";
 #endif

} // Expressions

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

void Functions()
{
  // 1. C pointer to a function - OOP144
  double (*f)(double arg);  // a pointer to a function that returns a dounble and takes a double as an argument

  f = sqrt;
  std::cout <<  "sqrt(" << 1000. << ")=" << f(1000.) << "\n"; 
  f = fabs;
  std::cout <<  "fabs(" << -1000. << ")=" << f(-1000.) << "\n"; 

  #define F(FUN,ARG) f=FUN; std::cout <<  #FUN "(" #ARG ")=" << f(ARG) << "\n"; 
  F(sqrt, 1000.)
  F(fabs, -1000.)
  #undef F

  // 2. C++ pointer to a function
  std::function<double(double)> g;
  #define G(FUN,ARG) g=FUN; std::cout <<  #FUN "(" #ARG ")=" << g(ARG) << "\n"; 
  G(sqrt, 81.)
  G(fabs, -123.)
  #undef G

  // 3. function objects or functors  (overload operator() for a class)
  class MyFunctionObject {
  public:
    int operator() (int arg) { return 2*arg; }
  };
  MyFunctionObject mfo;
  std::cout << "mfo(99)=" << mfo(99) << "\n";

  // 4. lambda functions
  auto lambda = [] (int arg) { return 3*arg; };

  std::cout << "lambda(99)=" << lambda(99) << "\n";

  SimpleDIYPrintf("variable argument function SimpleDIYPrintf: char '%c', string '%s', int '%d', float '%f'\n", '?', "QWERTY", 99, 199.);
} // Functions

void ErrorHandling()
{
   try {
     double trouble = 30 / 1.001; // frame rate for NTSC Television 29.970 FPS
     throw std::string("throwing a string");
   } catch (const std::string& e) {
     std::cout << e << "\n";
   } catch(...) {
     std::cout << "Unknown error\n";
   }

   try {
     double trouble = 30 / 1.001; // frame rate for NTSC Television 29.970 FPS
     throw nullptr;               // need to throw nullptr or program will segfault
   } catch (const std::string& e) {
     std::cout << e << "\n";
   } catch(...) {                 // catch 'throw nullptr'
     std::cout << "Unknown error\n";
   }

   try {
     for (int i = 0; i < 10; i++) {
      if(i == 7) throw std::string("i is 7");
     }
     throw nullptr;
   } catch (const std::string& e) {
     std::cout << e << "\n";
   } catch(...) {                 // catch 'throw nullptr'
     std::cout << "Unknown error\n";
   }

   try {
     uint8_t* pointerToTeraByte = new uint8_t [ 1000000LL * 1000000LL * 1000000LL];
     // or uint8_t* pointerToTeraByte = new uint8_t [ size_t(1.e18) ];
     delete [] pointerToTeraByte;  // might work.  If it did, delete it.
   } catch(std::exception& e) {
     std::cout << "std::exception error=" << e.what() << "\n"; // expect std::bad_alloc()
   }

} // ErrorHandling

template <class T>  // templates have to be global
class DIYStack {
  class Node {
  public: T     data;
    Node *link;
  } *head;

public:
  DIYStack() : head(nullptr) {}
  void Insert(T d) 
  {
    Node* n = new Node;
    n->data = d;
    n->link = head;
    head = n;
  }
  void Print() 
  {
    for(Node* p=head; p; p=p->link)
      std::cout << p->data << "\n";
  }
  size_t Size() 
  {
    size_t size = 0;
    for(Node* p=head; p; p=p->link)
      size++;
    return size;
  }

};


void LinkedLists()
{
  DIYStack<std::string> s;
  s.Insert("rat");
  s.Insert("ox");
  s.Insert("tiger");
  s.Insert("rabbit");
  s.Insert("dragon");
  s.Insert("snake");
  s.Insert("horse");
  s.Insert("sheep");
  s.Insert("monkey");
  s.Insert("rooster");
  s.Insert("dog");
  s.Insert("pig");

  std::cout << s.Size() << " Chinese animals - NOTE print order is opposite of insert order\n";
  std::cout << "'rat' was inserted first. 'pig' was inserted last.\n";
  std::cout << "This list is a stack, a LILO: last in, last out.\n";
  s.Print();
} // LinkedLists

void STLContainers()
{
  std::list<std::string> l;
  l.push_back("rat");
  l.push_back("ox");
  l.push_back("tiger");
  l.push_back("rabbit");
  l.push_back("dragon");
  l.push_back("snake");
  l.push_back("horse");
  l.push_back("sheep");
  l.push_back("monkey");
  l.push_back("rooster");
  l.push_back("dog");
  l.push_back("pig");
  std::cout << l.size() << " Chinese animals - NOTE print order is the same as the insert order\n";
  std::cout << "'rat' was added to the end of the list first. 'pig' was added to the end of the list last.\n";
  std::cout << "This list is a queue, a FIFO: first in, first out. Just like the coffee lineup at Tim Hortons.\n";
  for(auto e : l) std::cout << e << "\n";
} // STLContainers

void StdLibAlgorthms()
{
  // many useful algorithms
   double trouble[] = { 1,2,3,4,5 }; // compiler promotes integers into doubles
   std::cout << "sum of trouble = " << std::accumulate(trouble+0, trouble+5, 0.0) << "\n";

   double bigTrouble[] = { 6,7,8,9,10 };
   std::cout << "sum of bigTrouble = " << std::accumulate(bigTrouble+0, bigTrouble+5, 0.0) << "\n";

   std::cout << "sum over i of trouble[i] * bigTrouble[i] = " << std::inner_product(trouble+0, trouble+5, bigTrouble, 0.0) << "\n";

   std::cout << "Algorithms also work for for STL containers\n";
   std::vector<double> vecTrouble = { 1.,2.,3.,4.,5. };
   std::cout << "sum of vecTrouble = " << std::accumulate(vecTrouble.begin(), vecTrouble.end(), 0.0) << "\n";

   std::vector<double> vecBigTrouble = { 6.,7.,8.,9.,10. };
   std::cout << "sum of bigTrouble = " << std::accumulate(vecBigTrouble.begin(), vecBigTrouble.end(), 0.0) << "\n";

   std::cout << "sum over i of vecTrouble[i] * vecBigTrouble[i] = " << std::inner_product(vecTrouble.begin(), vecTrouble.end(), vecBigTrouble.begin(), 0.0) << "\n";
 
 
} // StdLibAlgorthms

void Pointers()
{
  try {
    // raw pointer (not smart, so must be dumb)
    uint8_t *p = new uint8_t[1000000];  // 1 MB

    // smart pointer, (not raw, so must be cooked)
    std::unique_ptr<uint8_t> up(new uint8_t[1000000]); // 1 MB

    throw std::string("the smart pointer will be automatically deleted, the raw pointer will not be deleted");

    // when variables, 'p'+'up' go out of scope because of the throw, 'up' is automatically deleted, 'p' is not deleted.
    delete [] p;

  } catch (const std::string& e) {
    std::cout << "try threw: " << e << "\n";
  }
} // Pointers

void Threading()
{
  std::string msg( // use C++11 "R string literal" to create message
  // in this case, the string literal is everything between the opening 'foo(' and the closing ')foo'.
  // the string literal contains all newline '\n' at the end of each line.
  // 'foo' can be any string, or can be absent

  R"foo(What is a thread?

  A thread is a function possibly executing in parallel with the main program.
  Clearly if there is only one core on a computer, threads cannot cannot execute 
  in parallel.  Only one at a time can be active on the single processing unit (core).

  Threads share all global memory with the main program and other threads.

  Threads need to be able to, at times, have exclusive access to a memory item
  or be the only thread executing. They need to wait their turn for access to
  this memory or wait their turn to execute (mutex locks)

  Threads need to able to hold or wait until told to continue (condition variables).

  Threads need to be able to perform some operations without being interrupted
  (atomic operations).

  To summarize, threads are simply C or C++ functions that execute in parallel
  subject to an available processing element (core).  We use C++ functions in 345.
  )foo"

  );

  std::cout << msg;

  auto cores = std::thread::hardware_concurrency();  // Make the compiler figure out the variable type
  std::cout << "\nThis computer has " << cores << " cores.\n";
  std::cout << "------------------------ thread/join threading\n";

  auto function = [] () { 
      // std::cout << std::string("hello from thread ") + std::to_string(std::this_thread::get_id()) + "\n"; 
      // Compiler won't coerce get_id() data type to something to_string can format
      std::thread::id tid = std::this_thread::get_id();
      uint64_t u64 = 0LL;
      memcpy(&u64, &tid, sizeof(tid));
      std::cout << std::string("function: hello from thread get_id() = ") + std::to_string(u64) + "\n"; 
    };

  std::thread t(function); 
  t.join();

  auto threadFunction = [] (int arg, int sleep) { 
    std::this_thread::sleep_for (std::chrono::milliseconds(sleep));
    std::cout << std::string("threadFunction: thread, arg=") + std::to_string(arg) + ", slept for " + std::to_string(sleep) + " msec\n";
  };

  const int NUM_THREADS = 10;
  std::thread threads[NUM_THREADS];
  int  arg = 100;
  for (int i = 0; i < NUM_THREADS; ++i)
    threads[i] = std::thread(threadFunction, arg++, rand() % 1000);  // sleep for a random number of milliseconds

  // wait for thread to complete
  for (int i = 0; i < NUM_THREADS; ++i)
    threads[i].join();

  // how to get return value from a thread?  Futures!
  auto returnValueThreadFunction = [] (int arg) -> int { 
    int sleepMSec =  rand() % 1000;
    std::this_thread::sleep_for (std::chrono::milliseconds(sleepMSec));
    std::cout << std::string("returnValueThreadFunction: arg ") + std::to_string(arg) + " slept for " + std::to_string(sleepMSec) + " msecs\n";
    return 1000000 * arg + sleepMSec;
  };

  std::cout << "------------------------ future/get threading\n";
  arg = 200;
  std::future<int> fut = std::async(std::launch::async, returnValueThreadFunction, arg++);
  int returnedValue = fut.get(); // wait for thread to finish and get result.
  std::cout << "Thread returned " << returnedValue << "\n";

  std::future<int> futs[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; ++i)
    futs[i] = std::async(std::launch::async, returnValueThreadFunction, arg++);
  for (int i = 0; i < NUM_THREADS; ++i) {
    returnedValue  = futs[i].get();
    std::cout << std::string("future Thread #") + std::to_string(i) + " returned " + std::to_string(returnedValue)+ "\n";
  }

} // Threading

void FileStream()
{
  // TBA
  // C++ OOP244 disk file operations.
  // open, close, read, write, seek, tell
} // FileStream

void QuadraticComplexity()
{
  std::cout << "QuadraticComplexity\n";

  class Timer { // use C++11 std::chrono features to create a stop-watch timer class
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> stop;
  public:
    Timer() {}
    void Start() { start = std::chrono::high_resolution_clock::now(); }
    void Stop () { stop  = std::chrono::high_resolution_clock::now(); }
    // basic form to calculate time differences, illustrate with microseconds
    uint64_t usecs() {
      typedef std::chrono::duration<int,std::micro> microsecs_t ;
      microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
      uint64_t us = duration_get.count();
      return us;
    }
    // Now use a macro to return milli, micro, and nano seconds 
    #define RET(UNITS) uint64_t UNITS##secs() { \
      typedef std::chrono::duration<int,std::UNITS> UNITS##secs_t ; \
      UNITS##secs_t duration_get( std::chrono::duration_cast<UNITS##secs_t>(stop-start) ) ; \
      uint64_t us = duration_get.count(); \
      return us; \
    }
    RET(milli) // creates member function 'uint64_t millisecs()' - which returns 'stop-start' in millisecs
    RET(micro) // creates member function 'uint64_t microsecs()' - which returns 'stop-start' in microsecs
    RET(nano)  // creates member function 'uint64_t nanosecs()'  - which returns 'stop-start' in nanosecs
  };

  uint64_t N;
  uint64_t z;
  Timer    t;
  std::vector<double> X;
  std::vector<double> Y;

  auto PrintMicroSeconds = [&t] (std::string msg) -> uint64_t {
      t.Stop();
      uint64_t elapsedMicroseconds = t.microsecs();
      std::cout << msg << elapsedMicroseconds << " usecs\n";
      return elapsedMicroseconds;
  };

  N = 500;
  for(int scale = 0; scale < 13 ; scale++) {
    t.Start();
    X.push_back(double(N));
    z = 0;
    for(uint64_t  x = 0; x < N; x++)
      for(uint64_t y = 0; y < N; y++)
        z += x + y*y;
    Y.push_back(double( PrintMicroSeconds("Scale=" + std::to_string(scale) + " N=" + std::to_string(N) + " ") ) );
    N *= 1.5;
  }

  // If time (Y) varies as a power 'p' of size (X)
  // Take the log of both sides.
  // log(Y) = 'p' * log(X)
  // fit a straight to the log of X+Y. The slope will be the power 'p'

  // if 'p' is around 1, time varies as size varies. 
  //     It is a linear algorithm. Double the size takes double the time.
  // if 'p' is around 2, time varies as a power of 2.  
  //     It is a quadratic algorithm.  Double the size takes 4 times as long
  // if 'p' is around 3, time varies as a power of 3.  
  //     It is a cubic algorithm. Double the size takes 8 times as long
  for (size_t i = 0; i < X.size(); i++ ) {
    X[i] = log10( X[i] );
    Y[i] = log10( Y[i] );
  }

  // statistics 101: fit a line Y = Slope*X+Intercept 
  // think algebra 101 straight line equation: y = m*x+b
  // m = slope dy/dx = delta y / delta x = rise over run
  // b = y-axis intercept
  auto regression = [X,Y] (double& Slope, double& Intercept, double& R2) -> void
    {
       if(X.size() == 0) // Do not divide by zero.
           throw std::string("Need 1 or more data points to fit a linear regression line.");

       if(X.size() != Y.size())
           throw std::string("What? X+Y sizes are different!");
       // symbols:
       // SX  = sum of X elements: X[i]
       // SY  = sum of Y elements: Y[i]
       // SXX = sum of X squared: X[i] * X[i]
       // SXY = sum of X * Y crossproducts: X[i] * Y[i]
       double SX = 0.,  SY = 0.,  SXX = 0., SYY = 0.,  SXY = 0.;
       // brute force calculation
       for (int i = 0; i < X.size(); i++) {
          SX  += X[i];
          SY  += Y[i];
          SXX += X[i] * X[i];
          SYY += Y[i] * Y[i];
          SXY += X[i] * Y[i];
       }

       // calculate using std::algorithms functions.  
       // These may be optimized compared to brute force calculation
       // not a big deal for small counts.  May make a difference for huge counts.
       int    n   =  Y.size();
       SX  =  std::accumulate(X.begin(), X.end(), double(0.)); // easy to understand

              // or alternatively, using accumulate with a function that happens to be a lambda
              SX =   std::accumulate(X.begin(), X.end(), double(0),  // HARDER TO UNDERSTAND!
                        [] (double sum, double element) { return sum + element; }
                 );

       SY  =  std::accumulate(Y.begin(), Y.end(), double(0.)); // easy to understand

       SXX =  std::inner_product(X.begin(), X.end(), X.begin(), double(0.)); // easy to understand

              // or alternatively, using accumulate with a function that happens to be a lambda
              SXX =  std::accumulate(X.begin(), X.end(), double(0.),   // HARDER TO UNDERSTAND!
                        [] (double sum, double element) 
                        { return sum + element * element; } // calculate sum of X squared using function
                  );
       SYY =  std::inner_product(Y.begin(), Y.end(), Y.begin(), double(0.)); // easy to understand
       SXY =  std::inner_product(X.begin(), X.end(), Y.begin(), double(0.));

       // statistics 101: straight line equation
       Slope      =  (n*SXY - SX*SY)    / (n*SXX - SX*SX);
       Intercept  =  (SY    - Slope*SX) / n;
       R2         =  (n*SXY - SX*SY)    / (sqrt(n*SXX-SX*SX) * sqrt(n*SYY-SY*SY));
    };

  std::cout << "Data:\n";
  for (size_t i = 0; i < X.size(); i++ )
    std::cout << i << ": " << X[i] << "," << Y[i] << "\n";

  double Slope, Intercept, R2;
  regression(Slope, Intercept, R2);
  std::cout << "slope=" << Slope << " <-- if this is quadratic, the slope will be about 2\n";
  std::cout << "intercept="  << Intercept << " <-- not relevant.  Calculated it, so print it\n";
  std::cout << "correlation coefficient, R squared=" << R2 << " <-- values near 1 imply regression is a good fit!\n";

} // QuadraticComplexity

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week01()
{
  NamespaceCompilation();
} // week01

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week02()
{
  FundamentalTypes();
  CompoundTypes();
} // week02

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week03()
{
  Inheritance();
  Templates();
} // week03

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week04()
{
  Composition(); 
  Association();
  Aggregation(); 
} // week04

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week05()
{
  Expressions();
  Functions();
} // week05

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week06()
{
  ErrorHandling();
  LinkedLists();
} // week06

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week07()
{
  STLContainers();
} // week07

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week08()
{
  StdLibAlgorthms();
  QuadraticComplexity();
} // week08

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week09()
{
  Pointers();
} // week09

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week10()
{
  Threading();
} // week10

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week11()
{
  // Quiz 2
  FileStream();
} // week11

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week12()
{
  // TBA
} // week12

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void week13()
{
  // TBA
  // Course review
} // week13


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void run(std::string label, void(*f)())
{
  std::cout << "RUNNING [" << label << "]\n";
  f();
  std::cout << "--> FINISHED [" << label << "], press any key to continue\n";
  char c; std::cin.get(c); std::cout << "\n";
}

#define RUN(FUNCTION, LABEL) run("week" #FUNCTION " - " #LABEL, week##FUNCTION); 
// NOTE: #  is the string operator
//       ## is the concatenation operator

// RUN(01, whatever 1 2 3) expands to
// 1. run("week" "01" " - " "whatever 1 2 3", week01);
//    The compiler concatenates string 
//    "week" "01" " - " "whatever 1 2 3"  into  "week01 - whatever 1 2 3"
// 2. week##FUNCTION expands into week01
// The resulting call is:
// run("week01 - whatever 1 2 3", week01);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char**argv)
{
#ifdef __MSVC__   // one of the many, many, many Microsoft visual studio c compilers
  std::cout << R"msc(
  Everytime I go near Windows, Windows bites me. 
  
  Windows programs execute with a tiny stack.

  This code generates some classes with data elements that are a million bytes plus.
  For encapsulation, these classes are created local to a function, that is, on the stack.

  This causes a segfault if this code is run under windows. :-( :-( :-(

  One can specify the stack size for a thread under windows.  See 
  http://msdn.microsoft.com/en-us/library/windows/desktop/ms686774%28v=vs.85%29.aspx

  One can request Windows to run a program with a realistic stack size.
 
  Here's how:
    1. if using the MSC compiler try the /F <size> compiler option
           to set the stack to 10 MB:
           CL fastforward.cpp /F 10000000
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
  std::cout << "press any key to continue\n";
  char c; std::cin.get(c); std::cout << "\n";
#endif

#ifdef __GNUC__  // either g++ or clang++
  std::cout << "\nFILE '" << __FILE__ << "' compiled " 
            << __DATE__ << " at " << __TIME__ << " by\n";
#ifdef __clang__
  std::cout << "clang++ compiler " << __VERSION__ << "\n";
  #else
    std::cout << "g++ compiler " << __VERSION__ << "\n";
  #endif
  std::cout << "--> generating " << 8*sizeof(void*) << "-bit code\n";

  std::cout << R"gnu(
  /******************************************************************\
  * This program was developed on a 64-bit Ubuntu 14.04 LTS system.  *
  * It runs correctly in this environment.                           *
  *                                                                  *
  * Compile with either g++ or clang++                               * 
  *   g++     -std=c++11 fastforward.cpp -o fastfoward -pthread      *
  *   g++-5   -std=c++11 fastforward.cpp -o fastfoward -pthread      *
  *   clang++ -std=c++11 fastforward.cpp -o fastfoward -pthread      *
  *                                                                  *
  *  It compiles and runs fine with the Ubuntu 15.04 compilers.      *                                                                 *
  *                                                                  *
  * It also runs fine on Matrix, a 2011 32-bit SUSE linux system.    *
  * Compile with either g++ 4.9.0 using the -std=c++11 flag          *
  *   /usr/local/gcc/gcc-cilk/bin/g++ -std=c++11 fastforward.cpp ... *
  * or compile with /usr/bin/g++ 4.6.2 using the -std=c++0x flag     *
  *   g++ -std=c++0x fastforward.cpp -o fastfoward -pthread          *
  *                                                                  *
  * NOTE:                                                            *
  * The Matrix clang++ will not compile fastforward.cpp.             *
  * The Matrix clang++ (version 3.4) chrono header files are broken. *
  * Just including <chrono.h> generates syntax errors.               *
  *     (use g++ instead)                                            *
  \******************************************************************/

  )gnu";
#endif

  RUN(01, create namespaces + multiple source files)
  RUN(02, fundamental + compound types);
  RUN(03, inheritance + templates);
  RUN(04, composition + aggregation + association);
  RUN(05, expressions + functions);
  RUN(06, error handling + linked lists);
  RUN(07, STL containers + test 1);
  RUN(08, stdlib algorithms + quadratic complexity);
  RUN(09, pointers);
  RUN(10, threading);
  RUN(11, file stream);
  RUN(12, more fundamental + compound types);
  RUN(13, more on functions + course review);

  std::cout << R"bar(So there you go.

  This 'fastforward' program illustrates some of the concepts we will learn in 345.

  Don't worry about it if there is anything you do not understand.  

  We have 13 weeks to sort it out.  Don't be intimidated.  You will like C++11.

  C is shorthand for the assembler used on 1970's vintage PDP/11 minicomputers.  
  ++, and --, were hardware instructions on the PDP/11.

  C is a low level language, perfect for low level things.

  C++ is a better C.  It is easier to code things in C++ than C.

  Similarily, C++11 is a better C++.  It is easier to code things in C++11 than C++.

  C++11 makes things easier to do.  

  It takes less time to create C++11 programs.

  C++11 programs are less complex than C++.  

  Simplier programs have fewer bugs.  C++11 programs a have fewer bugs.

  It is easier to modify and maintain C++11 programs.

  C++11 is an exciting development.  It is easy to learn and once
  you have some experience with C++11, there is no going back.
  You will not want to code in legacy C++ (1998, or 2003), or C.

  The best way to learn anything is practise.  This applies to aquiring
  any new skill.  Learning C++11 is not different.  You need to write many
  short simple programs to see how a feature works.  You learn by doing.

  I hope this 'fastforward.cpp' can be a starting point for your own explorations
  as you master C, C++, and C++11.

  OOP345 is an exciting course.  I am honoured help you become C++11 wizzards.

  Enjoy!

  Greg Blair, 
  john.blair@senecacollege.ca

  )bar";
  
  return 0;
}
