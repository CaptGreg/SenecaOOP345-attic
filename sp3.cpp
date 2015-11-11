//  PtrManager

#include <iostream>
using namespace std;

template <typename T>
class PtrManager
{
private:
    T*    pData;
public:
    PtrManager(T* pValue) : pData(pValue) { }        // constructor
    ~PtrManager()     { delete  pData; }             // destructor
    T& operator*   () { return *pData; }             // operator*  overload
    T* operator->  () { return  pData; }             // operator-> overload
};

class X {
  string name;
  uint8_t memBlock[1024*1024];  // 1 MByte
public:
  X(const std::string& n) { name = n; cout << "X ctor " << name << "\n"; }
  ~X() { cout << "X dtor " << name << "\n"; }
  void hello() { cout << "hello from " << name << "\n\n"; }
};


int main(int argc, char**argv)
{
  cout << "main starting...\n";

  try {
    X x("x");
    x.hello();

    X* p = new X("p");
    p->hello();

    PtrManager<X> p2(new X("p2 destructor-delete managed pointer"));
    p2->hello();

    if(argc > 2)  {
       cout << "argc > 2, return\n";
       return 0;
    } else
    if(argc > 1)  {
       cout << "argc > 1, we are throwing\n";
       throw string("we are throwing");
    }

    delete p;
  } catch (const std::exception& e){
    cout << "It threw a std::exception: " << e.what() << "\n";
  } catch (const string& e){
    cout << "It threw a string: " << e << "\n";
  }

  cout << "main terminating...\n";
}
