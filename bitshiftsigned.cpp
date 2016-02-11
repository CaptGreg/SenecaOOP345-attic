#include <cinttypes>
#include <iostream>
using namespace std;

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

int main(int argc, char ** argv, char ** env)
{
    for(int bit = 0; bit < 8; bit++ ) {
        unsigned char  target  = 0x80;
        target >>= bit;
        cout <<"right shift unsigned 0x80 " << bit << " bits right =" ;
	printBits(target); 
	cout << " target=" << (unsigned int)(target & 0xff) <<  "\n";
    }
    cout << "\n";

    for(int bit = 0; bit < 8; bit++ ) {
        char  target  = 0x80;
        target >>= bit;
        cout <<"right shift signed 0x80 " << bit << " bits right =" ;
	printBits(target); 
	cout << " target=" << (int)(target) <<  "\n";
    }
}
