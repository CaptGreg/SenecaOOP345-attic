// copy ctor, assignment operators with a unique_ptr data member

#include <memory>      // unique_ptr
#include <cstring>     // memcpy
#include <iostream>
#include <exception>

using namespace std;

class X {
    unique_ptr<uint8_t> data; 
    static const size_t SIZE = 1LL * 1000LL * 1000LL * 1000LL;  // 1e9 (GB)
public:
    X()                          // constructor
    { 
      cout << "   X ctor allocating " << SIZE << " bytes\n";
      data = unique_ptr<uint8_t> (new uint8_t[SIZE]);
    }
    ~X()                         // destructor
    { 
        cout << "   X dtor";
        if(data) {
           cout <<  " releasing " << SIZE << " bytes\n";
           data.reset(nullptr);  
        } else {
           cout <<  " ZOMBIE\n";
        }
    }
    X(X &x) //copy constructor                         // deep copy
    { 
        cout << "   X copy ctor allocate and deep copy of " << SIZE << " bytes\n";
        data = unique_ptr<uint8_t> (new uint8_t[SIZE]);
        memcpy( (void*) data.get(),  (void*) x.data.get(), SIZE * sizeof(*data));
    }
    X& operator=(const X &x) // copy assignment operator // deep copy
    { 
        cout << "   X assignment operator deep copy of " << SIZE << " bytes\n";
        // no need to de-allocate and immediately re-allocate the same size buffer
        memcpy( (void*) data.get(),  (void*) x.data.get(), SIZE * sizeof(*data));
        return *this;
    }
    X(X &&x) : data(move(x.data)) // move constructor      
    { 
        cout << "   X move ctor\n";
    }
    X&& operator=(X &&x)                // move assignment operator
    { 
        cout << "   X move assignment operator\n";
        data = move(x.data);
        x.data.reset(nullptr);  
        return move( *this );
    }
    uint8_t get(size_t i)            { return *(data.get()+i);     }
    void    set(size_t i, uint8_t v) {        *(data.get()+i) = v; }
};


int main()
{
  cout << "start-of-main\n";
  atexit([] () { cout << "atexit callback\n"; } );

  try {
    cout << "X x;\n";
    X x; 
    x.set(1000,42);

    cout << "X xx(x);\n";
    X xx(x);  
    cout << "xx(1000) = " << xx.get(1000) << " (ascii 42 is character '*')\n";

    cout << "x = xx;\n";
    x = xx;
    cout << "x(1000) = " << x.get(1000) << " (ascii 42 is character '*')\n";
  
    cout << "X xxx(move(x));\n";
    X xxx(move(x));
    cout << "xxx(1000) = " << xxx.get(1000) << " (ascii 42 is character '*')\n";

    cout << "xx = move(xxx);\n";
    xx = move(xxx);
    cout << "xx(1000) = " << xx.get(1000) << " (ascii 42 is character '*')\n";

    cout << "end-of-try-catch block\n";

  } catch (exception& e) {
    cerr << e.what() << "\n";
  }

  cout << "end-of-main\n";
}
