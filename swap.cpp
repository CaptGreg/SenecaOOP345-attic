#include <iostream>
using namespace std;

template <typename T>
inline void printBits(const T var,ostream& s = cout)
  {
    for(int bit = 8 * sizeof(T) - 1; bit >= 0 ; bit--) {
      s << (char) ( '0' + (var >> bit & 1));
    }
    s << endl;
  }

template <typename T>
inline void Swap(T& X, T& Y) {
    cout <<"Swap: " <<"X=" <<(int) X <<" Y=" <<(int) Y <<endl;
    cout <<"X:"; printBits(X);
    cout <<"Y:"; printBits(Y);
    X = X ^ Y;
    cout <<"after X = X ^ Y " <<endl;
    cout <<"X:"; printBits(X);
    cout <<"Y:"; printBits(Y);
    Y = X ^ Y;
    cout <<"after Y = X ^ Y " <<endl;
    cout <<"X:"; printBits(X);
    cout <<"Y:"; printBits(Y);
    X = X ^ Y;
    cout <<"after X = X ^ Y " <<endl;
    cout <<"X:"; printBits(X);
    cout <<"Y:"; printBits(Y);
    cout <<"Swap: " <<"X=" <<(int) X <<" Y=" <<(int) Y <<endl;
};

int main(int argc, char**argv)
{
    uint8_t X = 7;
    uint8_t Y = 16;

    cout << "before Swap " << "X(" << (int) X << ") Y(" << (int) Y <<")" << endl;

    Swap(X, Y);

    cout << "after Swap " << "X(" << (int) X << ") Y(" << (int) Y <<")" << endl;


    cout << "before ADD Swap " << "X(" << (int) X << ") Y(" << (int) Y <<")" << endl;
    X = X + Y;
    Y = X - Y;  // Y now has X value
    X = X - Y;  // X now has Y value
    cout << "after ADD Swap " << "X(" << (int) X << ") Y(" << (int) Y <<")" << endl;
}
