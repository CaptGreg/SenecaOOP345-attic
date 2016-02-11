// clang++ -std=c++11 vectorchar.cpp -o vectorchar
// g++     -std=c++11 vectorchar.cpp -o vectorchar

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// string and vector share a lot in common.
// Vector does not have any overloaded operators such as the usual <<, +, and +=

// But, we can make our own DIY <<, +, and += operators 
ostream& operator<<(ostream& os, vector<char>& v)                    // DIY <<
  { for(auto c: v) if(c) os << c; return os; }

vector<char>& operator+(vector<char>& lhs, const vector<char>& rhs)  // DIY +
  // {vector<char>* temp = new vector<char>; *temp = lhs; for(auto c:rhs) temp->push_back(c); return *temp; }               // range-based for
  {vector<char>* temp = new vector<char>; *temp = lhs; temp->insert( temp->end(), rhs.begin(), rhs.end() ); return *temp; } // iterators

vector<char>& operator+=(vector<char>& lhs, const vector<char>& rhs) // DIY +=
  // {for(auto c:rhs) lhs.push_back(c); return lhs; }             // range-based for
  {lhs.insert( lhs.end(), rhs.begin(), rhs.end() ); return lhs; } // iterators

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

  cout << "\n+++++++++++++++++++++++++++++ vector plus DIY <<, +, and += operators\n";

  // But we can use own DIY <<, +, and += operators 
  cout<<"v1 DIY operator<< " << v1 << "\n";
  cout<<"v2 DIY operator<< " << v2 << "\n";
  v3=v1+v2;      // DIY +
  cout<<"v3 DIY operator<< after DIY'+' v3=v1+v2; " << v3 << "<--NOTE: no space between v1 + v2" << "\n";
  v3=v1; v3+=v2; // DIY +=
  cout<<"v3 DIY operator<< after v3=v1; DIY'+=' v3+=v2; " << v3 << "<--NOTE: no space between v1 + v2" << "\n";

  cout << "\n+++++++++++++++++++++++++++++ string via std::vector operations\n";

  // Do the same thing using std::vector operations.
  string sv1; for(auto c:"'string via std::vector operations 1'") if(c) sv1.push_back(c); // ignore '\0'
  string sv2; for(auto c:"'string via std::vector operations 2'") if(c) sv2.push_back(c); // no string::emplace_back 

  string sv3 = sv1; sv3.push_back(' '); for(char c: sv2) sv3.push_back(c);

  cout<<"sv1: size=" << sv1.size() << ": "; for(char c: sv1) cout << c; cout << "\n";
  cout<<"sv2: size=" << sv2.size() << ": "; for(char c: sv2) cout << c; cout << "\n";
  cout<<"sv3: size=" << sv3.size() << ": "; for(char c: sv3) cout << c; cout << "\n";

  cout << "\n+++++++++++++++++++++++++++++\n";
}
