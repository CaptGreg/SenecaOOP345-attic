 // g++ -c -g -Wa,-alh asmlist.cpp -o asmlist.o
 // creates a 7 page mixed source assembler listing on cout

 // g++ -S asmlist.cpp -o asmlist.s
 // -rw-rw-r-- 1 greg greg  639 Oct 21 08:36 asmlist.cpp
 // -rw-rw-r-- 1 greg greg 2339 Oct 21 08:37 asmlist.s

 // g++ -save-temps -fverbose-asm asmlist.cpp -o asmlist
 // -rwxrwxr-x 1 greg greg   9112 Oct 21 08:35 asmlist
 // -rw-rw-r-- 1 greg greg    352 Oct 21 08:35 asmlist.cpp
 // -rw-rw-r-- 1 greg greg 428287 Oct 21 08:35 asmlist.ii
 // -rw-rw-r-- 1 greg greg   2776 Oct 21 08:35 asmlist.o
 // -rw-rw-r-- 1 greg greg   4489 Oct 21 08:35 asmlist.s

 // asmlist.s is an assembler listing
 // asmlist.ii is an cpp-code listing

 // g++ -Wa,-adhln -g asmlist.cpp -o asmlist

/// Generate assembly list mixed with the source code
/// Just add these gcc compile options:
/// -Wa,-adhln -g
/// The command:
/// $ gcc -Wa,-adhln -g source_code.c > assembly_list.s
/// The options:
/// -g: Produce debugging information
/// -Wa,option: Pass option as an option to the assembler
/// -adhln:
/// a: turn on listings
/// d: omit debugging directives; n: omit forms processing
/// h: include high-level source
/// l: include assembly

// #include <iostream>
// using namespace std;

class X {
public:
  int val;
};


int main()
{
  X xa, xb;

  xa.val = 89;
  xb.val = 99;

  xa = xb;

  int x;
  int y;

  x = 3;
  y = x;

//   cout << "x="<<x <<" y="<<y <<"\n";

  return x;
}
