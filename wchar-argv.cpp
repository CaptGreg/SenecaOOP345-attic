// passing Unicode parameters to main(int argc,char*argv[])

// Unicode Transformation Format (UTF) 
// UTF-8 is the minimal size unicode that handles ascii
// https://en.wikipedia.org/wiki/Unicode

// need to 
//  1. treat the char* argv[] characters as a unicode stream
//  2. make a wchar_t for each unicode character
  //  3. build up a wstring consisting of the wchar_t's

// http://stackoverflow.com/questions/1664476/is-it-possible-to-use-a-unicode-argv

// ./wchar-argv 1 2 3 零一二三四五六七八九 零 一 二 三 四 五 六 七 八 九 공 일 이 삼 사 오 육 칠 팔 구 صفر، واحد، إثنان، ثلاثة، اربعة، خمسة، ستة، سبعة، ثمانية، تسعة، عشرة 

#include <iostream>  // wcout
#include <string>    // wstring

#define ARR_LEN(x) (sizeof(x)/sizeof(x[0]))

std::wstring Convert(const char * s) {
  struct Level { 
    uint8_t  Head;  // the head shifted to the right
    uint8_t  Data;  // number of data bits
    uint8_t  Null;  // encoded byte with zero data bits
    Level(uint8_t h, uint8_t d) : Head(h), Data(d), Null(h << d) { }
    bool encoded(uint8_t byte) { return byte>>Data == Head; }
  }; // struct Level
  static Level lev[] = { 
    Level(2,   6),
    Level(6,   5), 
    Level(14,  4), 
    Level(30,  3), 
    Level(62,  2), 
    Level(126, 1)
  };

  wchar_t wc = 0;
  const char * p = s;
  std::wstring result;
  while (*p != 0) {
    uint8_t byte = *p++;
    if (byte>>7 == 0) { // deal with ASCII
      wc = byte;
      result.push_back(wc);
      continue;
    } // ASCII
    bool found = false;
    for (unsigned i = 1; i < ARR_LEN(lev); i++) {
      if (lev[i].encoded(byte)) {
        wc = byte ^ lev[i].Null; // remove the head
        wc <<= lev[0].Data * i;
        for (int j = i; j > 0; --j) { // trailing bytes
          if (*p == 0) 
            return result; // unexpected
          byte = *p++;   
          if (!lev[0].encoded(byte)) // encoding corrupted
            return result;
          wchar_t tmp = byte ^ lev[0].Null;
          wc |= tmp << lev[0].Data*(j-1);
        } // trailing bytes
        result.push_back(wc);
        found = true;
        break;
      } // lev[i]
    }   // for lev
    if (!found) 
      return result; // encoding incorrect
  }   // while
  return result;
}   // wstring Convert

int main(int argc, char*argv[])
{
#ifdef __MSVC
  _setmode(_fileno(stdout), _O_WTEXT); // or __O_U16TEXT
#endif

  std::locale::global( std::locale( "" ) ) ;
  // std::wcout.imbue( std::locale() ) ;      // works fine without this line
  // std::wcin .imbue( std::locale() ) ;      // not using wcin
  // std::wcerr.imbue( std::locale() ) ;      // not using wcerr

  for(int arg=1; arg <argc; arg++)
    std::wcout << "arg " << arg << ": " << Convert(argv[arg]) << "\n";
}
