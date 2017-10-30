#include <iostream>
#include <string>
using namespace std;

const wchar_t* chineseWCharDigits = L"零一二三四五六七八九"; // All 10 Chinese     digits 0 through 9
const wchar_t* koreanWCharDigits  = L"공일이삼사오육칠팔구"; // All 10 Sino-Korean digits 0 through 9

const std::wstring chineseWString = chineseWCharDigits;
const std::wstring koreanWString  = koreanWCharDigits;


template<typename T>
bool bittest(T t, int bit)
{
  return t & (T(1) << bit);
}


template<typename T>
void bitprint(T t, int bits)
{
  for(int i = bits -1 ; i >= 0 ; i--) {
    wcout << "01"[bittest(t,i)];
    // wcout << "FT"[bittest(t,i)];
    // wcout << chineseWCharDigits[bittest(t,i)];
    // wcout << koreanWCharDigits[bittest(t,i)];
  }
}

void printHeading(const int BITS)
{
  for(int i = 0; i < BITS; i++)
    wcout << " ";
  wcout << " : ";
  for(int i = 0; i < (1 << BITS); i++) {
    bitprint(i, BITS);
    wcout << " ";
  }
  wcout << "\n";
}

int main()
{
  std::ios::sync_with_stdio(false);  // NEED THIS!
  // without unsynching from C, GNU libstdc++ goes through C IO streams, which can never print a wide char after printing a narrow char on the same stream
  std::locale::global( std::locale( "" ) ) ;
  std::wcout.imbue( std::locale() ) ;
  // std::wcin.imbue( std::locale() ) ;       // not using wcin
  // std::wcerr.imbue( std::locale() ) ;      // not using wcerr

  const int BITS = 2;

  wcout << "& - AND - ON if both bit ON\n";
  printHeading(BITS);
  for(int i = 0; i < (1 << BITS); i++) {
    bitprint(i, BITS);
    wcout << " : ";
    for(int j = 0; j < (1 << BITS); j++) {
      bitprint(i&j, BITS);
      wcout << " ";
    }
    wcout << "\n";
  }


  wcout << "| - OR - ON if either bit ON\n";
  printHeading(BITS);
  for(int i = 0; i < (1 << BITS); i++) {
    bitprint(i, BITS);
    wcout << " : ";
    for(int j = 0; j < (1 << BITS); j++) {
      bitprint(i|j, BITS);
      wcout << " ";
    }
    wcout << "\n";
  }

  wcout << "^ - XOR - ON if bits are different\n";
  printHeading(BITS);
  for(int i = 0; i < (1 << BITS); i++) {
    bitprint(i, BITS);
    wcout << " : ";
    for(int j = 0; j < (1 << BITS); j++) {
      bitprint(i^j, BITS);
      wcout << " ";
    }
    wcout << "\n";
  }
}

