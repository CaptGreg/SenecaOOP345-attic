#include <iostream>
using namespace std;

int main(int argc, char**argv)
{
#ifdef DEBUG
  typedef float REAL;
#else
  typedef double REAL;
#endif

typedef class XXX { };

  REAL x = 9.0;

  cout << " sizeof(REAL) = " << sizeof(REAL) << endl;

  cout << x << endl;
}
