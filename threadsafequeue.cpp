#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <chrono>      // time functions
// #include <cstdlib>     // random, compiles fine without cstdlib
using namespace std;

#include "threadsafequeue.h"

// http://gnodebian.blogspot.ca/2013/07/a-thread-safe-asynchronous-queue-in-c11.html
 
#if 0
// factory main test program

int main()
{
  SafeQueue <int> my_queue;
 
  my_queue.push(1);
 
  return 0;
}
#endif

int main()  // GB made this test program
{
  SafeQueue <double> my_queue;
  vector <std::thread> t;
 
  auto push = [&my_queue] (int arg) { 
    int nap = random()%1000; 
    this_thread::sleep_for (std::chrono::milliseconds(nap)); 
    double d = arg + 0.1;
    my_queue.push(d);
    stringstream ss; 
    ss << "thread " << arg << " napped " << nap << " msec + pushed " << d << "\n"; 
    cout << ss.str(); 
  };

  for(int i = 0; i < 21; i++)
    t.push_back( std::thread(push,i) );
  for(auto &e : t) e.join();
 
  cout << "\n";

  auto pop = [&my_queue] (int arg) { 
    double d; 
    my_queue.pop(d); 
    stringstream ss; 
    ss << "thread " << arg << " popped " << d << "\n"; 
    cout << ss.str(); 
  };

  t.clear();
  for(int i = 0; i < 21; i++)
    t.push_back( std::thread(pop,i) );
  for(auto &e : t) e.join();
 
  return 0;
}
