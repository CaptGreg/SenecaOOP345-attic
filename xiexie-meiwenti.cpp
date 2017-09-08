#include <iostream>
#include <locale> // <-- not needed with g++

int main()
{
  std::locale::global( std::locale( "" ) ) ; // <-- enables wide character support for iostream

  std::wcout << L"谢谢, 没问题" << "\n";
}
