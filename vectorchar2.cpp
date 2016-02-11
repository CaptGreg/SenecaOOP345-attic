// clang++ -std=c++11 vectorchar.cpp -o vectorchar
// g++     -std=c++11 vectorchar.cpp -o vectorchar

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// string and vector share a lot in common.
// Vector does not have any overloaded operators such as the usual <<, +, and +=


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

  cout << "\n+++++++++++++++++++++++++++++ vector\n";

  // Cannot do 'vector<char> v("string");'
  // "string" is char* and vector<char> expects a single char.
  
  vector<char> v1; for(auto c:"'vector string 1'") if(c) v1.push_back(c); // ignore '\0'
  vector<char> v2; for(auto c:"'vector string 2'") if(c) v2.emplace_back(c); // or emplace_back

  // No vector 'operator+' so cannot do: vector<char> v3 = v1 + ' ' + v2;
  vector<char> v3 = v1; v3.push_back(' '); for(char c: v2) v3.push_back(c);

  // No vector 'operator<<' so cannot do: cout << v1 << "\n";
  cout<<"v1: size=" << v1.size() << ": "; for(char c: v1) cout << c; cout << "\n";
  cout<<"v2: size=" << v2.size() << ": "; for(char c: v2) cout << c; cout << "\n";
  cout<<"v3: size=" << v3.size() << ": "; for(char c: v3) cout << c; cout << "\n";

  cout << "\n+++++++++++++++++++++++++++++\n";
}
