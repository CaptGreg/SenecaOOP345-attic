#include <iostream>

char *p1  = "qwerty";
char p2[] = "QWERTY";

int main()
{
  //"qwerty" is a const char*

  // p1 points to a const char* "qwerty" is stored in const memory
  // the values of p1 cannot be altered.


  // p1 is a char array, not a const char array.
  // p1 happes to initialized with a const char array.
  // The values in p2 can be modified.
  
}
