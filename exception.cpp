 
#include <iostream>
#include <string>
using namespace std;

int sub(int argc, char**argv)
{
  try {

    if(argc > 1)throw string("argc > 1");

    else throw "argc not > 1";

  } catch(const string& e){

    cout << "sub: threw a string: " << e << "\n";

  } catch(const char* e){

    cout << "sub: threw a char*: " << e << "\n";

  }
}

int main(int argc, char**argv)
{
   sub(argc, argv);
}
