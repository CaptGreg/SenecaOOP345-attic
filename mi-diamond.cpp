// #define PROBLEM
#define FIX

#ifdef PROBLEM
#include <iostream>
using namespace std;

class Base {
    protected:
        int iData;
    public:
        Base() { iData = 10; }
};

class Derived1 : public Base { };
class Derived2 : public Base { };

class Derived3 : public Derived1, public Derived2 {
    public: 
        int GetData() { return iData; } 
};

int main (int argc, char**argv)
{
    cout << endl << argv[0] << endl << endl;
    Derived3 obj;
    cout << obj.GetData() << endl;
}
#endif


#ifdef FIX
// EXAMPLE: Demonstrate the usage of virtual base class in the Diamond problem to fix the compilation error

#include <iostream> 
using namespace std; 

class Base { 
    protected: 
        int iData; 
    public:
        Base() { cout <<"Base ctor\n"; iData = 10; }
};

class Derived1 : virtual public Base { 
        public: Derived1() { cout <<"Derived1 ctor\n"; }
};
class Derived2 : virtual public Base { 
        public: Derived2() { cout <<"Derived2 ctor\n"; }
};

class Derived3 : public Derived1, public Derived2 {
    public: Derived3() { cout <<"Derived3 ctor\n"; }
    public:
      int GetData() { return iData; } 
};

int main (int argc, char**argv)
{
    cout << endl << argv[0] << endl << endl;

    Derived3 obj;
    cout << "base class iData value-" << obj.GetData() << endl;
}
#endif
