// On a 2011 vintage Intel I5 laptop running Ubuntu 10.02 LTS

// This program ran for 30502 threads then:
// terminate called after throwing an instance of 'std::system_error'
//   what():  Resource temporarily unavailable

#include <thread>
#include <iostream>
#include <csignal>
#include <unistd.h>  // sleep
#include <cstdlib>   // exit
using namespace std;

void mainSigHandler(int sig)
{
  if(sig == SIGTERM) {
    cout << "Lousy AIM: main received the SIGTERM signal\n";
    exit(1);
  }
}

void threadSigHandler(int sig)
{
  if(sig == SIGTERM) {
    cout << "thread SIGTERM\n"; // sleep will return
    return;
  }
}

void somefunction()
{
  signal(SIGTERM, threadSigHandler);  // install interrupt handler for SIGTERM
  sleep(3600); // will return if SIGTERM received
  cout << "thread exiting - sleep returned\n";
}

thread t[1000000];

int main()
{
  signal(SIGTERM, mainSigHandler);  // install interrupt handler for SIGTERM
  int i;
  try {
    for(i=0; ; i++) {
	t[i] = thread(somefunction);
	cout << i<< "\n";
    }
  } catch (exception& e) {
    cout << e.what() << "\n";
    cout << "killing off threads\n";
    for(int ii = i-1; ii >= 0; ii--) {
        cout << "killing thread " << ii << "\n";
        pthread_kill(t[ii].native_handle(), SIGTERM);
    }
    // kill(-1, SIGTERM); // This kills everything, including our login session.
    for(int ii = i-1; ii >= 0; ii--) {
        cout << "joining thread " << ii << "\n";
        t[ii].join();
    }
    cout << "max number of threads created = " << i << "\n";
  }
}
