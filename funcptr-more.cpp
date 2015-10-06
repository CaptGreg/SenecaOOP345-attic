#include <iostream>
#include <string>
#include <functional>
#include <typeinfo>     // typeid
#define TYPEID(X) std::cout << "typeid(" #X ").name()=" << std::typeid(X).name() << "\n";

using namespace std;


string& funcA(const string& arg) { return * new string( string("funcA:") + arg + string("\n")); }
string& funcB(const string& arg) { return * new string( string("funcB:") + arg + string("\n")); }
string& funcC(const string& arg) { return * new string( string("funcC:") + arg + string("\n")); }

int main(int argc, char**argv)
{
  int i;
  // TYPEID(i);
  // std::cout << "typeid(" "i"  ").name()=" << std::typeid(i).name() << "\n";

  cout << funcA(string("calling funcA in line ") + to_string(__LINE__) );
  cout << funcB(string("calling funcB in line ") + to_string(__LINE__) );
  cout << funcC(string("calling funcC in line ") + to_string(__LINE__) );

  // TYPEID(funcA);
  // TYPEID(funcB);
  // TYPEID(funcC);

  string& (*func) (const string& arg);
  // TYPEID(func);

  func = funcC;
  cout << func(string("calling function pointer 'func' in line ") + to_string(__LINE__) );

  func = funcB;
  cout << func(string("calling function pointer 'func' in line ") + to_string(__LINE__) );

  func = funcA;
  cout << func(string("calling function pointer 'func' in line ") + to_string(__LINE__) );


  function < string& (const string&) >  funcpp;
  // TYPEID(funcpp);

  funcpp = funcA;
  cout << funcpp(string("calling C++ function pointer 'funcpp' in line ") + to_string(__LINE__) );

  funcpp = funcB;
  cout << funcpp(string("calling C++ function pointer 'funcpp' in line ") + to_string(__LINE__) );

  funcpp = funcC;
  cout << funcpp(string("calling C++ function pointer 'funcpp' in line ") + to_string(__LINE__) );


  class MFO { // My Function Object - 'Functor'
  public:
    string& operator() (const string& arg) { return * new string( string("instance of class MFO:") + arg + string("\n")); }
  };
  // TYPEID(MFO);

  MFO mfo;
  // TYPEID(mfO);

  cout << mfo(string("calling C++ function pointer 'instance mfo of class MFO' in line ") + to_string(__LINE__) );

  // lambda
  // string& funcLambda = [] (const string& arg) { return * new string( string("funcC:") + arg + string("\n")); };
  // compiler does not approve of our return type.
  // OK, let compiler figure it out, use 'auto'
  auto funcLambda = [] (const string& arg) { return * new string( string("funcLambda:") + arg + string("\n")); };
  // TYPEID(funcLambda);
  cout << funcLambda(string("calling C++ lambda function in line ") + to_string(__LINE__) );

  auto funcLambda2 = [] (const string& arg) -> string& { return * new string( string("funcLambda2:") + arg + string("\n")); };
  cout << funcLambda2(string("calling C++ lambda function (auto ... ->string& ...) in line ") + to_string(__LINE__) );

  cout << "\n";
  cout << "combining lambdas into one-liners is commonly practised by robost and aliens, but I can't read it!\n";
  cout << "Can you easily understand this?\n";
  
  cout << R"abc(cout << [] (const string& arg) -> string& { return * new string( string("inlined lanbda:") + arg + string("\n")); }(string("calling C++ inlined lambda function (auto ... ->string& ...) in line ") + to_string(__LINE__) );)abc" << "\n";
  cout << "\n";

  cout << R"abc(
  or do you prefer this:
  auto funcLambda2 = [] (const string& arg) -> string& { return * new string( string("funcLambda2:") + arg + string("\n")); };
  cout << funcLambda2(string("calling C++ lambda function (auto ... ->string& ...) in line ") + to_string(__LINE__) );
  )abc";
  cout << "\n";
  cout << [] (const string& arg) -> string& { return * new string( string("inlined lanbda:") + arg + string("\n")); }(string("calling C++ inlined lambda function (auto ... ->string& ...) in line ") + to_string(__LINE__) );


}

