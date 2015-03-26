// ss.str(""); clears ss

#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
  stringstream ss;

  ss << "1\n";
  cout << ss.str();

  ss.str("");
  ss << "2\n";
  cout << ss.str();

  ss.str("");
  ss << "3\n";
  cout << ss.str();
}
