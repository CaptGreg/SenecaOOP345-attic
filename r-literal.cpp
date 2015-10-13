#include <iostream>
#include <locale>
using namespace std;

int main(int argc, char**argv)
{
  char* p;
 
  p = "Dogs ears, cat tails, and moose tails\n";
  cout << p;
 
  p = "Dogs ears,"
" cat tails, "
"and moose tails\n";
  cout << p;
 
  p = "Dogs ears,"
" cat tails, "
" \"cats tails are a plant as well as the dangly thing on the back end of a cat \""
"and moose tails\n";
  cout << p;

// R literal R"symbol( ... )symbol"
  p = R"rliteral(Dogs ears,
 cat tails, 
"cats tails are a plant as well as the dangly thing on the back end of a cat"
and moose tails
)rliteral";

  cout << p;

#ifdef __GNUC__
  wcout.sync_with_stdio(false);
  wcout.imbue(locale("en_US.utf8"));
#endif

#ifdef __MSVC
_setmode(_fileno(stdout), _O_U16TEXT); // or _O_WTEXT
#endif

  wchar_t *wp;

  wp = L"( hello from 中国)";

  wcout << wp;

}

