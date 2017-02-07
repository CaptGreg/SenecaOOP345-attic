#include <iostream>
#include "externconst.h"

int main()
{
  double        trouble          [MAX];
  long double   bigDoubleTrouble [2*MAX];
  float         aLong            [888*MAX+33];
  char          broiled          [2*MAX+1];
  short         changed          [888*MAX+33];
  unsigned      cheque           [999*MAX+27];
  long          johns            [888*8*MAX+55];

  typedef long double fried;
  fried         chicken          [88*MAX];

  std::cout << "MAX       = " << MAX << "\n";

  int copyOfMAX = MAX;
  std::cout << "copyOfMAX = " << copyOfMAX << "\n";
}
