#include <locale>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <iosfwd>   // wofstream
#include <cstdlib>  // rand
using namespace std;

const wchar_t* chineseWCharDigits = L"零一二三四五六七八九"; // All 10 Chinese     digits 0 through 9
const wchar_t* koreanWCharDigits  = L"공일이삼사오육칠팔구"; // All 10 Sino-Korean digits 0 through 9
// const wchar_t* arabicWCharDigits  = L"ثلاثاثنانواحدصفرستةخمسةأربعةتسعةثمانيةسبعة"; // All 10 Arabic digits 0 through 9
// Chinese: google translate
// Korean http://www.omniglot.com/language/numbers/korean.htm
// Arabic: google translate

// Correct Arabic numbers: [from right to left from 0 to 10 comma separated] 
// صفر، واحد، إثنان، ثلاثة، اربعة، خمسة، ستة، سبعة، ثمانية، تسعة، عشرة 
const wchar_t *arabicWCharDigits[] = { 
  L"صفر",  L"واحد", L"إثنان", L"ثلاثة",   L"اربعة", 
  L"خمسة", L"ستة",  L"سبعة",  L"ثمانية", L"تسعة", 
  L"عشرة"
};


const std::wstring chineseWString = chineseWCharDigits;
const std::wstring koreanWString  = koreanWCharDigits;
const std::wstring arabicWString[11] // 0-10 = 11 numbers
 = { 
  std::wstring(L"صفر"),    /*  0 */
  std::wstring(L"واحد"),   /*  1 */
  std::wstring(L"إثنان"),  /*  2 */
  std::wstring(L"ثلاثة"),   /*  3 */
  std::wstring(L"اربعة"),  /*  4 */
  std::wstring(L"خمسة"),   /*  5 */
  std::wstring(L"ستة"),    /*  6 */
  std::wstring(L"سبعة"),   /*  7 */
  std::wstring(L"ثمانية"), /*  8 */
  std::wstring(L"تسعة"),   /*  9 */
  std::wstring(L"عشرة")    /* 10 */
};

void WideCharSetup()
{
#ifdef __MSVC
  _setmode(_fileno(stdout), _O_WTEXT); // or __O_U16TEXT); // or _O_WTEXT
#endif

  std::locale::global( std::locale( "" ) ) ;
  std::wcout.imbue( std::locale() ) ;
  // std::wcin.imbue( std::locale() ) ;       // not using wcin
  // std::wcerr.imbue( std::locale() ) ;      // not using wcerr

  // test it
  #define P(L,D) wcout << #L " wchar_t characters for 0 to 9: " << D << " ==> " << wcslen(D) << " wchar_t\n";
  P(Chinese, chineseWCharDigits)
  P(Korean, koreanWCharDigits)
//P(Arabic, arabicWCharDigits)

  // wcout << "Chinese wchar_t characters for 0 to 9: " << chineseWCharDigits << endl;
  // wcout << "Korean  wchar_t characters for 0 to 9: " << koreanWCharDigits << endl;
  // wcout << "Arabic  wchar_t characters for 0 to 9: " << arabicWCharDigits << endl;
  wcout << "\n";

  wcout << "Chinese std::wstring characters for 0 to 9: " << chineseWString << endl;
  wcout << "Korean  std::wstring characters for 0 to 9: " << koreanWString << endl;
  wcout << "\n";


  for(int i = 0; i <= 9; i++) 
    wcout << "Ascii digit " << i 
          << " is Chinese "   << chineseWCharDigits[i] 
          << ", Sino-Korean " << koreanWCharDigits[i] 
          << ", Arabic " << std::setw(8) << arabicWCharDigits[i] 
          << "\n";

  // works!
}

template<typename T> // a function template
void WriteTestFile(string& file, T& digits) // need T& so this works with 'const std::wstring'
{

  // write a test file with 1 million Chinese characters
  size_t size = 1000000;

  std::wofstream wos(file, wios::out | wios::trunc);
  if(wos.is_open()) {
    for(size_t i = 0; i < size; i++) {
       wchar_t wc;

       wc =  digits[rand() % 10];
       wos << wc;
    }
    wos.close();
  }
}

// void Translate(const wchar_t* digits, string& fileIn, string& fileOut)
template<typename T> // a function template
void Translate(T& digits, string& fileIn, string& fileOut)
{
  std::wifstream wis(fileIn,  wios::in);
  std::ofstream  os (fileOut, ios::out | ios::trunc);

  if(wis.is_open() && os.is_open()) {
    wchar_t wc;
    while( wis >> wc ) {
      for(size_t i = 0; i < wcslen(digits); i++) {
        if(wc == digits[i]) {
          char c = '0' + i;
          os << c;
          break;
        }
      }
    }
  } else {
    throw string("problem creating '") + fileOut + "' or opening '" + fileIn + "'\n";
  }

  if(wis) wis.close();
  if(os)  os .close();
}


int main(int argc, char* argv[], char* envp[])
// int wmain(int argc,wchar_t*argv[],wchar_t*envp[]) // appears to be a Microsoft extension, not C++
{
  try {

    string rootFileName   =  string(argv[0]);

    string fileChinese    = rootFileName + ".dat.Chinese";
    string fileChineseOut = fileChinese  + ".ASCII";

    string fileKorean     = rootFileName + ".dat.Korean";
    string fileKoreanOut  = fileKorean   + ".ASCII";

    WideCharSetup();

    WriteTestFile(fileChinese, chineseWCharDigits);
    WriteTestFile(fileChinese, chineseWString);

    WriteTestFile(fileKorean,  koreanWCharDigits );
    WriteTestFile(fileKorean,  koreanWString );

    Translate(chineseWCharDigits, fileChinese, fileChineseOut);
    Translate(koreanWCharDigits,  fileKorean,  fileKoreanOut);

  } catch(string& e) {
    cerr << e << "\n";
  }

    return 0;
}
