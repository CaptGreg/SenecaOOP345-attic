// clang++ -std=c++11 vectorchar.cpp -o vectorchar
// g++     -std=c++11 vectorchar.cpp -o vectorchar

#include <iostream>
#include <string>

using namespace std;


int main(int argc, char**argv)
{
  cout << argv[0] << endl;
  cout << "\n+++++++++++++++++++++++++++++ string\n";

  string s1("'char* string 1'");
  string s2("'char* string 2'");

  string s3 = s1 + ' ' + s2;
  cout<<"s1: "; cout << s1 << endl;
  cout<<"s2: "; cout << s2 << endl;
  cout<<"s3: "; cout << s3 << endl;

  cout << "\n+++++++++++++++++++++++++++++\n";
}
