#include <iostream>
using namespace std;

template <typename T>
void bitprint(T v, int bits)
{
  for(int i = bits-1 ; i >= 0; i--) {
    cout << (bool) (v & (1 << i));
  }
}


int main(int argc, char**argv)
{
  int bits = 2;
  if(argc > 1) bits = atoi(argv[1]);

  for(int i = 0; i < (1 << bits); i++) {
    for(int j = i; j < (1 << bits); j++) {

      cout << i << " " << j<< " ";

      bitprint(i,bits);
      cout <<" ";
      bitprint(j,bits);

      cout << " |";
      bitprint(i|j,bits);

      cout << " &";
      bitprint(i&j,bits);

      cout << " ^";
      bitprint(i^j,bits);

      cout << "\n";
    }
  }
}
