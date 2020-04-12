// https://austingwalters.com/multithreading-semaphores/

// GB does this code implement a semaphore?
// 15 threads wait their turn, executing sequentially.

/**
 * Semaphore example, written in C++ May 4, 2014
 * Compiled on OSX 10.9, using:
 * g++ -std=c++11 semaphore.cpp
 **/

#include <chrono>      
#include <condition_variable>
#include <iostream>      
#include <mutex>         
#include <thread>        
 
std::mutex              mtx;              // mutex for critical section
std::condition_variable cv;               // condition variable for critical section  
bool                    ready = false;    // Tell threads to run
int                     current = 0;      // current count

/* Prints the thread id / max number of threads */
void print_num(int num, int max) {
  // GB waits on cv for global variable current to be equal to num.

  // std::this_thread::sleep_for(std::chrono::milliseconds(1000*(max - num)));
  std::unique_lock<std::mutex> lck(mtx);
  while(num != current || !ready){ cv.wait(lck); }
  current++;
  std::cout << "Thread: "
            << num + 1 << " / " << max
            << " bumped count to: "
            << current << "\n";
  
  /* Notify next threads to check if it is their turn */
  cv.notify_all(); 
}

/* Changes ready to true, and begins the threads printing */
void run(){
  // current is 0.
  // thread 0 will pass the cv wait, bump current, notify all
  // thread 1 will pass the cv wait, bump current, notify all
  // ...

  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  cv.notify_all();
}
 
int web (){

  int         threadnum = 15;
  std::thread threads[15];

  /* spawn threadnum threads */
  for (int id = 0; id < threadnum; id++)
    threads[id] = std::thread(print_num, id, threadnum);

  std::cout << "\nRunning " << threadnum << " thread in parallel: \n" << "\n";

  run(); // Allows threads to run

  /* Merge all threads to the main thread */
  for(auto& e: threads)
      e.join();

  std::cout << "\nCompleted semaphore example!\n\n";

  return 0;
}

// #include  <semaphore> // c++20
//  2020-04-12
//  not in g++-9.3 (GCC) 9.3.0
//  not in clang version 9.0.0-2~ubuntu18.04.2

#if 0
  std::counting_semaphore, std::binary_semaphore
  Thread support library
  std::counting_semaphore
   
  Defined in header <semaphore>
      
  template<std::ptrdiff_t LeastMaxValue = /* implementation-defined */>
  class counting_semaphore;
    (1) 	(since C++20)
  using binary_semaphore = std::counting_semaphore<1>;
    (2) 	(since C++20)
      
  1) A counting_semaphore is a lightweight synchronization primitive that can control access to a shared resource. Unlike a std::mutex, a counting_semaphore allows more than one concurrent access to the same resource, for at least LeastMaxValue concurrent accessors. The program is ill-formed if LeastMaxValue is negative.
  2) binary_semaphore is an alias for specialization of std::counting_semaphore with LeastMaxValue being 1. Implementations may implement binary_semaphore more efficiently than the default implementation of std::counting_semaphore.

  A counting_semaphore contains an internal counter initialized by the constructor. This counter is decremented by calls to acquire() and related methods, and is incremented by calls to release(). When the counter is zero, acquire() blocks until the counter is incremented, but try_acquire() does not block; try_acquire_for() and try_acquire_until() block until the counter is incremented or a timeout is reached.

  Similar to std::condition_variable's wait(), counting_semaphore's try_acquire() can spuriously fail.

  The template class std::counting_semaphore is not DefaultConstructible, CopyConstructible, MoveConstructible, CopyAssignable, or MoveAssignable.
  Member functions
  (constructor) constructs a counting_semaphore
  (public member function) (destructor)
    destructs the counting_semaphore
  (public member function) operator= [deleted]
    counting_semaphore is not assignable
  (public member function) release
    increments the internal counter and unblocks acquirers
  (public member function) acquire decrements the internal counter or blocks until it can
  (public member function) try_acquire
    tries to decrement the internal counter without blocking
  (public member function) try_acquire_for
    tries to decrement the internal counter, blocking for up to a duration time
  (public member function) try_acquire_until
    tries to decrement the internal counter, blocking until a point in time
  (public member function) Constants max [static]
    returns the maximum possible value of the internal counter
  (public static member function)

  Notes

  As its name indicates, the LeastMaxValue is the minimum max value, not the actual max value. Thus max() can yield a number larger than LeastMaxValue.

  Unlike std::mutex a counting_semaphore is not tied to threads of execution - acquiring a semaphore can occur on a different thread than releasing the semaphore, for example. All operations on counting_semaphore can be performed concurrently and without any relation to specific threads of execution, with the exception of the destructor which cannot be performed concurrently but can be performed on a different thread.

  Semaphores are also often used for the semantics of signalling/notifying rather than mutual exclusion, by initializing the semaphore with ​0​ and thus blocking the receiver(s) that try to acquire(), until the notifier "signals" by invoking release(n). In this respect semaphores can be considered alternatives to std::condition_variables, often with better performance.

  #endif

int main (int argc, char* argv[])
{
  // #pragma message "Compiling " __FILE__  " at : " __TIME__ " on " __DATE__
  std::cout <<"+++++++++++++++++++++++++++++++\n";
  std::cout << argv[0] << "\n";
  std::cout << "FILE '" __FILE__ "' compiled " __DATE__ " at " __TIME__  " on " __DATE__ 
    #ifdef __GNUC__  // either g++ or clang++
      #ifdef __clang__
        " by clang++ "
      #else
        " by g++ "
      #endif
       __VERSION__
    #endif
       "\n\n";

  web();
}
