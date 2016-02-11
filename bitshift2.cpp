#include <iostream>
using namespace std;

unsigned int intSetBit(unsigned int target, int bit)
{
    unsigned int mask = (1 << bit);
    return target | mask;
}

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
    return target & ~mask;
}

template <class T>
bool testBit(T target, int bit)
{
    T mask = (1 << bit);
    return (target & mask) != 0;
}

template <class T>
void printBits(T target)
{
    for(int bit = 8 * sizeof(target) -1; bit >=0; bit-- ) 
        cout << testBit(target, bit);
}


int main(int argc, char ** argv, char ** env)
{
    for(int bit = 0; bit < 8; bit++ ) {
        unsigned int target  = 1;
        target = intSetBit(target, bit);
        cout <<"intSetBit: bit " << bit << " on, target = " << target << "\n";
    }

    unsigned short target;
    for(int bit = 0; bit < 8 * sizeof(target); bit++ ) {
        target  = 0;
        target = setBit(target, bit);
        cout <<"setBit: bit " << bit << " on, target = " << (unsigned long long) target << "\n";
    }

    for(int bit = 0; bit < 8 * sizeof(target); bit++ ) {
        target  = ~0;
        target = clearBit(target, bit);
        // cout <<"clearBit: bit " << bit << " is off, target = " << (long long) target << "\n";
        printBits(target);
	cout << "\n";

    }

    int A = 'a';
    int B = 72;

    cout << "A=" << A << "\n";
    cout << "B=" << B << "\n";

    cout << "A="; printBits(A); cout << "\n";
    cout << "B="; printBits(B); cout << "\n";

    A = A ^ B;
    cout << "after A = A ^ B;\n";
    cout << "A="; printBits(A); cout << "\n";
    cout << "B="; printBits(B); cout << "\n";
    

    B = A ^ B;
    cout << "after B = A ^ B;\n";
    cout << "A="; printBits(A); cout << "\n";
    cout << "B="; printBits(B); cout << "\n";

    A = A ^ B;
    cout << "after A = A ^ B;\n";
    cout << "A="; printBits(A); cout << "\n";
    cout << "B="; printBits(B); cout << "\n";

    cout << "A=" << A << "\n";
    cout << "B=" << B << "\n";
}
