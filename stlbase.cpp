
#include <vector>
#include <iostream>
#include <cstdint>  // intptr_t

using namespace std;

class myVec : public vector<int> {
public:
  void operator+=(int i) { push_back(i); }
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
}
