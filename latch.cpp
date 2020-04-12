#include <condition_variable>
#include <future>        
#include <iostream>         
#include <mutex>         
#include <thread>        
 
// #include  <latch> // c++20
//  2020-04-12
//  not in g++-9.3 (GCC) 9.3.0
//  not in clang version 9.0.0-2~ubuntu18.04.2

#if 0
  Thread support library
   
  std::latch
  Defined in header <latch>
      
  class latch;
      (since C++20)

  The latch class is a downward counter of type ptrdiff_t which can be used to synchronize threads. The value of the counter is initialized on creation. Threads may block on the latch until the counter is decremented to zero. There is no possibility to increase or reset the counter, which makes the latch a single-use barrier.

  Concurrent invocations of the member functions of latch, except for the destructor, do not introduce data races.

  Unlike std::barrier, std::latch can be decremented by a participating thread more than once.
  Member functions (constructor)
    constructs a latch
  (public member function) (destructor)
    destroys the latch
  (public member function) operator= [deleted]
    latch is not assignable
  (public member function) count_down
    decrements the counter in a non-blocking manner
  (public member function) try_wait
    tests if the internal counter equals zero
  (public member function) wait
    blocks until the counter reaches zero
  (public member function) arrive_and_wait
    decrements the counter and blocks until it reaches zero
  (public member function) Constants max
  [static]
    the maximum value of counter supported by the implementation

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

}
