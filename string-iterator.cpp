#include <string>
#include <iostream>
using namespace std;

int main()
{
  string s("ABCDEFG");

  cout << "i = 0; i < s.size(); i++ s[i]: ";
  for(int i = 0; i < s.size(); i++)
    cout << s[i];
  cout << "\n";

  cout << "i = 0; i < s.size(); i++ s.at(i): ";
  for(int i = 0; i < s.size(); i++)
    cout << s.at(i);
  cout << "\n";

  cout << "char*p=s.c_str(); *p; p++: ";
  for(const char* p  =s.c_str(); *p ; p++)
    cout << *p;
  cout << "\n";

  cout << "iterator begin *it: ";
  for(string::iterator it = s.begin() ; *it; it++)
    cout << *it;
  cout << "\n";

  cout << "iterator begin end: ";
  for(string::iterator it = s.begin() ; it != s.end(); it++)
    cout << *it;
  cout << "\n";

  cout << "reverse iterator: ";
  for(string::reverse_iterator rit = s.rbegin() ; rit != s.rend(); rit++)
    cout << *rit;
  cout << "\n";

  cout << "range-based for: ";
  for(auto e : s)
    cout << e;
  cout << "\n";

}
