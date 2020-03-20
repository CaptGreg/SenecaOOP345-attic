#include <iostream>

struct {
  uint8_t  ui8;
  uint16_t ui16;
  uint32_t ui32;
} s;

struct {
  char  i8;
  short i16;
  int  i32;
} t;


int main(int argc, char* argv[])
{
  std::cout << "sizeof(s)=" << sizeof(s) << "\n";
  std::cout << "sizeof(t)=" << sizeof(t) << "\n";
}
