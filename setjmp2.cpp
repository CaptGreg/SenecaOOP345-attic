/* setjmp example: error handling */
#include <iostream>
using namespace std;
#include <csetjmp>     /* jmp_buf, setjmp, longjmp */

int main(int argc, char**argv)
{
  jmp_buf env;
  int val;

  val = setjmp (env);
  if (val) {
    cout << "Error " << val << " happened\n";
    return val;
  }

  /* code here */

  longjmp (env,101);   /* signaling an error */

  return 0;
}
