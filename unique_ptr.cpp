// copy ctor, assignment operators with a unique_ptr data member

#include <memory>      // std::unique_ptr
#include <cstring>     // memcpy
#include <iostream>
#include <exception>

class X {
    std::unique_ptr<uint8_t> data; 
    static const size_t SIZE = 1LL * 1024LL * 1024LL * 1024LL;  // 1 GB
public:
    X()                          // constructor
    { 
      std::cout << "   X ctor allocating " << SIZE << " bytes\n";
      data = std::unique_ptr<uint8_t> (new uint8_t[SIZE]);
    }
    ~X()                         // destructor
    { 
        std::cout << "   X dtor";
        if(data) {
           std::cout <<  " releasing " << SIZE << " bytes\n";
           data.reset(nullptr);
        } else {
           std::cout <<  " ZOMBIE\n";
        }
    }
    X(X &x) //copy constructor                         // deep copy
    { 
        std::cout << "   X copy ctor allocate and deep copy of " << SIZE << " bytes\n";
        data = std::unique_ptr<uint8_t> (new uint8_t[SIZE]);
        memcpy( (void*) data.get(),  (void*) x.data.get(), SIZE * sizeof(*data));
    }
    X& operator=(const X &x) // copy assignment operator // deep copy
    { 
        std::cout << "   X assignment operator deep copy of " << SIZE << " bytes\n";
        // no need to de-allocate and immediately re-allocate the same size buffer
        memcpy( (void*) data.get(),  (void*) x.data.get(), SIZE * sizeof(*data));
        return *this;
    }
    X(X &&x) : data(std::move(x.data)) // move constructor      
    { 
        std::cout << "   X move ctor\n";
    }
    X& operator=(X &&x)                // move assignment operator
    { 
        std::cout << "   X move assignment operator\n";
        data = std::move(x.data);
        return *this;
    }
    uint8_t get(size_t i)            { return *(data.get()+i);     }
    void    set(size_t i, uint8_t v) {        *(data.get()+i) = v; }
};


int main()
{
  std::cout << "start-of-main\n";

  try {
    std::cout << "X x;\n";
    X x; 
    x.set(1000,42);

    std::cout << "X xx(x);\n";
    X xx(x);  
    std::cout << "xx(1000) = " << xx.get(1000) << "\n";

    std::cout << "x = xx;\n";
    x = xx;
    std::cout << "x(1000) = " << x.get(1000) << "\n";
  
    std::cout << "X xxx(std::move(x));\n";
    X xxx(std::move(x));
    std::cout << "xxx(1000) = " << xxx.get(1000) << "\n";

    std::cout << "xx = std::move(xxx);\n";
    xx = std::move(xxx);
    std::cout << "xx(1000) = " << xx.get(1000) << "\n";

    std::cout << "end-of-try-catch block\n";

  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }

  std::cout << "end-of-main\n";
}
