/////////////////////////////////////////////
//  Read this file second                  //
//  file 'thread-for-async.cpp'            //
//    future/async/get - non-void function //
/////////////////////////////////////////////

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

++++++++++++++++++++++++++++++++++++++++
+ future/async/get - non-void function +
++++++++++++++++++++++++++++++++++++++++

step 1.
  * take the for loop and turn the body into a function

    for example, take this code:
      sum = 0
      for(size_t i = 0; i < N; i++) {
         sum += something based on the loop index 'i'
      }

  * note 'N' is the size of the problem
          
  * replace the for loop with a function 'doWork'

      void doWork(size_t start, size_t end) { 
        sum = 0;
        for(size_t i = start; i < end; i++) {
          sum +=  something based on the for loop index i
        return sum;
      }

  * call the function

      sum = doWork(0, N);

  * should do the same thing

step 2.
  * calculate the number of threads you are going to use

    int NUM_THREADS = std::thread::hardware_concurrency();

step 3.
  * calculate the amount of work each thread will do. (Call it 'chunk')

    either
      size_t chunk = N / NUM_THREADS;
    or
      size_t chunk = (N + NUM_THREADS-1) / NUM_THREADS;  // round chunk up to the next integer

step 4.
  * calculate the amount of work each thread will do. (Call it 'chunk')

    int chunk = N / NUM_THREADS;

step 5.
  * declare a futre array, one per thread

    std::future<int> tid[N];   // suppose function doWrok returns an 'int'

step 6.
  * launch the threads using future/async

    for(int t = 0; i < NUM_THREADS; t++) {
      size_t s = t * chunk;
      size_t e = s + chunk;
      if(t == NUM_THREADS-1) // NUM_THREADS may not evenly divide the problem size, 'N'.
        e = N;
      fut[t] = std::async(launch::async, doWork, s, e);
    }

step 7.
  * wait for the threads to finish
  * get() step:

    sum = 0;
    for(auto e ; tid)
      sum += e.get();

*/

#include <iostream>   // cout
#include <cmath>      // problem needs sqrt
#include <future>     // future/async/get
using namespace std;

int main()
{
  size_t N = 26;

  cout << "\nProblem for loop\n";
  int count;
  count = 0;
  for(size_t i = 0; i < N ; i++) {
    int val = sqrt(i); 
    if(val * val == i) {
      count++;
      cout << to_string(i) + " is a perfect square\n";
    }
  }
  cout << to_string(count) + " perfect squares between 0 and " + to_string(N) + "\n";

  auto  doWork = [] (size_t start, size_t end) { 
    int count = 0;
    for(size_t i = start; i < end ; i++) {
      int val = sqrt(i); 
      if(val * val == i) {
        count++;
        cout << to_string(i) + " is a perfect square\n";
      }
    }
    return count;
  };

  cout << "\nProblem for loop * converted to doWork(size_t start, size_t end)  function\n";
  count = doWork(0, N);
  cout << to_string(count) + " perfect squares between 0 and " + to_string(N) + "\n";

  // Thread using thread/join
  cout << "\nProblem threaded future/async/get non-void doWork\n";
  int NUM_THREADS = thread::hardware_concurrency();

  future<int> fut[NUM_THREADS];  // doWork returns 'int'

  size_t chunk = (N + NUM_THREADS-1) / NUM_THREADS;  // round up
  for(int t = 0; t < NUM_THREADS; t++) {
    size_t start  = t * chunk;
    size_t end    = start + chunk;
    if(t == NUM_THREADS -1 )
      end = N;

    fut[t] = async(launch::async, doWork, start, end);
  }
  count = 0;
  for(int t = 0; t < NUM_THREADS; t++) {
    count += fut[t].get();
  }
  cout << to_string(count) + " perfect squares between 0 and " + to_string(N) + "\n";
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
6 perfect squares between 0 and 26

Problem for loop * converted to doWork(size_t start, size_t end)  function
0 is a perfect square
1 is a perfect square
4 is a perfect square
9 is a perfect square
16 is a perfect square
25 is a perfect square
6 perfect squares between 0 and 26

Problem threaded future/async/get non-void doWork
0 is a perfect square
1 is a perfect square
4 is a perfect square
16 is a perfect square
9 is a perfect square
25 is a perfect square
6 perfect squares between 0 and 26

*/
