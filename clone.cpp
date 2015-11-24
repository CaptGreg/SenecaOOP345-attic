// clone member function. 
// see inheritance section, page 59+60, of text book 
// and http://www.cplusplus.com/forum/articles/18757/

#include <iostream>
#include <vector> // works with vector, a contiguous memory table
#include <list>   // works with linked lists, not a contiguous memory table
#include <memory>

using namespace std;

class B {  // an ABC, (abstract base class)
public:
  virtual ~B() {cout<<"B:dtor\n";}   // ABC: virtual destructors to insure the dtor for any B* pointer to a derived class is called 
  virtual B* clone() = 0;
};

class X : public B {
  list<uint16_t> v;  // or vector<uint16_t> v;
public:
  ~X() {cout<<"X:dtor\n";}

//B* clone() { return new X(*this); }  // see page 59+60 of text book.
  X* clone() { return new X(*this); }  // Covariant Return Type: ABC declares B*clone() + class X declares X*clone()

  void operator+=(int vv) { v.push_back(vv); }
  void print() { for(auto element : v) cout << element;  cout << "\n"; }
};

int main(int argc, char**argv)
{
  X x;
  x += 1;
  x += 2;
  x += 3;
  cout << "x=";  x.print();
  
  // X* xClone =  x.clone(); 
  // error: cannot initialize a variable of type 'X *' with an rvalue of type 'B *'
  // fix error with a cast
  // X* xClone = dynamic_cast<X*> ( x.clone() );  // clone calls new, must delete.
  unique_ptr<X> xClone(dynamic_cast<X*> ( x.clone() ) );

  cout << "xClone="; xClone->print();

  cout << "adding 4 to xClone\n";
  *xClone += 4;
  cout << "xClone="; xClone->print();

  cout << "if x prints 123 + xClone prints 1234, they are different\n";
  cout << "This means STL did a deep copy during the 'clone of x' operation.\n";
  cout << "x=";  x.print();

  // yes, x printed 123.

  // delete xClone;  // clone called new, must delete
}
