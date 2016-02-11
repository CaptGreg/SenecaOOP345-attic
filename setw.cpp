// http://www.cplusplus.com/reference/iomanip/setw/

// setw example
#include <iostream>     // std::cout
#include <iomanip>      // std::setw

int main () {
  int width = 20;

  std::cout << "0        1         2         3\n";
  std::cout << "123456789012345678901234567890\n";
  std::cout << std::setw(width);
  std::cout << 77 <<  "<-- " << width << " wide, right justified by default\n";
  std::cout << std::left << std::setw(width);
  std::cout << 77 << "<-- " << width << " wide, left justified by std::left directive\n";
  return 0;
}
