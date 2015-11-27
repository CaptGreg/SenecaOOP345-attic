#include <locale>
#include <iostream>
#include <fstream> 
#include <iosfwd>   // wofstream
#include <iomanip>
#include <cstdlib>  // rand
using namespace std;

const wchar_t* chineseWCharDigits = L"零一二三四五六七八九"; // All 10 Chinese     digits 0 through 9
const wchar_t* koreanWCharDigits  = L"공일이삼사오육칠팔구"; // All 10 Sino-Korean digits 0 through 9
const wchar_t* arabicWCharDigits  = L"ثلاثاثنانواحدصفرستةخمسةأربعةتسعةثمانيةسبعة"; 
// Chinese: google translate
// Korean http://www.omniglot.com/language/numbers/korean.htm
// Arabic: google translate

const std::wstring chineseWString = chineseWCharDigits;
const std::wstring koreanWString  = koreanWCharDigits;
const std::wstring arabicWStringArray[] = // Arabic: google translate
  {L"ثلاث", L"اثن", L"انو", L"احدصف", L"رست", L"ةخمسة", L"أربعة", L"تسعةثم", L"اني", L"ةسبعة"}; 
//   0       1       2       3         4       5         6         7          8       9

const std::wstring arabicWStringArray2[] =
{
  /* 0 */     L"صفر",     /* Sifr       */
  /* 1 */     L"واحد",    /* waa7id     */
  /* 2 */     L"اثنان",   /* ithnaan    */
  /* 3 */     L"ثلاثة",    /* thalaatha  */
  /* 4 */     L"أربعة",   /* arba3a     */
  /* 5 */     L"خمسة",    /* khamsa     */
  /* 6 */     L"ستة",     /* sitta      */
  /* 7 */     L"سبعة",    /* sab3a      */
  /* 8 */     L"ثمانية",  /* thamaaneya */
  /* 9 */     L"تسعة"     /* tis3a      */
};

// http://blogs.transparent.com/arabic/arabic-numbers-1-20/
// 0 صفر Sifr 
// 1 واحد waa7id 
// 2 اثنان ithnaan 
// 3 ثلاثة thalaatha 
// 4 أربعة arba3a 
// 5 خمسة khamsa 
// 6 ستة sitta 
// 7 سبعة sab3a 
// 8 ثمانية thamaaneya 
// 9 تسعة tis3a 
// 10 عشرة 3ashara 
// 11 أحد عشر a7ada 3ashar
// 12 اثنا عشر ithna 3ashar 
// 13 ثلاثة عشر thalaathata 3ashar 
// 14 أربعة عشر arba3ata 3ashar 
// 15 خمسة عشر khamsata 3ashar 
// 16 ستة عشر sittata 3ashar 
// 17 سبعة عشر sab3ata 3ashar 
// 18 ثمانية عشر thamaneyata 3ashar 
// 19 تسعة عشر tis3ata 3ashar 
// 20 عشرون 3ishroon
 

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
  P(Arabic, arabicWCharDigits)

  // wcout << "Chinese wchar_t characters for 0 to 9: " << chineseWCharDigits << endl;
  // wcout << "Korean  wchar_t characters for 0 to 9: " << koreanWCharDigits << endl;
  // wcout << "Arabic  wchar_t characters for 0 to 9: " << arabicWCharDigits << endl;
  wcout << "\n";

  wcout << "Chinese std::wstring characters for 0 to 9: " << chineseWString << endl;
  wcout << "Korean  std::wstring characters for 0 to 9: " << koreanWString << endl;
  wcout << "\n";

  for(int i = 0; i <= 9; i++) 
    wcout << "Ascii digit " << i 
          << " is Chinese "   << chineseWCharDigits  [i] 
          << ", Sino-Korean " << koreanWCharDigits   [i] 
          << ", Arabic "      << setw(7) << arabicWStringArray  [i] 
          << ", Arabic2 "     << setw(7) << arabicWStringArray2 [i] 
          << ", w=" << wcslen(arabicWStringArray2 [i].c_str())
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
