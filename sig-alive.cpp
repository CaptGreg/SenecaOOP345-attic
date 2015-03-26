// http://stackoverflow.com/questions/16024774/how-do-i-use-the-sigalrm-correctly

// GB converted to C++ and cleaned up
// need windows alarm(1) equivalent

#include <csignal> // signal, raise
#include <iostream>
#include <unistd.h>  // alarm(1)
using namespace std;

unsigned Count = 0; //Counts the number of times it receives the signal SIGINT.
bool sigAck = false;
void signalCallback(int sign); //prototype of the function my pause.

void signalCallback(int sig) {
  // is cout re-enterant?
  sigAck = true;
  switch (sig) {
  case SIGALRM:
    alarm(1);
    signal(SIGINT, signalCallback);
    cout << "\nsignalCallback: I'm alive\n";
    break;
  case SIGINT:
    cout << "\nsignalCallback: SIGINT (Pressed Ctrl-C)\n";
    cout << "signalCallback: I'm running, waiting for a signal\n";
    Count++;
    if(Count==5) exit(0); //Exit program.
    break;
  case SIGQUIT:
    cout << "\nsignalCallback: SIGQUIT\n";
    cout << "signalCallback: You pressed Ctrl-C " << Count << " times\n";
    exit(0); //Exit program.
    break;
  default:
    cout << "\nsignalCallback: caught signal: " << sig << "\n";
    break;
  }
}

int main(int argc, char**argv) 
{
  signal(SIGALRM, signalCallback);
  // signal(SIGINT, signalCallback);
  // signal(SIGQUIT, signalCallback);
  alarm(1);
  cout << "\nI'm running waiting for a signal\n";
  while (1) {
    if(sigAck) {
      sigAck=false;
      cout << "\nmain: acknowledging signal received\n";
    }
  }
  return (0);
}
