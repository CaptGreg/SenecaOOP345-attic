// http://web.eecs.utk.edu/~huangj/cs360/360/notes/Setjmp/lecture.html

#include <csignal>
#include <sys/types.h> // for kill
#include <unistd.h>    // getpid, alarm
#include <iostream>
using namespace std;


int inner, outer;
time_t tStart;

void signal_handler(int sig)
{
  if(sig == SIGALRM) {
    time_t tElapsed;

    tElapsed = time(0) - tStart;

    cout << tElapsed << " second" << (tElapsed == 1? "": "s");

    cout << ":  outer loop = " << outer << "/10000, inner loop = " << inner << "/1000000\n";

    if (tElapsed >= 4) {
      cout <<"signal_handler says \"Die! Die! Die!\" (sending kill signal)\n";
      kill(getpid(), SIGKILL);
    }
    alarm(1);
    signal(SIGALRM, signal_handler);
  }
}

int main(int argc, char**argv)
{

  signal(SIGALRM, signal_handler);
  alarm(1);

  tStart = time(0);

  for (outer = 0; outer < 10000; outer++) {
    for (inner = 0; inner < 1000000; inner++);
  }
}

