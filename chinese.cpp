#include <locale>
#include <iostream>
using namespace std;


int main(int argc, char ** argv)
{
#ifdef __GNUC__
  wcout.sync_with_stdio(false);
  wcout.imbue(locale("en_US.utf8"));
#endif

#ifdef __MSVC
_setmode(_fileno(stdout), _O_U16TEXT); // or _O_WTEXT
#endif

  wcout << L"双喜雙喜!" << endl;
  wcout << L"零一二三四五六七八九" << endl;

  const wchar_t *chineseDigits = L"零一二三四五六七八九";

  wcout << chineseDigits << endl;
}
