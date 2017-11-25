// wide characters: 'thankyou, no problem', in Chinese

#include <iostream>
#include <locale> // <-- not needed with g++

int main()
{
  std::ios::sync_with_stdio(false);  // NEED THIS!
  // without unsynching from C, GNU libstdc++ goes through C IO streams, which can never print a wide char after printing a narrow char on the same stream
  std::locale::global( std::locale( "" ) ) ; // <-- enables wide character support for iostream

  std::wcout << L"谢谢, 没问题" << "\n";
}
