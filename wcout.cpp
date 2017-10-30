#include <iostream>
#include <string>
// #include <locale>  // not needed with g++
using namespace std;

int main()
{
  std::ios::sync_with_stdio(false);  // NEED THIS!
  // without unsynching from C, GNU libstdc++ goes through C IO streams, which can never print a wide char after printing a narrow char on the same stream
  locale::global( std::locale( "" ) );  //  enable wide characters

  wcout << "Hello World" << "\n";       // wcout is wide charcter print

  wcout << L"Hello World" << "\n";      // L"..." wide character ... constant

  wcout << L"你好世界" << "\n";         // Nǐhǎoshìjiè (hello world)

  // wide characters
  wchar_t* wp = L"中文";                // Zhōngwén (Chinese, Chinese language)
  wcout << wp << "\n";

  // wide string
  wstring ws = L"中文";                 // Zhōngwén (Chinese, Chinese language)
  wcout << ws << "\n";

  return 0;
}
