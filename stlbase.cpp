#include <vector>
#include <iostream>
#include <cstdint>  // intptr_t

using namespace std;

class myVec : public vector<int> {
public:
  void operator+=(int i)         { push_back(i); }
  void rangebasedforPrint()      { for(auto e: *this) cout << e << " "; cout << "\n"; }
};

class myString : public vector<char> {
public:
  void operator+=(char c)        { push_back(c); }
  void operator+=(const char* p) { while(*p) push_back(*p++); }
  void rangebasedforPrint()      { for(auto e: *this) cout << e << " "; cout << "\n"; }
};

int main(int argc,char**)
{
  myVec v;
  v += 1;
  v += 2;
  v += 3;
  v.push_back(4);

  for(auto e : v) cout << e << " "; cout << "\n";

  v[2] = 99;
  for(auto e : v) cout << e << " "; cout << "\n";
  cout << "v.rangebasedforPrint()=";
  v.rangebasedforPrint();

  myString s;
  s += 'a';
  s += 'b';
  s += "AB";
  for(auto e : s) cout << e << " "; cout << "\n";
  for(auto e : s) cout << e;        cout << "\n";
  for(size_t i = 0; i < s.size(); i++)  cout << s[i];  cout << "\n";
  cout << "s.rangebasedforPrint()=";
  s.rangebasedforPrint();
}
