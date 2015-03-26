#include <iostream>

using namespace std;

void print(int&  i) { cout << i << " is an l-value\n"; }
void print(int&& i) { cout << i << " is an r-value\n"; }

int main(int argc, char**argv)
{

  print(6);
  print(9);
  print(9 + 6);

  int six  = 6;
  int nine = 9;

  print(six);
  print(nine);
  print(nine + six);

  return 0;

}
