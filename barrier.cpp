// https://en.cppreference.com/w/cpp/experimental/barrier

#include <chrono>                  // sleep_for
// #include <experimental/barrier>    // MISSING FILE 2019-04-25 g++-7.3.0 g++-8.2.0 clang++-6.0.0-1ubuntu2``
// #include <barrier>                 // MISSING FILE 2019-04-25 g++-7.3.0 g++-8.2.0, clang++-6.0.0-1ubuntu generates error

// wait a year.  Maybe things have improved:
// #include <experimental/barrier>    // MISSING FILE 2020-04-12 g++-9.3 (GCC) 9.3.0, clang++-9.0.0-2~ubuntu18.04.2
// #include <barrier>                 // MISSING FILE 2020-04-12 g++-9.3 (GCC) 9.3.0, clang++-9.0.0-2~ubuntu18.04.2
//
// g++-9 experimental doesn't have barrier either
// ls /usr/include/c++/9/experimental
//   algorithm  buffer    filesystem    io_context  memory           numeric          ratio   source_location  timer          unordered_set
//   any        chrono    forward_list  iterator    memory_resource  optional         regex   string           tuple          utility
//   array      deque     functional    list        net              propagate_const  set     string_view      type_traits    vector
//   bits       executor  internet      map         netfwd           random           socket  system_error     unordered_map
// find /usr/include -name barrier
//   --- nothing found

//   std::barrier
//    
//   C++20
//    
//   Technical specifications
//    
//   Extensions for concurrency
//    
//   std::barrier
//    
//   Defined in header <barrier>
//       
//   class barrier;
//       (concurrency TS)
// 
//   The class std::barrier provides a thread-coordination mechanism that allows a set of participating threads to block until an operation is completed. 
//   Unlike std::latch, barriers are reusable; once the participating threads are released from a barrier's synchronization point, they can reuse the same barrier.
// 
//   A barrier has a completion phase, which is executed by one of the participating threads once all threads in the set of participating threads arrive at the synchronization point. 
//   The arrive_and_wait and arrive_and_drop calls synchronize with the start of the completion phase; the end of the completion phase synchronizes with the returns from all calls blocked by its completion.
// 
//   For std::barrier, the completion phase is empty. std::flex_barrier allows the user to control the completion phase with a function object.
// 
//   The set of participating threads for a barrier constructed for num_threads threads is the first num_threads threads to arrive at its synchronization point after construction. 
//   The same set of threads (except for threads that called arrive_and_drop()) must arrive at the barrier each cycle.
//   Member functions
//   (constructor) constructs a barrier
//   (public member function) (destructor)
//     destroys the barrier
//   (public member function) operator= [deleted]
//     not copy-assignable
//   (public member function) arrive_and_wait
//     arrive at the synchronization point and block
//   (public member function) arrive_and_drop
//     arrive at the synchronization point and remove the current thread from the set of participating threads

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

class barrier { // DIY barrier
 private:
   std::mutex              m;
   std::condition_variable cv;
   volatile uint32_t       threshold;
   volatile uint32_t       count;
   volatile uint32_t       generation = 0;

 public:
   barrier(unsigned int count): threshold(count), count(count) { } 
   bool arrive_and_wait() { 
      // Return true when all at barrier, false otherwise.
      // This allows the caller to do something else if the entire group is not at the barrier.
      std::unique_lock<std::mutex> lock(m);
      uint32_t gen = generation;

      if (--count == 0) {
          generation++;
          count = threshold;
          cv.notify_all();
          return true;
      }

      // while (gen == generation)  // while(not ready) wait
          // cv.wait(lock);
   // cv.wait(lock, [=] { return gen == generation; } ); // TODO: check return true condition, is it == or !=?
      cv.wait(lock, [=] { return gen != generation; } ); // TODO: check return true condition, is it == or !=?
      return false;
   }
};

int main(int argc, char**argv)
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


  barrier b(2);

  auto f = [&] (int i) {
    cout << string("thread ") + to_string(i) + " alive\n";
    this_thread::sleep_for(std::chrono::seconds(1));
    b.arrive_and_wait();
    // b.arrive_and_drop();  // other std::experimental::barrier call
    cout << string("thread ") + to_string(i) + " passed barrier\n";
  };

  thread t1 = thread(f,1);
  thread t2 = thread(f,2);

  t1.join();
  t2.join();
}

