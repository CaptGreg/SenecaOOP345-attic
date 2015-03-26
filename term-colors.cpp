// http://www.termsys.demon.co.uk/vtansi.htm
// 
// Set Display Attributes
// 
// Set Attribute Mode	<ESC>[{attr1};...;{attrn}m
// 
//     Sets multiple display attribute settings. The following lists standard attributes:
// 
//     0	Reset all attributes
//     1	Bright
//     2	Dim
//     4	Underscore	
//     5	Blink
//     7	Reverse
//     8	Hidden
// 
//     	Foreground Colours
//     30	Black
//     31	Red
//     32	Green
//     33	Yellow
//     34	Blue
//     35	Magenta
//     36	Cyan
//     37	White
// 
//     	Background Colours
//     40	Black
//     41	Red
//     42	Green
//     43	Yellow
//     44	Blue
//     45	Magenta
//     46	Cyan
//     47	White

#include <iostream>
using namespace std;

int main()
{

  const char *colors[] ={"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"};
  for(int fg = 30; fg <= 37; fg++) {
  for(int bg = 40; bg <= 47; bg++) {
     // Set Attribute Mode	<ESC>[{attr1};...;{attrn}m
     // Are attr values binary values or ascii integers?
     // Ascii values, I think
     cout << "\0x1b[1;" << fg << ';' << bg << 'm';
     cout 
       << "fg " << int(fg)  << " " << colors[fg&7] << " "
       << "bg " << int(bg)  << " " << colors[bg&7] << " "
       ;
     cout << "\033[" << "\0" << 'm';
  }}
}
