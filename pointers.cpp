#include <iostream>
using namespace std;

int main(int argc, char**argv)
{
   char str[] = "12345678";
   // int str[] = {1,2,3,4,5,6,7,8,9};

   // cout << "str = " << str << '\n';   // print char's
   for(auto i : str) cout << i; cout << '\n';

   char *p  = str;
   char *p1 = str;
   // int *p  = str;
   // int *p1 = str;

   cout << "*p = " << *p << '\n';

   cout << "++(*p) = " << ++(*p) << '\n';
   // cout << "str = " << str << '\n';   // print char's
   for(auto i : str) cout << i; cout << '\n';

   cout << "*p = " << *p << '\n';
   // cout << "str = " << str << '\n';   // print char's
   for(auto i : str) cout << i; cout << '\n';

   p = str+3;

   p = &str[3];

   cout << "--(*p) = " << --(*p) << '\n';
   cout << "*p = " << *p << '\n';
   // cout << "str = " << str << '\n';   // print char's
   for(auto i : str) cout << i; cout << '\n';
}
