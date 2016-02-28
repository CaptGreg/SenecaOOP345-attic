// for a mixed assmbler c++ code listing
// g++ -c -g -Wa,-alh unsigned-warning.cpp -o unsigned-warning.o > unsigned-warning.s

#include <iostream>
using namespace std;

template <class T>
void printBits(T target)
{
    for(int bit = 8 * sizeof(T) -1; bit >=0 ; bit--)
        cout << (bool) (target & (0x1 << bit));
}


int main(int argc,char** argv)
{
  {
    unsigned u = 0;
    int      s = -1;

    cout << "\nunsigned + int:\n";
    if(s > u) 
      cout << s << " is > " << u << "\n";
    else 
      cout << s << " is NOT > " << u << "\n";

    cout << "u="; printBits(u); cout << "\n";
    cout << "s="; printBits(s); cout << "\n";
  }

  cout << "\nJust when we thought it was safe to let the kids out,\n";
  cout << "Just when we thought we understood the evil of comparing signed and unsigned numbers, we find:\n";
  {
    unsigned short u = 0;
    short          s = -1;

    cout << "\nunsigned short + short:\n";
    if(s > u) 
      cout << s << " is > " << u << "\n";
    else 
      cout << s << " is NOT > " << u << "\n";

    cout << "u="; printBits(u); cout << "\n";
    cout << "s="; printBits(s); cout << "\n";
  }

  {
    unsigned char u = 0;
    char          s = -1;

    cout << "\nunsigned char + char:\n";
    if(s > u) 
      cout << (int) s << " is > " << (int) u << "\n";
    else
      cout << (int) s << " is NOT > " << (int) u << "\n";
    cout << "u="; printBits(u); cout << "\n";
    cout << "s="; printBits(s); cout << "\n";

    if((unsigned) s > (unsigned) u) 
      cout << (unsigned) s << " is > " << (unsigned) u << "\n";
    else 
      cout << (unsigned) s << " is NOT > " << (unsigned) u << "\n";
    cout << "u="; printBits((unsigned) u); cout << "\n";
    cout << "s="; printBits((unsigned) s); cout << "\n";
  }

}
