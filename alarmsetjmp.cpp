// http://web.eecs.utk.edu/~huangj/cs360/360/notes/Setjmp/lecture.html

#include <csignal>
#include <unistd.h>  // alarm
#include <csetjmp>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;


int inner, outer;
time_t tStart;
jmp_buf Env;

void signal_handler(int sig)
{
  if(sig == SIGALRM) {
    time_t tElapsed;

    tElapsed = time(0) - tStart;

    cout << tElapsed << " second" << (tElapsed == 1? "": "s");

    cout << ":  outer loop = " << outer << "/10000, inner loop = " << inner << "/1000000\n";

    if (tElapsed >= 4) {
      cout <<"signal_handler says \"Give up\"\n";
      longjmp(Env, 1);
    }
    alarm(1);
    signal(SIGALRM, signal_handler);
  }
}

int main(int argc, char**argv)
{
  signal(SIGALRM, signal_handler);
  int seconds = 1;
#ifdef __MSVC__
  SetTimer(NULL, 0, 1000*seconds,(TIMERPROC) &signal_handler); 
#else
  alarm(seconds);
#endif

  if (setjmp(Env) != 0) {
    cout << "received signal to \"Give up:\"  outer loop = " << outer << "/10000, inner loop = " << inner << "/1000000\n";
    return 1;
  }

  tStart = time(0);

  for (outer = 0; outer < 10*1000; outer++) {
    for (inner = 0; inner < 1000*1000; inner++);
  }
}

