#include <iostream>
using namespace std;

void goo1() {}
void goo2() {}
void goo3() {}
void goo4() {}
void goo5() {}
void goo19() {}
void gooxyz() {}

int main(int argc, char**argv)
{
#if 0
// suppose you need:
extern void goo1();
extern void goo2();
extern void goo3();
extern void goo4();
extern void goo5();
struct {
  char *command,
  void (*callBack) ()
} tableFull[] ={
   {"goo1",   goo1},
   {"goo2",   goo2},
   {"goo3",   goo3},
   {"goo4",   goo4},
   {"goo5",   goo5},
   {"goo19",  goo19},
   {"gooxyz", gooxyz},
   };
#endif


#define EXTERN_LINE(f,num) extern void f##num();
EXTERN_LINE(goo,1)
EXTERN_LINE(goo,2)
EXTERN_LINE(goo,3)
EXTERN_LINE(goo,4)
EXTERN_LINE(goo,5)
EXTERN_LINE(goo,19)
EXTERN_LINE(goo,xyz)

typedef struct tableEntry_s {
  const char *command;
  void (*callBack) ();
} tableEntry_t;

#define TABLE_ENTRY(f,num) {#f #num, f##num}

tableEntry_t commandTable[] ={
  TABLE_ENTRY(goo,1),
  TABLE_ENTRY(goo,2),
  TABLE_ENTRY(goo,3),
  TABLE_ENTRY(goo,4),
  TABLE_ENTRY(goo,5),
  TABLE_ENTRY(goo,19),
  TABLE_ENTRY(goo,xyz)
};

  for (size_t i = 0; i < sizeof(commandTable)/sizeof(tableEntry_t); i++)
    cout << i << ": " << commandTable[i].command << "\n";
}
