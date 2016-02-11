#include <locale>
#include <iostream>
#include <exception>
using namespace std;

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
  const wchar_t* chineseWCharDigits = L"零一二三四五六七八九"; // All 10 Chinese     digits 0 through 9
  const wchar_t* koreanWCharDigits  = L"공일이삼사오육칠팔구"; // All 10 Sino-Korean digits 0 through 9

  const std::wstring chineseWString = chineseWCharDigits;
  const std::wstring koreanWString  = koreanWCharDigits;

  wcout << "Chinese wchar_t characters for 0 to 9: " << chineseWCharDigits << "\n";
  wcout << "Korean  wchar_t characters for 0 to 9: " << koreanWCharDigits << "\n";
  wcout << "\n";
  wcout << "Chinese std::wstring characters for 0 to 9: " << chineseWString << "\n";
  wcout << "Korean  std::wstring characters for 0 to 9: " << koreanWString << "\n";
  wcout << "\n";


  for(int i = 0; i <= 9; i++) 
    wcout << "Ascii digit " << i 
          << " is Chinese "   << chineseWCharDigits[i] 
          << ", Sino-Korean " << koreanWCharDigits[i] 
          << "\n";

  // works!
}


int main(int argc, char ** argv)
{
  try {
    WideCharSetup();

    #if 0
    {   // http://en.cppreference.com/w/cpp/locale/locale

        std::wcout << "User-preferred locale is " << std::locale("").name().c_str() << '\n';
        // on startup, the global locale is the "C" locale
        std::wcout << 1000.01 << '\n';
        // replace the C++ global locale as well as the C locale with the user-preferred locale
        std::locale::global(std::locale(""));
        // use the new global locale for future wide character output
        std::wcout.imbue(std::locale());
        // output the same number again
        std::wcout << 1000.01 << '\n';
    }
    #endif

    #ifdef __GNUC__
      wcout.sync_with_stdio(false);
      // wcout.imbue(locale("en_US.utf8"));

      wcout.imbue(locale("zh_CN.utf8")); 
      // select Chinese from /etc/locale.gen locale list, NOTE lower case .utf8
    #endif

    #ifdef __MSVC
      _setmode(_fileno(stdout), _O_U16TEXT); // or _O_WTEXT
    #endif

    // http://www.codeproject.com/Questions/104056/wcout-for-different-locales
    // std::locale loc("zh_CN.utf-8");   // Chinese locale name from /etc/locale.gen
    // std::cout.imbue(loc);
    // std::wcout.imbue(loc);

    wcout << "Selected locale " << locale("zh_CN.utf8").name().c_str() << '\n';

    wcout << L"双喜雙喜!" << L" Shuāngxǐ" << "\n"; //double (twin) happiness?
    wcout << L"双喜雙喜!" << "\n"; //double (twin) happiness?
    wcout << L"Shuāngxǐ"  << "\n"; //double (twin) happiness?

    wcout << L"零一二三四五六七八九" << "\n";

    const wchar_t *chineseDigits = L"零一二三四五六七八九";

    wcout << chineseDigits << "\n";
    for(int i = 0; i < 10; i++)
      wcout << chineseDigits[i] << " = " << i << "\n";
  } catch (std::exception& e) {
    cerr << "threw std::execption, exception.what()=" << e.what() << "\n";
  }
}
