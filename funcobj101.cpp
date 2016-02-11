// Function Objects (functors) 101

#include <iostream>
#include <string>
#include <cstring> // strcmp
#include <cstdlib> // qsort
using namespace std;

// the idea behind function objects is to use an overoaded () operator for a class as function pointer
// illustrate using the c-code qsort routine test code from 'man qsort'


static int
cmpstringp(const void *p1, const void *p2) // man qsort sample code
{
   /* The actual arguments to this function are "pointers to
      pointers to char", but strcmp(3) arguments are "pointers
      to char", hence the following cast plus dereference */

   return strcmp(* (char * const *) p1, * (char * const *) p2);
}

enum Order_e { ASCEND, DESCEND };
class X {
  Order_e o;
public:
  X(Order_e O) : o(O) {}
  int operator()(const void*p1, const void*p2) {
    return o == ASCEND?
           strcmp((const char*)p1, (const char*)p2):
      -1 * strcmp((const char*)p1, (const char*)p2);
  }
};

X a(ASCEND);
int ascend(const void*p1, const void*p2) {
  return a(p1,p2); // function object
}

X d(DESCEND);
int descend(const void*p1, const void*p2) {
  return d(p1,p2); // function object
}

int main(int argc, char**argv)
{
  cout << "input data\n";
  for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

  cout << "c-code function compare\n";
  qsort(argv+1, argc-1, sizeof(char*), cmpstringp); // c-code function
  for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

  cout << "function ascend\n";
  qsort(argv+1, argc-1, sizeof(char*), ascend);     // function
  for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

  cout << "function descend\n";
  qsort(argv+1, argc-1, sizeof(char*), descend);    // function
  for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

  cout << "function object lambda ascend\n";
  qsort(argv+1, argc-1, sizeof(char*), [](const void*p1,const void*p2) {X x(ASCEND); return x(p1,p2); } ); // lambda function object
  for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

  cout << "function object lambda descend\n";
  qsort(argv+1, argc-1, sizeof(char*), [](const void*p1,const void*p2) {X x(DESCEND); return x(p1,p2); } ); // lambda function object
  for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

  // cout << "function object a (ascend)\n";
  // X a(ASCEND);
  // qsort(argv+1, argc-1, sizeof(char*),  a);          // function object
  // for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

  // cout << "function object d (descend)\n";
  // X d(DESCEND);
  // qsort(argv+1, argc-1, sizeof(char*), d);          // function object
  // for(int arg=1; arg<argc; arg++) cout << argv[arg] << "\n";

}
