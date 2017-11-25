// wcat - wide version of cat.
// Actually no need for this.  Cat works just fine save for not turning on wide char support for output terminals
// Greg Blair, Oct 30, 2017

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{

  std::ios::sync_with_stdio(false);  // NEED THIS!
  // without unsynching from C, GNU libstdc++ goes through C IO streams, which can never print a wide char after printing a narrow char on the same stream

  std::locale::global( std::locale( "" ) ) ;
  std::wcout.imbue( std::locale() ) ;     // works fine on Ubuntu without this line
  std::wcin.imbue( std::locale() ) ;      // not using wcin
  std::wcerr.imbue( std::locale() ) ;     // not using wcerr

  wstring s;

  if(argc == 1) {  // no args.  copy wcin to wcout

    while (getline(wcin, s))
        wcout << s << L"\n";

  } else {  // args, copy each file to wcout

    for(int arg = 1; arg < argc; arg++) {
      wifstream f(argv[arg]);
      if(f.is_open()) {
        // while (f >> s) // no good, can't use: it eats \n
        while (getline(f, s))
            wcout << s << L"\n";
        f.close();
      }
    }

  }
}
