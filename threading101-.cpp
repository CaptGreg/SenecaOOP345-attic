#include <iostream>
#include <thread>

using namespace std;

const int NUM_THREADS = 10;

void threadfunction_cout(int arg) 
{
  cout << "cout: " << arg << "\n";
  cout << "cout: ";
  cout << arg;
  cout << "\n";
}

int main() 
{
  int arg;
  thread tid[NUM_THREADS];

  cout << "\nthreads using cout:\n\n";
  // launch threads
  arg = 100;
  for (int t = 0; t < NUM_THREADS; t++)
    tid[t] = thread(threadfunction_cout, arg++); // pass i as arg to thread

  // wait for thread to complete
  for (int t = 0; t < NUM_THREADS; t++)
    tid[t].join();

  return 0;
}
