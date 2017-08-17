#include <iostream>
#include <iomanip>
using namespace std;

template <typename T>
void printBits(T t, int bits=8*sizeof(T))
{
  for(int bit = bits-1; bit >= 0; bit--) {
#if 0
    cout << (bool) (t & (1 << bit));
#else
    cout << (bool) ((t >> bit)&1);
#endif
  }
}

int main()
{
 for(int16_t i = 0; i <= 32; i++) {
   cout << "i=" << setw(2) << i;
   cout <<" "; printBits(i); cout<<" ";
   int16_t j;

   j = -i;
   cout << "    -i=" << setw(3) << j;
   cout <<" "; printBits(j); cout<<" ";

   j = ~i;
   cout << "    ~i=" << setw(3) << j;
   cout <<" "; printBits(j); cout<<" ";

   j = (~i) + 1;
   cout << "    (~i) + 1 =" << setw(3) << j;
   cout <<" "; printBits(j); cout<<" ";

   cout << "\n";
 }

 return EXIT_SUCCESS;
}
