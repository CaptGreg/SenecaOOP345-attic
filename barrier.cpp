// https://en.cppreference.com/w/cpp/experimental/barrier

#include <chrono>                  // sleep_for
// #include <experimental/barrier>    // MISSING FILE 2019-04-25 g++-7.3.0 g++-8.2.0 clang++-6.0.0-1ubuntu2``
// #include <barrier>                 // MISSING FILE 2019-04-25 g++-7.3.0 g++-8.2.0, clang++-6.0.0-1ubuntu generates error
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
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
   bool arrive_and_wait() { // return true when all at barrier, false otherwise.
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
      cv.wait(lock, [=] { return gen == generation; } );
      return false;
   }
};

int main(int argc, char**argv)
{
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
