#include <string.h>  // strlen

/* reverse:  reverse string s in place */
static void reverse(char s[])
{
  int lo, hi;
  char c;

  for (lo = 0, hi = strlen(s) - 1; lo < hi; lo++, hi--) {
    // swap a[lo] + a[hi]
    c = s[lo];
    s[lo] = s[hi];
    s[hi] = c;
  }
}

/* itoa:  convert n to characters in s */
extern void itoa1(int n, char s[])
{
  int i, sign;

  if ((sign = n) < 0) {  /* record sign */
    n = -n;          /* make n positive */
  }

  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0) {
    s[i++] = '-';
  }

  s[i] = '\0';

  reverse(s);
}

/* itoa:  convert n to characters in s */
extern void itoa2(int n, char s[], int radix)
{
  int sign = (radix == 10 && n < 0);   /* record sign */
  if (sign) {
    n = -n;          /* make n positive */
  } 

  int i = 0;
  do {
    /* generate digits in reverse order */
    int v = n % radix;
    s[i++] = (v < 10) ?  v + '0': v - 10 + 'a';
  } while ((n /= radix) > 0);     /* delete it */

  if (sign < 0) {
    s[i++] = '-';
  }

  s[i] = '\0';

  reverse(s);
}

