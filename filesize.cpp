#include <iostream>
#include <fstream>

using namespace std;

int64_t filesize(char*f)
{
  ifstream i(f, ios::in | ios::binary | ios::ate);
  return i? uint64_t(i.tellg()): -1;
}

int main(int argc, char**argv)
{
  for(int arg = 1; arg < argc; arg++)
    cout << argv[arg] << " = " << filesize(argv[arg]) << "\n";
}

