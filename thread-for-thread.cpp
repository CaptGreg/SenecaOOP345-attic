////////////////////////////////////
//  Read this file first          //
//  file 'thread-for-thread.cpp'  //
//    thread/join - void function //
////////////////////////////////////

/*
for loop threading

There are three sample for loop threading eaxmples.

Read these files in the followiung order:

  1st Read and understand file 'thread-for-thread.cpp'       
    thread/join - void function

  2nd Read and understand file 'thread-for-async.cpp'        
    future/async/get - non-void function

  3rd Read and understand file 'thread-for-thread-args.cpp'  
    thread/join non-void function passing args+return value in a struct.

+++++++++++++++++++++++++++++++
+ thread/join - void function +
+++++++++++++++++++++++++++++++

step 1.
  * take the for loop and turn the body into a function
    for example, take this code:
      for(int i = 0; i < N; i++) {
         - do something based on the loop index 'i'
      }

  * note 'N' is the size of the problem
          
  * replace the for loop with a function 'doWork'
      void doWork(int start, int end) { 
        for(int i = start; i < end; i++) {
          - do something based on the for loop index i
      }

  * call the function
      doWork(0, N);

  * should do the same thing

step 2.
  * calculate the number of threads you are going to use
    int NUM_THREADS = std::thread::hardware_concurrency();

step 3.
  * calculate the amount of work each thread will do. (Call it 'chunk')
    either
      int chunk = N / NUM_THREADS;
    or
      int chunk = (N + NUM_THREADS-1) / NUM_THREADS;  // round chunk up to the next integer

step 4.
  * calculate the amount of work each thread will do. (Call it 'chunk')
    int chunk = N / NUM_THREADS;

step 5.
  * declare a array to hold the thread identifier (tid)
    std::thread tid[N];

step 6.
  * launch the threads using thread/join
  * thread(...) step:
    for(int t = 0; i < NUM_THREADS; t++) {
      int s = t * chunk;
      int e = s + chunk;
      if(t == NUM_THREADS-1) // NUM_THREADS may not evenly divide the problem size, 'N'.
        e = N;
      tid[t] = std::thread(doWork, s, e);
    }

step 7.
  * wait for the threads to finish
  * join() step:
    for(auto e ; tid)
      e.join();

*/

#include <iostream>   // cout
#include <cmath>      // problem needs sqrt
#include <thread>     // thread/join
using namespace std;

int main()
{
  size_t N = 26;

  cout << "\nProblem for loop\n";
  for(size_t i = 0; i < N ; i++) {
    int val = sqrt(i); 
    if(val * val == i) {
      cout << to_string(i) + " is a perfect square\n";
    }
  }

  auto  doWork = [] (size_t start, size_t end) { 
    for(size_t i = start; i < end ; i++) {
      int val = sqrt(i); 
      if(val * val == i) {
        cout << to_string(i) + " is a perfect square\n";
      }
    }
  };

  cout << "\nProblem for loop converted to doWork(size_t start, size_t end) function\n";
  doWork(0, N);

  // Thread using thread/join
  cout << "\nProblem threaded thread/join void doWork\n";
  int NUM_THREADS = thread::hardware_concurrency();

  thread tid[NUM_THREADS];

  size_t chunk = (N + NUM_THREADS-1) / NUM_THREADS;  // round up
  for(int t = 0; t < NUM_THREADS; t++) {
    size_t start  = t * chunk;
    size_t end    = start + chunk;
    if(t == NUM_THREADS -1 )
      end = N;

    tid[t] = thread(doWork, start, end);
  }
  for(auto &e : tid)
    e.join();
}

/* 
sample output:

Problem for loop
0 is a perfect square
1 is a perfect square
4 is a perfect square
9 is a perfect square
16 is a perfect square
25 is a perfect square

Problem for loop converted to doWork(size_t start, size_t end) function
0 is a perfect square
1 is a perfect square
4 is a perfect square
9 is a perfect square
16 is a perfect square
25 is a perfect square

Problem threaded thread/join void doWork
9 is a perfect square
0 is a perfect square
1 is a perfect square
4 is a perfect square
25 is a perfect square
16 is a perfect square

*/
