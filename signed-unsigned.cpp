#include <cinttypes>
#include <iostream>
using namespace std;

template <class T>
T setBit(T target, int bit)
{
    T mask = (1 << bit);
    return target | mask;
}

template <class T>
T clearBit(T target, int bit)
{
    T mask = (1 << bit);
    return target & (~mask);
}

template <class T>
bool testBit(T target, int bit)
{
    T mask = (1 << bit);
    return target & mask;
}

template <class T>
void printBits(T target)
{
    for(int bit = 8 * sizeof(T) -1; bit >=0 ; bit--) {
        cout << testBit(target,bit);
    }
}


int main()
{
   uint16_t ui = 32768;
   int16_t  i = ui;;


  cout << ui << "\n";
  cout <<  i << "\n";

  printBits(ui);
  cout << "\n";
  printBits(i);
  cout << "\n";

  ui = 65535;
  i = ui;

  cout << ui << "\n";
  cout <<  i << "\n";

  printBits(ui);
  cout << "\n";
  printBits(i);
  cout << "\n";
}


