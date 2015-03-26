#include <iostream>     // C++ code: cout
#include <unistd.h>     // C-code: open/close/read/write/seek/tell
#include <cstdio>       // C-code: fopen/fclose/fread/fwrite/fseek/ftell
#include <cstring>      // strlen

int main()
{
   char *p;

   p ="hello low-level write fd 1\n";
   write(1, p, strlen(p));    // stdout/cout

   p ="hello low-level write fd 2\n";
   write(2, p, strlen(p));    // stderr/cerr

   p ="hello stdio stdout\n";
   fwrite(p, 1, strlen(p), stdout);

   p ="hello stdio stderr\n";
   fwrite(p, 1, strlen(p), stderr);

   FILE* fp;

   fp = stdout;
   p ="hello stdio stdout\n";
   fwrite(p, 1, strlen(p), fp);

   fp = stderr;
   p ="hello stdio stderr\n";
   fwrite(p, 1, strlen(p), fp);

   p ="hello C++ cout\n";
   std::cout << p;

   p ="hello C++ cerr\n";
   std::cerr << p;
}
