#include <iostream>
#include <string>
#include <exception>  // throw std::bad_alloc
using namespace std;

class X {
  const static size_t SIZE = 375 * 1000 * 1000; // 375 million doubles * sizeof(double) = 3 GByte
  double *trouble;
public:
 X()                      { trouble = new double[SIZE]; print("ctor"); }  // ctor
 ~X()                     { print("dtor"); if(trouble) delete [] trouble;}  // dtor
 void print(string label) { cout << label << ": this/trouble=" << (void*) this << "/" << (void*) trouble << "\n"; }
 X(const X&  rhs) = delete;
 X(      X&& rhs) { // move ctor C++11
   cout<<"X move ctor (move rhs pointer)\n"; 
   trouble = nullptr;
   *this = std::move(rhs);  // let move assignment operator do the work.
 }
 X&  operator= (const X&  rhs)  = delete;
 X&& operator= (      X&& rhs) {  // move assignment operator C++11
   cout << "move assignment operator\n";  
   if(this != &rhs) {
     if(trouble) { delete [] trouble; }
     trouble = rhs.trouble;          rhs.trouble = nullptr;
   } else {
     cout << "somebody called move assignment operator with same value, eg x=std::move(x)\n";
   }
   return std::move(*this);
 }
 
};

int main(int argc, char** argv)
{
  try {

    X hotPotato1;                        // "I just made one."
      hotPotato1.print("hotPotato1");
    X hotPotato2(std::move(hotPotato1)); // "I'm new.  It is my turn.  Give it to me."
      hotPotato1.print("hotPotato1");
      hotPotato2.print("hotPotato2");
    X hotPotato3(std::move(hotPotato2)); // "I'm new.  It is my turn.  Give it to me."
      hotPotato1.print("hotPotato1");
      hotPotato2.print("hotPotato2");
      hotPotato3.print("hotPotato3");
    X hotPotato4(std::move(hotPotato3)); // "I'm new.  It is my turn.  Give it to me."
      hotPotato1.print("hotPotato1");
      hotPotato2.print("hotPotato2");
      hotPotato3.print("hotPotato3");
      hotPotato4.print("hotPotato4");

    hotPotato1 = std::move(hotPotato4);  // "I want it."
      hotPotato1.print("hotPotato1");
      hotPotato2.print("hotPotato2");
      hotPotato3.print("hotPotato3");
      hotPotato4.print("hotPotato4");
    hotPotato2 = std::move(hotPotato1);  // "I want it."
      hotPotato1.print("hotPotato1");
      hotPotato2.print("hotPotato2");
      hotPotato3.print("hotPotato3");
      hotPotato4.print("hotPotato4");
    hotPotato3 = std::move(hotPotato2);  // "I want it."
      hotPotato1.print("hotPotato1");
      hotPotato2.print("hotPotato2");
      hotPotato3.print("hotPotato3");
      hotPotato4.print("hotPotato4");
    hotPotato4 = std::move(hotPotato3);  // "I want it."
      hotPotato1.print("hotPotato1");
      hotPotato2.print("hotPotato2");
      hotPotato3.print("hotPotato3");
      hotPotato4.print("hotPotato4");
    hotPotato1 = std::move(hotPotato2);  // "I want it, but 2 does not have it!"

  } catch(exception& e) {
    cerr << "It threw! :-( " << e.what() << "\n";
  }
  return 0;
}
