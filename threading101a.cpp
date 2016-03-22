#include <iostream>
#include <thread>

using namespace std;

const int NUM_THREADS = 10;

void threadfunction_cout(int arg) 
{
  this_thread::yield();                   // give up the CPU
  cout << "cout: " << arg << "\n";

  this_thread::yield();                   // give up the CPU
  cout << "cout: ";

  this_thread::yield();                   // give up the CPU
  cout << arg;

  this_thread::yield();                   // give up the CPU
  cout << "\n";

  this_thread::yield();                   // give up the CPU
}

int main() 
{
  int arg;
  thread t[NUM_THREADS];

  cout << "\nthreads using cout:\n\n";
  // launch threads
  arg = 100;
  for (int i = 0; i < NUM_THREADS; ++i)
    t[i] = thread(threadfunction_cout, arg++); // pass i as arg to thread

  // wait for thread to complete
  for (int i = 0; i < NUM_THREADS; ++i)
    t[i].join();

  return 0;
}
