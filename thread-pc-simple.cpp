// thread-pc-simple.cpp
// a simple producer consumer threading model, mutex free, spin wait for buffer empty or available
// single producer produces data for a single consumer.

#include <chrono>      // chrono::milliseconds(ms)
#include <csignal>     // SIGTERM
#include <iostream>
#include <pthread.h>   // pthread_kill resolved without header, perhaps <thread> includes it
#include <string>
#include <thread>      // thread, join, yield, sleep_for, native_handle
using namespace std;

class PC {
  bool   bufValid = false; // true data available for consumer, false needs filling by producer
  int    buf      = 0;
  bool   stop     = false;
  thread pTid;
  thread cTid;
  bool   pAlive   = false;
  bool   cAlive   = false;

  void waitEmpty()     { while(    bufValid and not stop); }  // spin waiting for empty buffer
  void waitAvailable() { while(not bufValid and not stop); }  // spin waiting for data

  void producer()
  {
    pAlive = true;
    while(not stop) {
      waitEmpty();    // wait for empty buffer
      if(stop) break;

      buf++; // create some data
      bufValid = true;
    }
    pAlive = false;
  }

  void consumer()
  {
    cAlive = true;
    while(not stop) {
      waitAvailable(); // wait for data
      if(stop) break;

      // cout << to_string(buf) + "\n"; // do something with it
      bufValid = false;
    }
    cAlive = false;
  }

public:
  int getBuf() { return buf; }

  void run_ms(int ms)
  {
    pTid = thread(&PC::producer, this); // parm 1: member function
    cTid = thread(&PC::consumer, this); // parm 2: instance

    this_thread::sleep_for(chrono::milliseconds(ms));
    stop = true;
    // this_thread::sleep_for(chrono::milliseconds(1));  // main thread: NAP
    this_thread::yield();                                // main thread: GIVE UP CPU

    if(pAlive) {cout<<"killing producer\n"; pthread_kill(pTid.native_handle(), SIGTERM); }
    if(cAlive) {cout<<"killing consumer\n"; pthread_kill(cTid.native_handle(), SIGTERM); }

    pTid.join();
    cTid.join();
  }
};

class Main {
public:
  static int start(int argc, char*argv[])
  {
    PC pc;

    pc.run_ms(1000);
    cout<< "buf=" << pc.getBuf() << "\n";  // prints 6-8 Million on 2014 era I5 laptop
    return 0;
  }
};

int main(int argc, char*argv[])
{
  return Main::start(argc,argv);
}
