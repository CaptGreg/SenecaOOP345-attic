#include <iostream>
using namespace std;

template <typename T>
void bitprint(T v, int bits = 8*sizeof(T))
{
  for(int i = bits-1 ; i >= 0; i--) { // bits numbered from zero to bits-1
    cout << (bool) (v & (1 << i));
  }
}


int main(int argc, char**argv)
{
  int bits = 2;
  if(argc > 1) bits = atoi(argv[1]);

  // test bitprint
  uint8_t  ones8  = ~0; bitprint(ones8);  cout << "\n";
  uint16_t ones16 = ~0; bitprint(ones16); cout << "\n";
//uint32_t ones32 = ~0; bitprint(ones32); cout << "\n";
//uint64_t ones64 = ~0; bitprint(ones64); cout << "\n";
  cout << "\n";

  cout << "\nboolean OR: bit on if either bit is on.\n";
  // print first row
    cout << "|";
    for(int i = 0; i <  bits; i++) {
      cout << " ";
    }
    for(int i = 0; i < (1 << bits); i++) {
      bitprint(i, bits);
      cout << " ";
    }
    cout <<"\n";
  // print table
  for(int i = 0; i < (1 << bits); i++) {
    bitprint(i, bits);
    cout << " ";
    for(int j = 0; j < (1 << bits); j++) {
      bitprint(i|j,bits);
      cout << " ";
    }
    cout <<"\n";
  }

  cout << "\nboolean AND: bit on if both bits are on.\n";
  // print first row
    cout << "&";
    for(int i = 0; i <  bits; i++) {
      cout << " ";
    }
    for(int i = 0; i < (1 << bits); i++) {
      bitprint(i, bits);
      cout << " ";
    }
    cout <<"\n";
  // print table
  for(int i = 0; i < (1 << bits); i++) {
    bitprint(i, bits);
    cout << " ";
    for(int j = 0; j < (1 << bits); j++) {
      bitprint(i&j,bits);
      cout << " ";
    }
    cout <<"\n";
  }

  cout << "\nboolean XOR: bit on if bits are different.\n";
  // print first row
    cout << "^";
    for(int i = 0; i <  bits; i++) {
      cout << " ";
    }
    for(int i = 0; i < (1 << bits); i++) {
      bitprint(i, bits);
      cout << " ";
    }
    cout <<"\n";
  // print table
  for(int i = 0; i < (1 << bits); i++) {
    bitprint(i, bits);
    cout << " ";
    for(int j = 0; j < (1 << bits); j++) {
      bitprint(i^j,bits);
      cout << " ";
    }
    cout <<"\n";
  }
}
