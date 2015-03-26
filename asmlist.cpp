 // g++ -c -g -Wa,-alh asmlist.cpp -o asmlist.s
 // g++ -c -S asmlist.cpp -o asmlist.s
 // g++ -save-temps -fverbose-asm asmlist.cpp


int main()
{
  int x;
  int y;

  x = 3;
  y = x;

  return x;
}
