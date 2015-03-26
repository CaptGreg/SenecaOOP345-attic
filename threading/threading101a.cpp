#include <iostream>
#include <thread>

using namespace std;

const int NUM_THREADS = 10;

void threadfunction_cout(int arg) 
{
  cout << "cout: " << arg << "\n";
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
