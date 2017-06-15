#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

vector<string> v ={ "Windows", "UNIX"};

int main()
{
  long long seed = time(0LL);

  // start off the random number generator with the current time in seconds since Jan 1, 1970.

  srandom(seed);

  cout << "USE " << v[random() % 2] << "\n";
}
