#include <iostream>
#include <cstring>    // memcpy
#include <vector>     // standard template library, STL, vector
#include <memory>     // unique_ptr

using namespace std;

template <class T>
class X3 {  // rule of 3
protected:
  size_t size;
  T*     data;
public:
  X3() : size(0), data(nullptr) 
  { cout << "X3() constructor()\n"; }

  X3(size_t s) : size(s), data(new T[size]) 
  { cout << "X3 (" << s << ") constructor\n"; }

  virtual ~X3() 
  { 
    cout << "X3 destructor, size=" << size << "\n";
    delete [] data;
  }

  virtual void print() 
  {
    cout << "X3 print: size=" << size << " (" << size*sizeof(T) << " BYTES)\n";
  }

  virtual X3& operator= (const X3& rhs) // C++
  {
    cout << "X3 copy assignment operator size,rhs.size=" << size << "," << rhs.size << "\n";
    if(this != &rhs) {
      delete [] data;

      data = nullptr;
      size = 0;

      if(rhs.data) {
        size = rhs.size;
        // data = new T[size];
        // memcpy( data, rhs.data, size * sizeof(T) );
        data = (T*) memcpy( new T[size], rhs.data, size * sizeof(T) );  // 1 line
      }
    } else {
      cout << "X3 copy assignment operator called on itself\n";
    }
    return *this;
  }

  X3(const X3& rhs) // C++ copy constructor
  {
    cout << "X3 copy constructor rhs.size=" << rhs.size << "\n";
    data = nullptr;
    size = 0;
    *this = rhs;      // Let copy assignment operator do the work.
  }
}; //X3

template <class T>
class X : public X3<T> { // rule of 5: inheritance use the rule of 3 as a base class
public:
  ~X() { cout << "X destructor, size=" << X3<T>::size << "\n"; }
  // X() : X3<T>() {}
  X() = default;
  X(size_t s) : X3<T>(s) {}

  // X& operator= (const X& rhs) { X3<T>::operator=( rhs ); return *this; } // C++ copy assignment operator
  X& operator= (const X& rhs) = default;

  // X(const X& rhs) :X3<T>(rhs) {} // C++ copy constructor  
  X(const X& rhs) = default;

  X&& operator= (X&& rhs) // C++11 move assignment operator
  {
    cout << "X move assignment operator size,rhs.size=" << X3<T>::size << "," << rhs.size << "\n";
    if(this != &rhs) {
      delete [] X3<T>::data;

      X3<T>::size = rhs.size; // Steal brains (copy size+pointer)
      X3<T>::data = rhs.data;

      rhs.size = 0;    // Zombie
      rhs.data = nullptr;
    } else {
      cout << "X move assignment operator called on itself\n";
    }
    return std::move(*this);
  }

  X(X&& rhs) // C++11 move constructor
  {
    cout << "X move constructor rhs.size=" << rhs.size << "\n";
    X3<T>::data = nullptr;
    X3<T>::size = 0;
    *this = std::move(rhs);      // Let move assignment operator do the work.
  }
};

int main(int argc, char**argv)
{
 cout << "program " << argv[0] << "\n\n";

 X<double> x, y(125 * 1000 * 1000);  // 8 * 125 * 1000 * 1000 = 1,000,000,000 BYTES

 cout << "LINE " << __LINE__ << ": x = y;\n";
 x = y;

 cout << "LINE " << __LINE__ << ": X<double> z(x);\n";
 X<double> z(x);

 cout << "LINE " << __LINE__ << ": y = std::move(x);\n";
 y = std::move(x);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": x = std::move(y);\n";
 x = std::move(y);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": y = std::move(x);\n";
 y = std::move(x);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": x = std::move(y);\n";
 x = std::move(y);
 cout << "x:"; x.print();
 cout << "y:"; y.print();

 cout << "LINE " << __LINE__ << ": X<double> A(std::move(x));\n";
 X<double> A(std::move(x));
 cout << "x:"; x.print();
 cout << "A:"; A.print();

#if 1
 cout << "\n";
 cout << "Is pushing X<double>(size) different from pushing *unique_ptr<X<double>> new X<double>(size) ?\n";
 {
   cout << "\n";
   vector< X<double> > v;
   cout << "LINE " << __LINE__ << ": v.push_back(X<double> (1000));\n";
   v.push_back(X<double> (1000));
   v.back().print();
 }

 {
   cout << "\n";
   vector< X<double> > v;
   cout << "LINE " << __LINE__ << ": v.push_back(std::move(* unique_ptr<X<double>> (new X<double> (2000)) ) );\n";
   v.push_back(std::move(* unique_ptr<X<double>> (new X<double> (2000)) ) );
   v.back().print();
 }
 cout << R"foo(

The above code answered the question: Is pushing X<double>(size) different from pushing *unique_ptr<X<double>> new X<double>(size) ?

Answer: NO they both generate exactly the same sequence of calls:
 1 x constructor
 1 x move constructor which called the
 1 x move assignment operator
 2 x destructor

v.push_back(X(1000):
X (1000) constructor
move constructor rhs.size=1000
move assignment operator size,rhs.size=0,1000
X destructor, size=0
X destructor, size=1000

v.push_back(std::move(* unique_ptr<X<double>> (new X<double> (2000)) ) ):
X (2000) constructor
move constructor rhs.size=2000
move assignment operator size,rhs.size=0,2000
X destructor, size=0
LINE 139: v.push_back(std::move(*up));
X destructor, size=2000

Now try it with a class without move semantics
)foo";
 {
   cout << "\n";
   vector< X3<double> > v;
   cout << "LINE " << __LINE__ << ": v.push_back(X3<double> (3000));\n";
   v.push_back(X3<double> (3000));
   v.back().print();
 }

 {
   cout << "\n";
   vector< X3<double> > v;
   cout << "LINE " << __LINE__ << ": v.push_back(std::move(* unique_ptr<X3<double>> (new X3<double> (4000)) ) );\n";
   v.push_back(std::move(* unique_ptr<X3<double>> (new X3<double> (4000)) ) );
   v.back().print();
 }
 cout << R"foo(

They both generate exactly the same sequence of calls only this time with copy semantics:
 1 x constructor
 1 x copy constructor which called the
 1 x copy assignment operator
 2 x destructor
)foo";

 cout << "\n";
 {
   cout << "\n";
   vector< X<double> > v;
   cout << "LINE " << __LINE__ << ": v.emplace_back(5000);\n";
   v.emplace_back(5000);
   v.back().print();
 }

#endif
 cout << "\n";

 cout << "LINE " << __LINE__ << ": MAIN OVER\n\n";
}
