#include <locale>
#include <iostream>
#include <fstream> 
#include <iosfwd>   // wofstream
#include <cstdlib>  // rand
#include <string>
using namespace std;

const wchar_t* chineseDigits = L"零一二三四五六七八九"; // All 10 Chinese digits 0 through 9

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
  wcout << "Chinese characters for 0 to 9: " << chineseDigits << endl;
  for(int i = 0; i <= 9; i++) 
    wcout << "Ascii digit " << i << " is Chinese character" << chineseDigits[i] << "\n";

  // works!
}

void WriteTestFile(string& fileChinese)
{

  // write a test file with 1 million Chinese characters
  size_t size = 1000000;

  std::wofstream wos(fileChinese, wios::out | wios::trunc);
  if(wos.is_open()) {
    for(int i = 0; i < size; i++) {
       wchar_t wc;

       wc =  chineseDigits[rand() % 10];
       wos << wc;
    }
    wos.close();
  }
}

void Translate(string& fileChinese, string& fileOut)
{
///////////////////////////////////////////////////////////////////////////////
  
  // Write code here.
  // Read in the file created above wide character by wide character 
  // Write a second file out with the characters translated to ASCII digits.

///////////////////////////////////////////////////////////////////////////////

  // Solution:
  std::wifstream wis(fileChinese, wios::in);
  std::ofstream  os (fileOut,     ios::out | ios::trunc);

  if(wis.is_open() && os.is_open()) {
    wchar_t wc;
    while( wis >> wc ) {
      for(int i = 0; i < 10; i++) {
        if(wc == chineseDigits[i]) {
          char c = '0' + i;
          os << c;
          break;
        }
      }
    }
  } else {
    throw string("problem creating '") + fileOut + "' or opening '" + fileChinese + "'\n";
  }

  if(wis) wis.close();
  if(os)  os .close();
///////////////////////////////////////////////////////////////////////////////

}


int main(int argc, char* argv[])
{
  try {

    string rootFileName  =  string(argv[0]);
    string fileChinese = rootFileName + ".dat.Chinese";
    string fileOut     = fileChinese  + ".ASCII";

    WideCharSetup();

    WriteTestFile(fileChinese);

    Translate(fileChinese, fileOut);

  } catch(string& e) {
    cerr << e << "\n";
  }

    return 0;
}
