#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char**) 
{
   try {
     throw "hi there";
   }
   catch(const string& e) {
     cout << "somebody threw 'const string'&=" << e << "\n";
   }
   catch(const char* e) {
     cout << "somebody threw 'const char*'=" << e << "\n";
   }
   catch(...) {
     cout << "somebody threw ...\n";
   }

   try {
     throw string("hi there");
   }
   catch(const string& e) {
     cout << "somebody threw 'const string'&=" << e << "\n";
   }
   catch(const char* e) {
     cout << "somebody threw 'const char*'=" << e << "\n";
   }
   catch(...) {
     cout << "somebody threw ...\n";
   }


   try {
     stringstream ss;
     ss << "hi there - this is a stringstream";
     throw ss.str();
   }
   catch(const string& e) {
     cout << "somebody threw 'const string'&=" << e << "\n";
   }
   catch(const char* e) {
     cout << "somebody threw 'const char*'=" << e << "\n";
   }
   catch(...) {
     cout << "somebody threw ...\n";
   }
}
