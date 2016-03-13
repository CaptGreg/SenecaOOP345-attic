#include <csetjmp>       // setjmp/longjmp/jmp_buf
#include <iostream>
using namespace std;


int main()
{
  int i, j, k, N = 5;;

  // nested loop break method 1: c-code goto
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      for(k = 0; k < N; k++) {
         if(i == j && j == k && k == 3)
           goto loopDone;
      }
    }
  }
  loopDone:
  cout << i << "," << j << "," << k << " goto\n";


  // nested loop break method 2: c-code flag with side effects
  bool bContinue = true;
  for(i = 0; bContinue && i < N; i++) {
    for(j = 0; bContinue && j < N; j++) {
      for(k = 0; bContinue && k < N; k++) {
         if(i == j && j == k && k == 3)
           bContinue = false;
      }
    }
  }
  cout << i << "," << j << "," << k << " flag with i,j,k side effects\n";

 // nested loop break method 3: c-code flag with side effect compensation
  bContinue = true;
  for(i = 0; bContinue && i < N; i++) {
    for(j = 0; bContinue && j < N; j++) {
      for(k = 0; bContinue && k < N; k++) {
         if(i == j && j == k && k == 3) {
           bContinue = false;
           i--, j--, k--; // compensate for bottom of loop i,j,k ++ side effect
         }
      }
    }
  }
  cout << i << "," << j << "," << k << " flag with side effect compensation\n";



  // nested loop break method 4: flag without side effects
  bool bKeepGoing = true;
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      for(k = 0; k < N; k++) {
         if(i == j && j == k && k == 3) {
           bKeepGoing = false;
           break;              // need break to prevent k++ side effect
         }
      }
      if(!bKeepGoing) break;   // need break to prevent j++ side effect
    }
    if(!bKeepGoing) break;     // need break to prevent i++ side effect
  }
  cout << i << "," << j << "," << k << " flag+break\n";

  // nested loop break method 5: setjmp-longjmp
  jmp_buf jb;
  if(setjmp(jb) == 0) {
    for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++) {
        for(k = 0; k < N; k++) {
           if(i == j && j == k && k == 3)
             longjmp(jb, 1);
        }
      }
    }
  }
  cout << i << "," << j << "," << k << " setjmp-longjmp\n";

  // nested loop break method 6: c++ throw
  try {
    for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++) {
        for(k = 0; k < N; k++) {
           if(i == j && j == k && k == 3)
             throw nullptr;
        }
      }
    }
  } catch(...) {}
  cout << i << "," << j << "," << k << " try-throw-catch\n";


  // nested loop break method 7: c++11 anonymous function return
  [&] {
   for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++) {
        for(k = 0; k < N; k++) {
           if(i == j && j == k && k == 3)
             return;
        }
      }
    }
  } ();
  cout << i << "," << j << "," << k << " C++11 anonymous function return\n";

}
