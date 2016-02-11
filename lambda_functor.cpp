#include <iostream>
using namespace std;

int global;
int main(int argc, char**argv)
{

  // lambda:
  // [=] (int x) -> decltype(x) { return x+y; }

  int (*fg)(int x); 
  fg = [=] (int x) -> decltype(x) { return x+global; };  // global compiles

  global = 6;
  cout << "lambda=" << fg(9) << "\n";

  // int local;

  // int (*fl)(int x); 
  // fl = [=] (int x) -> decltype(x) { return x+local;  };  // wants to capture all globals by value
  // fl = [local] (int x) -> decltype(x) { return x+local;  };  // wants to capture global local

  // local = 15;
  // cout << "lambda=" << fl(25) << "\n";

  struct lambda_like {
    int operator() (int x) { return x + y; }
    int y;
  } functor;

  functor.y = 90;
  cout << "functor=" << functor(60) << "\n";
}
