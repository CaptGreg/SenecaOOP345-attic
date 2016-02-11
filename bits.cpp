#include <iostream> // cout
using namespace std;

// bit manipulation function templates

template <typename T>
bool getBit(const int bit, const T  var)
    { return var & (1<< bit); }

template <typename T>
T setBit(const int bit, const T  var)
    { return var | (1<< bit); }

template <typename T>
void setBitInPlace(const int bit, T&  var)
    { var |= (1<< bit); }

template <typename T>
T clearBit(const int bit, const T var)
    { return var & (~(1<< bit)); }

template <typename T>
void clearBitInPlace(const int bit, T& var)
    { var &= (~(1<< bit)); }

template <typename T>
T storeBit(const int bit, const T var, const bool bitValue)
    { return bitValue? setBit(bit, var): clearBit(bit,var); }

template <typename T>
void storeBitInPlace(const int bit, T& var, const bool bitValue)
    { bitValue? setBitInPlace(bit, var): clearBitInPlace(bit,var); }

template <typename T>
T flipBit(const int bit, const T var)
    { bool b = getBit(bit,var); return storeBit(bit, var, !b); }

template <typename T>
void flipBitInPlace(const int bit, T& var)
    { storeBitInPlace(bit, var, !getBit(bit,var)); }

template <typename T>
void printBits(const T var,ostream& s = cout)
  {
    for(int bit = 8 * sizeof(T) - 1; bit >= 0 ; bit--) {
      // s << (char) ( '0' + getBit(bit, var) );
      s << "01"[ getBit(bit, var) ];
    }
    s << "\n";
  }

template <typename T>
int onBits(T var)
  {
    int ret;
    for(ret = 0; var ; var >>= 1) {
      if(var & 1) { ret++; }
    }
    return ret;
  }

int main(int argc, char ** argv)
{
    uint8_t u8 = 0;
    printBits(u8);
    u8 = setBit(3,u8);
    printBits(u8);
    u8 = clearBit(3,u8);
    printBits(u8);

    uint16_t u16 = ~0;
    printBits(u16);
    cout << "bits on=" << onBits(u16) << "\n";
    u16 = flipBit(7,u16);
    printBits(u16);
    cout << "bits on=" << onBits(u16) << "\n";
    u16 = flipBit(7,u16);
    printBits(u16);
    u16 = storeBit(8*sizeof(u16)-1 ,u16, 0);
    printBits(u16);

    uint32_t u32 = 0;
    printBits(u32);
    u32 = setBit(15,u32);
    printBits(u32);
    cout << "bits on=" << onBits(u32) << "\n";
    u32 = clearBit(15,u32);
    printBits(u32);

    return 0;
}
