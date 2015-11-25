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
  cout << i << "," << j << "," << k << "\n";


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
  cout << i << "," << j << "," << k << " <--- NOTE i,j,k values are NOT 3,3,3! (side effects, side effects)\n";


  // nested loop break method 3: flag without side effects
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
  cout << i << "," << j << "," << k << " <--- side effects fixed!\n";

  // nested loop break method 4: c++ throw
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
  cout << i << "," << j << "," << k << "\n";


  // nested loop break method 5: c++11 lambda return
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
  cout << i << "," << j << "," << k << "\n";

}
