#include <iostream>
using namespace std;

int main()
{
  int j = 99; 
  j = j++;  // warning: operation on ‘j’ may be undefined [-Wsequence-point]

  // j++ = j;  // error: lvalue required as left operand of assignment
  // j++ = 7;  // error: lvalue required as left operand of assignment

  cout << "j=" << j << "\n";

  char data[] = "asdfghjkl";
  char*p = data;
  *p++ = 'X';
  cout << p << "\n";
  cout << data << "\n";

  cout << R"abc(cout << (1,2) << "\n"; prints )abc";
  cout << (1,2) << "\n";

  int x = (1,2);
  cout << "x=" << x << "\n";
  x = x++;
  cout << "x=" << x << "\n";

  for(int i = (1,22,3,4,5,6,0) ; i < (99,33,44,55,10); i += (2,3,4,6,1,2,3,11,1)) {
    cout << "i=" << i << "\n";
  }
}
