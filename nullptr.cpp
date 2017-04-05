#include <iostream>

int main()
{
  char*p = nullptr;
  std::cout << (void *)p << "\n";
  // std::cout << (int) p[0] << "\n";    // <-- segfaults

  p++;
  std::cout << (void *)p << "\n";
  // std::cout << (int) p[0] << "\n";    // <-- segfaults

  p--;
  std::cout << (void *)p << "\n";
  // std::cout << (int) p[0] << "\n";    // <-- segfaults
}
