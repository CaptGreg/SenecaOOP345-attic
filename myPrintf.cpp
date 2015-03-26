#include <iostream>
using namespace std;

#include <cstdio>
#include <cstdarg>

// void myPrintf(const char *fmt, ...);
void myPrintf(const char *fmt, ...)
// C dudes call this a variable argument function
// C++ dudes call this a variadic function --- not a word (not in the Oxford English Dictionary)
{
  va_list va;           // allocate a va_list variable 'va'
  va_start(va, fmt);    // initialize 'va' to start after 'fmt'
  // for(char *f=fmt; *f; f++) { // print out fmt, one char at a time
    // cout << *f;
  // }

  for(const char *f=fmt; *f; f++) { // process fmt string, one char at a time
    if(*f == '%') { // process format character
      f++;  // want character after %
      double d;
      int    i;
      char   c;
      char  *s;
      switch(*f) {
      case 'f': d = va_arg(va, double); cout << d; break;
      case 's': s = va_arg(va, char*);  cout << s; break;
      case 'c': c = va_arg(va, int);    cout << c; break;  // error: use 'int' to fetch 'char' variable
      case 'd': i = va_arg(va, int);    cout << i; break;
      default: // unknown format character
        cout << '%' << *f; // don't understand format charcter, just print it along with %
        { // and throw an error to demonstrate C++ try - throw - catch error mechanism
          string errMsg("==>X<== bad format char");
          *((char*) errMsg.c_str() + 3) = *f; // replace 'X', 4th char, index=3, with unknown format character
          throw errMsg;
        }
        break;
      }
    } else { // not a format character, just print it
      cout << *f;
    }
  }
  va_end(va); // clean up
}

int main(int argc, char **argv) // returns 0 if successful, non-zero if error
{
  printf("double=%f, string=%s, char=%c, int=%d\n", 1.2, "hi there", '?', 127);

  try {
    cout << "\ncalling myPrintf\n";
    myPrintf("double=%f, string=%s, char=%c, int=%d  %?\n", 1.2, "hi there", '?', 127);
  } 
  // note 'catch' argument overload
  catch(const char*msg) { // if something throws a 'const char *'
    cout << '\n'<< argv[0] << ": " << msg << " - program threw\n";
    return 1;
  } catch(char*msg) { // if something throws a 'char *'
    cout << '\n'<< argv[0] << ": " << msg << " - program threw\n";
    return 1;
  } catch(string msg) { // if something throws a 'string'
    cout << '\n'<< argv[0] << ": " << msg << " - program threw\n";
    return 1;
  }
  return 0;
}
