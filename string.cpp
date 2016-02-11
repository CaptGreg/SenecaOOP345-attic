// http://www.tutorialspoint.com/cplusplus/cpp_quick_guide.htm

#include <iostream>
#include <string>
 
using namespace std;
 
int main ()
{
   string str1 = "Hello";
   string str2 = "World";
   string str3;
 
   // copy str1 into str3
   str3 = str1;
   cout << "str3 : " << str3 << "\n";
 
   // concatenates str1 and str2
   str3 = str1 + str2;
   cout << "str1 + str2 : " << str3 << "\n";
  
   return 0;
}
