#include <iostream>
// #include <locale>  // not needed with g++
using namespace std;

int main()
{
  locale::global( std::locale( "" ) ) ;

  wcout <<  "Hello World\n";

  wcout << L"Hello World\n";

  wcout << L"你好世界\n";   // Nǐhǎoshìjiè (hello world)

  wcout << L"中文\n";       // Zhōngwén (Chinese, Chinese language)
}
