#include <iostream>
#include <string>
#include <functional>
#include <typeinfo>     // typeid
// #define TYPEID(X) std::cout << "typeid(" #X ").name()=" << std::typeid(X).name() << "\n"; // error
#define TYPEID(X) // cout << "typeid(" #X ").name()=" << typeid(X).name() << "\n";  // compiles without std::

using namespace std;


string& funcA(const string& arg) { return *new string("funcA:" + arg + "\n"); }
string& funcB(const string& arg) { return *new string("funcB:" + arg + "\n"); }
string& funcC(const string& arg) { return *new string("funcC:" + arg + "\n"); }

int main(int argc, char**argv)
{
  cout << funcA("calling funcA in line " + to_string(__LINE__) );
  cout << funcB("calling funcB in line " + to_string(__LINE__) );
  cout << funcC("calling funcC in line " + to_string(__LINE__) );

  TYPEID(funcA);
  TYPEID(funcB);
  TYPEID(funcC);

  string& (*func) (const string& arg);   // 1977 c-code
  TYPEID(func);

  func = funcC;
  cout << func("calling function pointer 'func' in line " + to_string(__LINE__) );

  func = funcB;
  cout << func("calling function pointer 'func' in line " + to_string(__LINE__) );

  func = funcA;
  cout << func("calling function pointer 'func' in line " + to_string(__LINE__) );

  function < string& (const string&) >  funcpp;  // 1984? or later C++
  TYPEID(funcpp);

  funcpp = funcA;
  cout << funcpp("calling C++ function pointer 'funcpp' in line " + to_string(__LINE__) );

  funcpp = funcB;
  cout << funcpp("calling C++ function pointer 'funcpp' in line " + to_string(__LINE__) );

  funcpp = funcC;
  cout << funcpp("calling C++ function pointer 'funcpp' in line " + to_string(__LINE__) );

  class MFO { // My Function Object - 'Functor'
  public:
    string& operator() (const string& arg) { return *new string( "instance of class MFO:" + arg + "\n"); }
  };
  TYPEID(MFO);

  MFO mfo;
  TYPEID(mfo);

  cout << mfo("calling C++ function pointer 'instance mfo of class MFO' in line " + to_string(__LINE__) );

class MFO2 { // My Function Object - 'Functor'
    int n;
  public:
    MFO2(int num) : n(num) {}
    int operator() (const int arg) { return arg * n; }
  };

  MFO2 mfo2(argc);
  cout << "mfo2(__LINE__) returns argc*__LINE__:" << mfo2(__LINE__) << "\n";
  

  // lambda
  // string& funcLambda = [] (const string& arg) { return * new string( "funcC:" + arg + "\n"); };
  // compiler does not approve of our return type.
  // OK, let compiler figure it out, use 'auto'
  auto funcLambda = [] (const string& arg) { return * new string("funcLambda:" + arg + "\n"); };
  TYPEID(funcLambda);
  cout << funcLambda("calling C++ lambda function in line " + to_string(__LINE__) );

  auto funcLambda3 = [argc] (const int arg) -> int { return argc * arg; };
  cout << "funclambda3 capturing [argc] " << funcLambda3(99) << "\n"
;

  auto funcLambda2 = [] (const string& arg) -> string& { return * new string( "funcLambda2:" + arg + "\n"); };
  // cout << funcLambda2("calling C++ lambda function (auto ... ->string& ...) in line " + to_string(__LINE__) );
  cout << funcLambda2("calling C++ lambda function (auto ... ->string& ...) in line " + to_string(__LINE__) );

  cout << R"abc(
+++++++++++++++++++++++++++++++++++++
  Robots and aliens commonly pass lambdas as one-liners to other function that 
  can only be understood by other robots and other aliens.

  Can you easily understand this one-liner?
  
  cout << [] (const string& arg) -> string& { return * new string( "inlined lambda:" + arg + "\n"); }("calling C++ inlined lambda function (auto ... ->string& ...) in line " + to_string(__LINE__) );
          ^---------------------------------------- lambda definition ------------------------------^^------------------------------- calling argument ---------------------------------------------^

  or do you prefer this one line definition:

  auto funcLambda2 = [] (const string& arg) -> string& { return * new string( string("funcLambda2:") + arg + string("\n")); };

  or the same thing as a two line definition:

  auto funcLambda2 = [] (const string& arg) -> string& 
       { return * new string( string("funcLambda2:") + arg + string("\n")); };

  then use it:

  cout << funcLambda2("calling C++ lambda function (auto ... ->string& ...) in line " + to_string(__LINE__) );
          ^^^^^^^^^^^

  OR even better:

  string s =  "calling C++ inlined lambda function (auto ... ->string& ...) in line " + to_string(__LINE__);
  cout << funcLambda2(s);
+++++++++++++++++++++++++++++++++++++
  )abc";

  cout << [] (const string& arg) -> string& { return * new string( "inlined lambda:" + arg + "\n"); }("calling C++ inlined lambda function (auto ... ->string& ...) in line " + to_string(__LINE__) );

  string s =  "calling C++ inlined lambda function (auto ... ->string& ...) in line " + to_string(__LINE__);
  cout << funcLambda2(s);
}
