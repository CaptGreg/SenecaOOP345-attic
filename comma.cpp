#include <iostream>
using namespace std;

int func(int& i)
{
  return i++;
}

int main()
{
  int i, j, k, l, m;

  j = -99;
  j = func(j);
  cout << "j = " << j << "\n";

  i = -1000,3,4,5,6,7,8,9,func(i);
  cout << "i = " << i << "\n";


  i,j = 1,2,3,4,5,6,7,8,9;
  cout << "i = " << i << "\n";
  cout << "j = " << j << "\n";

  i=j=k=l=m = -100;
  i,j,k,l,m = 3;     // '144 dudes say this sets i=j=k=l=m = 3; but does it?

  cout << "i = " << i << "\n";
  cout << "j = " << j << "\n";
  cout << "k = " << k << "\n";
  cout << "l = " << l << "\n";
  cout << "m = " << m << "\n";

  return 0;
}
