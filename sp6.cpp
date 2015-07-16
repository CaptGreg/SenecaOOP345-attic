#include <iostream>
#include <string>
#include <memory>     // smart pointers
#include <exception> 
using namespace std;

class X {
  string name;
  uint8_t memBlock[1024*1024];  // 1 MByte
public:
  X(const std::string& n) { name = n; cout << "X ctor " << name << " this=" << this << "\n"; }
  ~X() { cout << "X dtor " << name << " this=" << this << "\n"; }
};

X xbeforeMain("X before main");

int main(int argc, char**argv)
{
  cout << "main starting...\n";

  try {
    X x("x");

    X* rp = new X("raw ptr");

    std::unique_ptr<X> usp(new X("unique smart pointer"));
    cout << "usp.get()=" << usp.get() << "\n";
    cout << "\n";

    std::shared_ptr<X> ssp(new X("shared smart pointer"));
    cout << "ssp.use_count()=" << ssp.use_count() << "\n";
    cout << "ssp.get()=" << ssp.get() << "\n";
    cout << "\n";

    std::shared_ptr<X> ssp2( ssp );    // copy ctor
    cout << "ssp.use_count()=" << ssp.use_count()  << "\n";
    cout << "ssp.get()==" << ssp.get()  << "\n";
    cout << "ssp2.use_count()=" << ssp2.use_count() << "\n";
    cout << "ssp2.get()=" << ssp2.get() << "\n";
    cout << "\n";

    std::shared_ptr<X> ssp3 =  ssp2;   // assignment operator
    cout << "ssp.use_count()=" << ssp.use_count()  << "\n";
    cout << "ssp.get()=" << ssp.get()  << "\n";
    cout << "ssp2.use_count()=" << ssp2.use_count() << "\n";
    cout << "ssp2.get()=" << ssp2.get() << "\n";
    cout << "ssp3.use_count()=" << ssp3.use_count() << "\n";
    cout << "ssp3.get()=" << ssp3.get() << "\n";
    cout << "\n";

    if(argc > 2)  {
       cout << "argc > 2, return\n";
       return 0;
    } else
    if(argc > 1)  {
       cout << "argc > 1, we are throwing\n";
       throw string("we are throwing");
    }
    delete rp;
  } catch (const string& e){
    cout << "It threw a string: " << e << "\n";
  }

  cout << "main terminating...\n";
}

X xafterMain("X after main");
