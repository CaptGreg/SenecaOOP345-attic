// http://www.cplusplus.com/reference/csetjmp/longjmp/

// GB converted printf to cout

/* longjmp example */
#include <iostream>
using namespace std;
#include <csetjmp>     /* jmp_buf, setjmp, longjmp */

int main(int argc, char**argv)
{
  jmp_buf env;
  int val;

  val=setjmp(env);

  cout << "val is " << val << "\n";

  if (!val) longjmp(env, -100);

  return 0;
}
