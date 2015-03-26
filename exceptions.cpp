// http://www.cplusplus.com/reference/exception/exception/

#include  <exception>   // std::exception
#include  <stdexcept>   // std::logic_error, std::runtime_error
#include  <typeinfo>    // std::bad_cast
#include  <future>      // std::future_error
#include  <iostream>    // std::cout
#include  <cstring>     // ::strcpy
#include  <string>      // std::string
using namespace std;

// From Chris Szalwinski's website on Standard Exceptions

// C++11 includes a standard library of exception classes.  The base class for the exception hierarchy is called exception and is defined in the header <exception>.  Classes derived from this base class include:

    // logic_error - handles problems in a program's internal logic, which in theory are preventable.  The following classes are derived from logic_error:
        // length_error
        // domain_error
        // out_of_range
        // invalid_argument
    // runtime_error - handles problems that can only be caught during execution.  The following classes are derived from runtime_error:
        // range_error
        // overflow_error
        // underflow_error
    // bad_alloc - handles the allocation exception thrown by new.  This class needs the header <new>
    // bad_cast - handles the exception thrown by dynamic_cast.  This class needs the header <typeinfo>

// In handling objects from derived classes, it is important to catch the most derived types first.  For example, the following code catches a std::bad_alloc exception before handling any other exceptions:

int main(int argc,char**argv)
{
     const char *s = "qwertyuio";

     try {
         char* p = new char[::strlen(s) + 1];
         ::strcpy(p, s);
 
         int size = 100 * 1000 * 1000 * 1000 ;   // this is not 100 Gig
         cout << "size = " << size << "\n";
         // size = 1,215,752,192  or 1.2 Gig
         char *ptr = new char[size];
         if(!ptr) throw  ":-( If size indeed was 100Gig, expected new to throw a std::bad_alloc and it DIDN'T!!!\n";
         if(ptr) delete [] ptr;

         size_t sizeBIG = 100LL * 1000LL * 1000LL * 1000LL ;
         cout << "sizeBIG = " << sizeBIG << "\n";
         char *ptrBIG = new char[sizeBIG];
         if(!ptrBIG) throw  ":-( size is 1e11, 100 gig, expected new to throw a std::bad_alloc and it DIDN'T!!!\n";
         if(ptrBIG) delete [] ptrBIG;
     } 
     catch (const char* e)             { cout << "threw const char* " << e << "\n"; }
     catch (std::domain_error)         { cout << "std::domain_error\n";}         // <stdexcept>
     catch (std::future_error)         { cout << "std::future_error\n";}         // <stdexcept>
     catch (std::invalid_argument)     { cout << "std::invalid_argument\n";}     // <stdexcept>
     catch (std::length_error)         { cout << "std::length_error\n";}         // <stdexcept>
     catch (std::out_of_range)         { cout << "std::out_of_range\n";}         // <stdexcept>
     catch (std::logic_error)          { cout << "std::logic_error\n";}          // <stdexcept>

     catch (std::overflow_error)       { cout << "std::overflow_error\n";}       // <stdexcept>
     catch (std::range_error)          { cout << "std::range_error\n";}          // <stdexcept>
//   catch (std::system_error)         { cout << "std::system_error\n";}         // <stdexcept>  C++11
     catch (std::underflow_error)      { cout << "std::underflow_error\n";}      // <stdexcept>
     catch (std::runtime_error)        { cout << "std::runtime_error\n";}        // <stdexcept>

     catch (std::bad_alloc)            { cout << "std::bad_alloc\n";}            // <stdexcept>
     catch (std::bad_array_new_length) { cout << "std::bad_array_new_length\n";} // <stdexcept>
//   catch (std::bad_cast)             { cout << "std::bad_cast\n";} 

     catch (std::exception& e)         { cout << "std::exception " << e.what() << "\n"; }

// The following handles all exceptions including the std:::bad_alloc exception under the first catch block

     try {
         char* p = new char[::strlen(s) + 1];
         ::strcpy(p, s);
     } catch (std::exception& e) { // called by std::bad_alloc also 
         cout << "Standard Exception " << e.what() << "\n";
     } catch (std::bad_alloc) { // UNREACHABLE!
         cout << "Insufficient memory\n";
     }
    
    char cstring[] = "qwertyuiop";
    for(int i = 0; i < 5 ; i++ )
         try {
             switch(i) {
	     case 0: throw 1234;
	     case 1: throw 99.999;
	     case 2: throw "howdy";
	     case 3: throw std::string("howdy doody");
	     case 4: throw cstring;
	     }
         } 
         catch (char* e) { std::cout << "threw char* " << e << "\n"; }
         catch (const char* e) { std::cout << "threw const char* " << e << "\n"; }
         catch (double e) { std::cout << "threw double " << e << "\n"; }
	 catch (int e) { std::cout << "threw int " << e << "\n"; }
         catch (std::string e) { std::cout << "threw std::string " << e << "\n"; }
}
