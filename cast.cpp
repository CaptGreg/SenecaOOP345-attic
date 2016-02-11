#include <iostream>
using namespace std;

// bit manipulation function templates: see bits.cpp

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



int main()
{
  {
    unsigned int ui;   // (same as unsigned)
    signed   int si;   // (same as int)

    ui = ~0;

    si = ~0;

    cout << "ui,si=" << ui << "," << si << "\n";

    si = (int) ui;
    cout << "ui,si=" << ui << "," << si << "\n";

    ui = (unsigned) si;
    cout << "ui,si=" << ui << "," << si << "\n";

    ui = 0;
    si = (int) ui;
    cout << "ui,si=" << ui << "," << si << "\n";
  }
  


  // what happens with bit fields?
  {
    struct {
      unsigned int ui : 3;
      signed   int si : 3;
    } bits;

    bits.ui = ~0;

    bits.si = ~0;

    cout << "bits.ui,bits.si=" << bits.ui << "," << bits.si << "\n";
    printBits(bits.ui);
    printBits(bits.si);

    bits.si = (int) bits.ui;
    cout << "bits.ui,bits.si=" << bits.ui << "," << bits.si << "\n";

    bits.ui = (unsigned) bits.si;
    cout << "ui,si=" << bits.ui << "," << bits.si << "\n";

    bits.ui = 0;
    bits.si = (int) bits.ui;
    }
}
