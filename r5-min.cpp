#include <string>
#include <iostream>
#include <utility>     // std::move
#include <cstring>     // memcpy

class A {  //  String has built in rule-of-5 member functions. Use them!  (String is your friend.)
  std::string s;
public:
  A(std::string ss = "") : s(ss)       { }
  A(const A& o) : s(o.s)               { }        // calls string cp ctor
  A(A&& o) : s(std::move(o.s))         { }        // calls string mv ctor
  A& operator = (const A& o) {
     if(this != &o) s = o.s;                      // calls string cp = op
     return *this;
  }
#if 0
  A& operator = (A&& o) {
     if(this != &o) s = std::move(o.s);           // calls string mv = op
     return *this;
  }
#else
  A&& operator = (A&& o) {
     if(this != &o) s = std::move(o.s);           // calls string mv = op
     return std::move(*this);
  }
#endif
};

class B {  //  Wstring has built in rule-of-5 member functions. Use them!  (Wstring is also your friend.)
  std::wstring s;
public:
  B(std::wstring ss = L"") : s(ss)     { }
  B(const B& o) : s(o.s)               { }        // calls wstring cp ctor
  B(B&& o) : s(std::move(o.s))         { }        // calls wstring mv ctor
  B& operator = (const B& o) {
     if(this != &o) s = o.s;                      // calls wstring cp = op
     return *this;
  }
  B& operator = (B&& o) {
     if(this != &o) s = std::move(o.s);           // calls wstring mv = op
     return *this;
  }
};


template<typename T>
class C {
  size_t s = 0;
  T*     b = nullptr;
public:
  ~C() { delete [] b; }
  C(size_t ss=0) : s(ss),  b(new T[ss]) { }
// C(const C& o)  : s(o.s), b((T*) memcpy(new T[s], o.b, s*sizeof(T))) { } // deep copy 'brains' from 'o' // c-style cast (yuk)
  C(const C& o)  : s(o.s), b(reinterpret_cast<T*> (memcpy(new T[s], o.b, s*sizeof(T)))) { } // deep copy 'brains' from 'o'
  C(C&& o)       : s(o.s), b(o.b) { o.s = 0; o.b = nullptr; } // steal 'brains', 'o' becomes 'zombie'
  C& operator = (const C& o) {
     if(this != &o) {
       delete [] b;                               // delete our 'brains'
       s = o.s;
    // b = (T*) memcpy((void*) new T[s], o.b, s*sizeof(T)); // allocate + deep copy of 'brains' from 'o'  // c-style cast (yuk)
       b = reinterpret_cast<T*> (memcpy((void*) new T[s], o.b, s*sizeof(T))); // allocate + deep copy of 'brains' from 'o' 
     }
     return *this;
  }
  C& operator = (C&& o) {
     if(this != &o) {
       delete [] b;                               // delete our 'brains'
       s = o.s;                                   // steal 'brains'
       b = o.b;
       o.s = 0;                                   // turn 'o' into a 'zombie'
       o.b = nullptr;
     }
     return *this;
  }
};

int main()
{
  try { 
    std::cout << "rule of 5 std::string: start\n";
      std::string s1, s2(std::string(1000*1000*1000, '?')), s3(s2), s4(std::move(s3));
      s1 = s2;
      s2 = std::move(s1);
    std::cout << "rule of 5 std::string: stop\n\n";
  } catch(std::exception& e) {
    std::cerr << "It threw! --> " << e.what() << "\n";
  }

  try {
    std::cout << "rule of 5 class A: start\n";
      A a1, a2(std::string(1000*1000*1000, '?')), a3(a2), a4(std::move(a3));
      a1 = a2;
      a2 = std::move(a1);
    std::cout << "rule of 5 class A: stop\n\n";
  } catch(std::exception& e) {
    std::cerr << "It threw! --> " << e.what() << "\n";
  }

  try {
    std::cout << "rule of 5 std::wstring: start\n";
      std::wstring w1, w2=std::wstring(1000*1000, '?'), w3(w2), w4(std::move(w3));
      w1 = w2;
      w2 = std::move(w1);
    std::cout << "rule of 5 std::wstring: stop\n\n";
  } catch(std::exception& e) {
    std::cerr << "It threw! --> " << e.what() << "\n";
  }

  try {
    std::cout << "rule of 5 class B: start\n";
      B b1, b2(std::wstring(1000*1000, '?')), b3(b2), b4(std::move(b3));
      b1 = b2;
      b2 = std::move(b1);
    std::cout << "rule of 5 class B: stop\n\n";
  } catch(std::exception& e) {
    std::cerr << "It threw! --> " << e.what() << "\n";
  }

  try {
    std::cout << "rule of 5 template class C: start\n";
      C<uint64_t> c1, c2(125*1000*1000), c3(c2), c4(std::move(c3));
      c1 = c2;
      c2 = std::move(c1);
    std::cout << "rule of 5 template class C: stop\n";
  } catch(std::exception& e) {
    std::cerr << "It threw! --> " << e.what() << "\n";
  }
}
