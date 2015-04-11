#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <cstdlib>          // on_exit()

using namespace std;

void exitCallback(int exitValue, void *dummy)
{
	cout << "exit call back called with exitvalue=" << exitValue << "\n";
}

int main(int argc, char**) 
{
   int dummy;
   on_exit(exitCallback, &dummy);

   try { throw "hi there"; }
   catch(const exception& e) { cout << "somebody threw 'exception' =" << e.what() << "\n"; }
   catch(const string& e) { cout << "somebody threw 'const string'&=" << e << "\n"; }
   catch(const char* e) { cout << "somebody threw 'const char*'=" << e << "\n"; }
   catch(...) { cout << "somebody threw ...\n"; }

   try { throw string("hi there"); }
   catch(const string& e) { cout << "somebody threw 'const string'&=" << e << "\n"; }
   catch(const char* e) { cout << "somebody threw 'const char*'=" << e << "\n"; }
   catch(...) { cout << "somebody threw ...\n"; }

   try {
     stringstream ss;
     ss << "hi there - this is a stringstream";
     throw ss.str();
   }
   catch(const exception& e) { cout << "somebody threw 'exception' =" << e.what() << "\n"; }
   catch(const string& e) { cout << "somebody threw 'const string'&=" << e << "\n"; }
   catch(const char* e) { cout << "somebody threw 'const char*'=" << e << "\n"; }
   catch(...) { cout << "somebody threw ...\n"; }

   try {
     cout << "throwing an int without an int catcher\n";
     throw 42;
   }
   catch(const exception& e) { cout << "somebody threw 'exception' =" << e.what() << "\n"; }
   catch(const string& e) { cout << "somebody threw 'const string'&=" << e << "\n"; }
   catch(const char* e) { cout << "somebody threw 'const char*'=" << e << "\n"; }
   catch(...) { cout << "somebody threw ...\n"; }

   try {
      cout << "will now 'throw nullptr'\n";
     throw nullptr;   // catch(...) will catch this one 
   }
   catch(const exception& e) { cout << "somebody threw 'exception' =" << e.what() << "\n"; }
   catch(const string& e) { cout << "somebody threw 'const string'&=" << e << "\n"; }
   catch(const char* e) { cout << "somebody threw 'const char*'=" << e << "\n"; }
   catch(...) { cout << "somebody threw ...\n"; }

   try {
      cout << "will now 'throw'\n";
      throw;   // fails with no catcher :-(
              // terminate called without an active exception
              // /bin/bash: line 1:  3322 Aborted                 (core dumped) ./trythrowcatch
   }
   catch(const exception& e) { cout << "somebody threw 'exception' =" << e.what() << "\n"; }
   catch(const string& e) { cout << "somebody threw 'const string'&=" << e << "\n"; }
   catch(const char* e) { cout << "somebody threw 'const char*'=" << e << "\n"; }
   catch(...) { cout << "somebody threw ...\n"; }
}
