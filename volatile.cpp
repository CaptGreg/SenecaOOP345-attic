// When the compile generates 'cv-error' error messages, the cv refers to const-volatile
// There are four combinations
// 1 - none
// 2 - 'const'
// 3 - 'volatile'
// 4 - both
// What is a 'const volatile'?  Is is const or is it volatile?

// NOTE: There is the 'mutable' keyword where 'mutable' = not 'const'.

#include <iostream>

int main()
{

  class X {
    int i;
  public:
    X(int xx) : i(xx) {}
    void x(int xx) { i = xx; }
    int  x() { return i++; }

    const            int  cx() { return i++; }
    volatile         int  vx() { return i++; }
    const volatile   int cvx() { return i++; }
    volatile const   int vcx() { return i++; }
#ifdef SHOW_ERRORS
    mutable          int  mx() { return i++; }
    mutable const    int mcx() { return i++; } // mutable members of const classes are modifiable.
    mutable volatile int mvx() { return i++; }
#endif
  };

  X x(1);

  int x1 = x.x(), x2 = x.cx(), x3 = x.vx(), x4 = x.cvx();
  std::cout << x1 << x2 << x3 << x4 << "\n";

  std::cout << x.x() << x.cx() << x.vx() << x.cvx() << "\n";

  std::cout <<  (x1 = x.x()) << (x2 = x.cx()) << (x3 = x.vx()) << (x4 = x.cvx()) << "\n";
  
  std::cout << x.x() << x.cx() << x.vx() << x.cvx() << x.vcx();

  std::cout 
#ifdef SHOW_ERRORS
  << x.mx() 
  << x.mcx() 
  << x.mvx() 
#endif
  << "\n";
}
