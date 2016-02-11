#include <iostream>
using namespace std;


class base1
{
  public:
     void some_function( )
     { cout << "base1 some_function\n";  }  
};

class base2
{
  public:
    void some_function( )
     { cout << "base2 some_function\n";  }  
};

class derived : public base1, public base2
{
    
};

int main (int argc, char**argv)
{
    cout << "\n" << argv[0] << "\n" << "\n";
    derived obj;
    
//  Error because compiler can't figure out which function to call either same_function( ) of base1 or base2 
    // obj.some_function( )  

    obj.base1::some_function( );  /* Function of class base1 is called. */
    obj.base2::some_function( );  /* Function of class base2 is called. */
}

