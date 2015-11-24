// g++ -Wall -std=c++11 vec-emplace-push.cpp -o vec-emplace-push && ./vec-emplace-push

#include <vector>
#include <iostream>
#include <string>
#include <cstring> // memcpy
#include <memory>  // unique_ptr

using namespace std;

class X {

  char*  b;
  size_t s;


public:

  X() : s(0), b(nullptr) { cout << "ctor, this=" << this << ", s = " << s << "\n"; }

  X(size_t size) : s(size), b(new char[s]) { cout << "ctor, this=" << this << ", s = " << s << "\n"; }

  ~X() { if(b) delete [] b; cout << "dtor, this=" << this << ", s = " << s << "\n"; }
 
  X(const X& rhs) : X()
  { 
    cout << "copy ctor, this=" << this << ", rhs=" << &rhs << "\n"; 
    *this = rhs;  
  }


  X& operator= (const X& rhs)
  {
    cout << "copy = op, this=" << this << ", rhs=" << &rhs << "\n";
    if(this != &rhs) {
      if(b) { delete [] b; b = nullptr; s = 0; }

      if(rhs.b) {
         s = rhs.s;
	 b = new char[s];
	 memcpy(b, rhs.b, s);
      }
    }
    print();
    rhs.print();
    return *this;
  }

  X(X&& rhs) : X()
  { 
    cout << "move ctor, this=" << this << ", rhs=" << &rhs << "\n";
    *this = move(rhs);  
  }

  X&& operator= (X&& rhs)
  {
    cout << "move = op, this=" << this << ", rhs=" << &rhs << "\n";    
    if(this != &rhs) {
      if(b) { delete [] b; b = nullptr; s = 0; }

      s = rhs.s;
      b = rhs.b;
      rhs.s = 0;
      rhs.b = nullptr;
    }
    print();
    rhs.print();
    return move( *this );
  }

  void print() const
  { cout << "print, this=" << this << ", s = " << s << "\n"; }

};

int main(int argc,char**argv)
{
  const size_t SIZE = 100000;

  vector<X> vecX;

  cout << "unique_ptr<X> Xptr1(new X(" << SIZE << "));\n";
  unique_ptr<X> Xptr1(new X(SIZE));
  Xptr1->print();
  cout << "\n";

  cout << "vecX.push_back(std::move(*Xptr1));\n";
  cout << "check out what brief and concise push_back calls:\n";
  vecX.push_back(std::move(*Xptr1));
  Xptr1->print();
  cout << "\n";

  cout << "unique_ptr<X> Xptr2(new X(" << 2*SIZE << "));\n";
  unique_ptr<X> Xptr2(new X(2*SIZE));
  Xptr2->print();
  cout << "\n";

  cout << "vecX.emplace_back(std::move(*Xptr2));\n";
  cout << "WOW! check out what emplace_back calls:\n";
  vecX.emplace_back(std::move(*Xptr2));
  Xptr2->print();
  cout << "\n";

  cout << "dumping vector vecX:\n";
  for( auto& e: vecX) e.print();

  cout << "end of main\n";
  cout << "\n";
}
