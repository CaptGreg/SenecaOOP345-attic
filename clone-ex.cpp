// clone member function. 
// see inheritance section, page 59+60, of text book 
// and http://www.cplusplus.com/forum/articles/18757/

#include <iostream>
#include <memory>     // unique_ptr
using namespace std;

class Base {
public:
  virtual Base*   clone()   { return new Base(*this); } // calls copy ctor
  virtual void print()   { cout << "   Base::print\n"; }
};

class Derived1 : public Base {
public:
  void print() { cout << "   Derived1::print\n"; }
};

class Derived2 : public Base {
public:
  Derived2* clone()  { return new Derived2(*this); } // calls copy ctor
  void print() { cout << "   Derived2::print\n"; }
};

void cloneAndPrint(Base* b)
{
  cout << "    cloneAndPrint: b      -> print(): "; b->print();
  Base* bClone = b->clone();  
  cout << "    cloneAndPrint: bClone -> print(): "; bClone->print();
}

int main(int argc, char**argv)
{
  Base b;
  cout << "\nmain calling b.print  ==> "; b.print();

  Derived1 d1;
  cout << "\nmain calling d1.print ==> "; d1.print();
  // Derived1* d1Clone = d1.clone(); // :-( syntax error
  // error: cannot initialize a variable of type 'Derived1 *' with an rvalue of type 'Base *'
  Derived1* d1Clone = reinterpret_cast<Derived1*> ( d1.clone() ); // :-) OK, cast it!
  cout << "\nmain calling d1Clone->print ==> "; d1Clone->print();

  Derived2 d2;
  cout << "\nmain calling d2.print ==> "; d2.print();
  Derived2* d2Clone = d2.clone();
  cout << "\nmain calling d2Clone->print ==> "; d2Clone->print();

  cout << "\nmain calling cloneAndPrint(d1Clone)\n"; cloneAndPrint(d1Clone);
  cout << "\nmain calling cloneAndPrint(d2Clone)\n"; cloneAndPrint(d2Clone);

  delete d1Clone;
  delete d2Clone;

  // same clone operations again using unique_ptr

  std::unique_ptr<Derived1> up_d1Clone (reinterpret_cast<Derived1*> ( d1.clone() ));
  cout << "\nmain calling up_d1Clone->print ==> "; up_d1Clone->print();

  std::unique_ptr<Derived2> up_d2Clone ( d2.clone() );
  cout << "\nmain calling up_d2Clone->print ==> "; up_d2Clone->print();
}
