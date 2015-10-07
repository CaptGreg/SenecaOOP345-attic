#include <cstring>
#include <utility>  // std::move
#include <fstream>
#include <iostream>

class Verbose {
  static bool bVerbose;
public:
  Verbose ()        { clrVerbose(); }
  void setVerbose() { bVerbose = true; }
  void clrVerbose() { bVerbose = false; }
  bool getVerbose() { return bVerbose; }
};
bool Verbose::bVerbose = false;
// Verbose::bVerbose unresolved symbol linker error if not initialized as above

int main()
{
}
