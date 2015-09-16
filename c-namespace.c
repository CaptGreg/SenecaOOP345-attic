// a student said C99 has namespace support
// Let's see:

// This program works as expected in C++:
// g++ c-namespace.c -o c-namespace && ./c-namespace
// namespace 'n::n' value = 3

// now try it in C99:

    /*
    gcc -std=c99 c-namespace.c -c c-namespace
    c-namespace.c:32:1: error: unknown type name ‘namespace’
     namespace n {
     ^
    c-namespace.c:32:13: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
     namespace n {
                 ^
    c-namespace.c: In function ‘main’:
    c-namespace.c:38:43: error: ‘n’ undeclared (first use in this function)
       printf("namespace 'n::n' value = %d\n", n::n);
                                               ^
    c-namespace.c:38:43: note: each undeclared identifier is reported only once for each function it appears in
    c-namespace.c:38:44: error: expected ‘)’ before ‘:’ token
       printf("namespace 'n::n' value = %d\n", n::n);
                                                ^
    */

// no namespaces in C99

#include <stdio.h>

namespace n {
  int n = 3;
}

int main(int argc, char*argv[])
{
  printf("namespace 'n::n' value = %d\n", n::n);
  return 0;
}

