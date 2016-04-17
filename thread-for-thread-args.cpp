//////////////////////////////////////////////////////////////////////////////
//  Read this file third                                                    //
//  file 'thread-for-thread-args.cpp'                                       //
//    thread/join non-void function passing args+return value in a struct.  //
//////////////////////////////////////////////////////////////////////////////

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


+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ for loop threading thread/join where functure returns a value +
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

step 1.
  * take the for loop and turn the body into a function
    for example, take this code:
      count = 0;
      for(int i = 0; i < N; i++) {
         count += something based on the loop index 'i'
      }

  * note 'N' is the size of the problem
          
  * replace the for loop with a function 'doWork'

    Use a structure to hold the arguments and return value.
      struct Args {
        size_t start;  // arg
        size_t end;    // arg
        int    count;
      };
      void doWork(struct Args* a) {
        a->count = 0;
        for(int i = a->start; i < a->end; i++) {
          a->count +=  something based on the for loop index i
      }

  * call the function
      Args a;
      a.start = 0;
      a.end   = N;
      doWork(&a);

      a.count contains the returned value;

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
  * declare a array to hold the future
    std::future<int> fut[N]; // suppose doWork returns 'int'

step 6.
  * declare a structure array to hold the args and return value for each thread
    Args args[N];

step 7.
  * launch the threads using thread/join
  * thread(...) step:
    for(int t = 0; t < NUM_THREADS; t++) {
      args[t].start  = t * chunk;
      args[t].end    = args[t].start + chunk;
      if(t == NUM_THREADS -1 )
        args[t].end = N;

      fut[t] = async(launch::async, doWork, args+t);
    }

step 8.
  * wait for the threads to finish
  * get() step:
    count = 0;
    for(int t = 0; t < NUM_THREADS; t++) {
      tid[t].join();
      count += args[t].count;
    }

    count hold the count calculated by all the threads

*/

#include <iostream>   // cout
#include <cmath>      // problem needs sqrt
#include <thread>     // thread/join
using namespace std;

int main()
{
  size_t N = 26;

  cout << "\nProblem for loop\n";
  int count = 0;
  for(size_t i = 0; i < N ; i++) {
    int val = sqrt(i); 
    if(val * val == i) {
      count++;
      cout << to_string(i) + " is a perfect square\n";
    }
  }
  cout << to_string(count) + " perfect squares between 0 and " + to_string(N) + "\n";

  // use struct to pass args and return value
  struct Args {
    size_t start;  // arg
    size_t end;    // arg
    int    count;  // return value
  };

  auto  doWork = [] (struct Args* a) {
    a->count = 0;
    for(size_t i = a->start; i < a->end ; i++) {
      int val = sqrt(i); 
      if(val * val == i) {
        a->count++;
        cout << to_string(i) + " is a perfect square\n";
      }
    }
  };

  cout << "\nProblem for loop * converted to doWork(struct Args* a) function\n";
  struct Args a;
  a.start = 0;
  a.end = N;
  doWork(&a);
  cout << to_string(a.count) + " perfect squares between 0 and " + to_string(N) + "\n";


  // Thread using thread/join
  cout << "\nProblem threaded thread/join doWork with args/return values passed using a structure\n";
  int NUM_THREADS = thread::hardware_concurrency();

  thread tid [NUM_THREADS];
  Args   args[NUM_THREADS];  // args are different for each thread

  size_t chunk = (N + NUM_THREADS-1) / NUM_THREADS;  // round up
  for(int t = 0; t < NUM_THREADS; t++) {
    args[t].start  = t * chunk;
    args[t].end    = args[t].start + chunk;
    if(t == NUM_THREADS -1 )
      args[t].end = N;

    tid[t] = thread(doWork, args+t);
  }
  count = 0;
  for(int t = 0; t < NUM_THREADS; t++) {
    tid[t].join();
    count += args[t].count;
  }

  cout << to_string(a.count) + " perfect squares between 0 and " + to_string(N) + "\n";
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

Problem for loop * converted to doWork(struct Args* a) function
0 is a perfect square
1 is a perfect square
4 is a perfect square
9 is a perfect square
16 is a perfect square
25 is a perfect square
6 perfect squares between 0 and 26

Problem threaded thread/join doWork with args/return values passed using a structure
9 is a perfect square
0 is a perfect square
1 is a perfect square
4 is a perfect square
16 is a perfect square
25 is a perfect square
6 perfect squares between 0 and 26

*/
