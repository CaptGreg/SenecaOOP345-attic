#include <locale>
#include <iostream>
#include <exception>
using namespace std;


int main(int argc, char ** argv)
{
  try {

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
