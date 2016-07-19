#include <iostream>
#include <string>
#include <memory>     // smart pointers
#include <exception> 
using namespace std;

class X {
  string name;
  uint8_t memBlock[1024*1024];  // 1 MByte
public:
  X(const std::string& n) { name = n; cout << "X ctor " << name << "\n"; }
  ~X() { cout << "X dtor " << name << "\n"; }
  void hello() { cout << "hello from " << name << "\n\n"; }
};

std::unique_ptr<X> global_usp(new X("GLOBAL unique smart pointer"));
X* global_p = new X("GLOBAL pointer");


int main(int argc, char**argv)
{
  cout << "main starting...\n";

  try {
    X x("x");
    x.hello();

    X* p = new X("p");
    p->hello();

    std::unique_ptr<X> usp(new X("unique smart pointer"));
    usp->hello();

    std::shared_ptr<X> ssp(new X("shared smart pointer"));
    ssp->hello();
    cout << "ssp.use_count()=" << ssp.use_count() << "\n";

    std::shared_ptr<X> ssp2( ssp );    // copy ctor
    ssp2->hello();
    cout << "ssp.use_count ()=" << ssp.use_count()  << "\n";
    cout << "ssp2.use_count()=" << ssp2.use_count() << "\n";

    std::shared_ptr<X> ssp3;
    ssp3 =  ssp2;   // assignment operator
    ssp3->hello();
    cout << "ssp.use_count ()=" << ssp.use_count()  << "\n";
    cout << "ssp2.use_count()=" << ssp2.use_count() << "\n";
    cout << "ssp3.use_count()=" << ssp3.use_count() << "\n";

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
