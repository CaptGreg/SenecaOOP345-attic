// clang++ -std=c++11 vectorchar.cpp -o vectorchar
// g++     -std=c++11 vectorchar.cpp -o vectorchar

#include <iostream>
#include <string>

using namespace std;


int main(int argc, char**argv)
{
  cout << argv[0] << "\n";
  cout << "\n+++++++++++++++++++++++++++++ string\n";

  string s1("'char* string 1'");
  string s2("'char* string 2'");

  string s3 = s1 + ' ' + s2;
  cout<<"s1: "; cout << s1 << "\n";
  cout<<"s2: "; cout << s2 << "\n";
  cout<<"s3: "; cout << s3 << "\n";

  cout << "\n+++++++++++++++++++++++++++++\n";
}
