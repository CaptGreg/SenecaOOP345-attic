#include <iostream>
using namespace std;

int main()
{
  unsigned int ui;   // (same as unsigned)
  signed   int si;   // (same as int)

  ui = ~0;

  si = ~0;

  cout << "ui,si=" << ui << "," << si << "\n";

  si = (int) ui;
  cout << "ui,si=" << ui << "," << si << "\n";

  ui = (unsigned) si;
  cout << "ui,si=" << ui << "," << si << "\n";

  ui = 0;
  si = (int) ui;
  cout << "ui,si=" << ui << "," << si << "\n";
}

