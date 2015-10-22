#include <iostream>
using namespace std;

int main()
{
   int data[] ={ 1,2,3,4,5 };

   // range-based for motivation:
   // the compiler often understands the number of elements in the array
   for(size_t i = 0; i < sizeof(data)/sizeof(data[0]); i++)
     cout << data[i] << " ";
   cout << "\n";

   // let the compiler generate code that loops through the array 
   // range-based for
   for(auto e : data)
     cout << e << " ";
   cout << "\n";

   for(int e : data)
     cout << e << " ";
   cout << "\n";

// #define SHOW_SYNTAX_ERRORS
#ifdef SHOW_SYNTAX_ERRORS

   // Can we initialize other things in a range-based for?  
   // It appears to be NO!

   for(bool breakFlag = false, auto e : data)
     cout << e << " ";
   cout << "\n";

   for(bool breakFlag = false, int e : data)
     cout << e << " ";
   cout << "\n";

   for(int e, bool breakFlag = false : data)
     cout << e << " ";
   cout << "\n";
 
   // Can we declare the element variable outside of the for loop?
   // It appears to be NO!
   int e;
   for(e : data)
     cout << e << " ";
   cout << "\n";

#endif
}
