// Definitive proof references are pointers
// 1. compile generating a mixed source code assembler listing
//    g++ -Wa,-adhln -g references-are-pointers.cpp -o references-are-pointers > references-are-pointers.s
// 2. examine 'references-are-pointers.s' 
// 3. note 
//    3.1 the reference and pointer function assembler code is identical.
//    3.2 the reference and pointer function assembler calling sequence is identical.

// QED: References are pointers.

int fValue   (int i)  {return  i *  i; }
int fRef     (int& r) {return  r *  r; }  // generates identical assembler to fPointer
int fPointer (int* p) {return *p * *p; }  // generates identical assembler to fRef

int main()
{
  int i, ret;

  i = 7;
  ret = fValue(i);

  ret = fPointer(&i);                     // generates identical calling sequence to fRef

  ret = fRef(i);                          // generates identical calling sequence to fPointer
  return 0;
}
