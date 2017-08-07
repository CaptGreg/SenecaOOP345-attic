// clone member function. 
// http://www.cplusplus.com/forum/articles/18757/

// Covariant Return Type: ABC declares B*clone() + class X declares X*clone()

#include <iostream>
#include <list>   // works with linked lists, not a contiguous memory table
#include <memory>

using namespace std;

class B {  // an abstract base class
public:
  // ABC: virtual destructors to insure the dtor for any B* pointer to a derived class is called 
  virtual ~B() {cout<<"B:dtor\n";}   
  virtual B* clone() = 0;
};

class X : public B {
  list<uint16_t> v;
public:
  ~X   ()                        { cout<<"X:dtor\n"; }

  X*   clone            ()       { return new X(*this); }  

  unique_ptr<X> cloneUP ()       { return unique_ptr<X> ( new X(*this) ); }

  void operator +=      (int vv) { v.push_back(vv); }
  void print            ()       { for(auto& e : v) cout << e;  cout << "\n"; }
};

int main(int argc, char**argv)
{
  atexit( [] {cout << "atexit callback called: main over, all dtors should have been called\n";});

  X x;
  x += 1;
  x += 2;
  x += 3;
  cout << "x=";  x.print();

  {
    cout << "+++++++++++++++++++++++++++++++++++++++++\n";
    cout << "+ clone result stored in a raw pointer. +\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++\n";
    X* xClone =  x.clone(); 

    cout << "xClone="; xClone->print();

    cout << "adding 4 to xClone\n";
    *xClone += 4;
    cout << "xClone="; xClone->print();

    cout << "if xClone prints 1234, they are different\n";
    cout << "This means STL did a deep copy during the 'clone of x' operation.\n";

    // yes, x prints 123, xClone prints 1234

    delete xClone;  // clone called new
  }


  {
    cout << "+++++++++++++++++++++++++++++++++++++++++++\n";
    cout << "+ clone result stored in a smart pointer. +\n";
    cout << "+++++++++++++++++++++++++++++++++++++++++++\n";

    unique_ptr<X> xClone( x.clone() );

    cout << "xClone="; xClone->print();

    cout << "adding 4 to xClone\n";
    *xClone += 4;
    cout << "xClone="; xClone->print();

    cout << "if xClone prints 1234, they are different\n";
    cout << "This means STL did a deep copy during the 'clone of x' operation.\n";

    // yes, x prints 123, xClone prints 1234
  }


  {
    cout << "+++++++++++++++++++++++++++++++++\n";
    cout << "+ smart pointer clone function. +\n";
    cout << "+++++++++++++++++++++++++++++++++\n";

    unique_ptr<X> xClone( x.cloneUP() );

    cout << "xClone="; xClone->print();

    cout << "adding 4 to xClone\n";
    *xClone += 4;
    cout << "xClone="; xClone->print();

    cout << "if xClone prints 1234, they are different\n";
    cout << "This means STL did a deep copy during the 'clone of x' operation.\n";

    // yes, x prints 123, xClone prints 1234
  }

  cout << "Main exiting, dtor for instance 'x' should be called once main returns\n";
  return 0;
}
