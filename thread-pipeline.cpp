#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <cstdlib> // rand
#include <chrono>

class Barrier { // http://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
  std::mutex              m;
  std::condition_variable cv;

  size_t                  count;
  const size_t            initial;

  enum State : uint8_t    { Up, Down };
  State                   state;
public:
  explicit Barrier(std::size_t count) : count{ count }, initial{ count }, state{ State::Down } { }

  void wait() /// Blocks until all N threads call wait
  {
    std::unique_lock<std::mutex> lock{ m };

    if (state == State::Down) { // Counting down the number of waiting threads
      if (--count == 0) {
        state = State::Up;
        cv.notify_all();
      } else {
        cv.wait(lock, [this] { return state == State::Up; });
      }
    } else { // (state == State::Up) // Counting back up for auto reset
      if (++count == initial) {
        state = State::Down;
        cv.notify_all();
      } else {
        cv.wait(lock, [this] { return state == State::Down; });
      }
    }
  }
};

const int NUM_THREADS = 5;
Barrier barrier(NUM_THREADS+1); // +1 for main

auto Time = [] {
  auto now = std::chrono::high_resolution_clock::now();
  static bool initialized = false;
  static decltype(now) epoch;
  if(not initialized) {
    epoch = std::chrono::high_resolution_clock::now();
    initialized = true;
  }

  typedef std::chrono::duration<int,std::micro> microsecs_t; 
  microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(now-epoch) ) ;
  auto t = 1e-6*duration_get.count();

  return std::to_string( t ) + " ";
};

void tFun(int threadNum, int loopCount)
{
  // auto threadNum = this_thread::get_id();
  auto nap = 1000 + rand() % 2000;
  std::cout << Time() + std::string("thread ")  + std::to_string(threadNum) + " alive.\n";

  std::this_thread::sleep_for(std::chrono::milliseconds(nap));

  std::string ID(std::string("thread ") + std::to_string(threadNum));
  for(int count = 0; count < loopCount; count++) {
    std::cout << Time() + ID + " count=" + std::to_string(count) + " at barrier.\n";
    barrier.wait();
    std::cout << Time() + ID + " passed barrier.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(nap));
  }
  barrier.wait(); // everyone exit at once
  std::cout << Time() + ID + " exiting.\n";
}

int main(int argc, char*argv[])
{
  std::thread tid[NUM_THREADS];
  const int loopCount = 4;

  for(int t = 0; t < NUM_THREADS; t++) {
    tid[t] = std::thread(tFun, t, loopCount);
    // thread(tFun, t, loopCount).detach();
  }

  for(int count = 0; count < loopCount; count++) {
    std::cout << Time() + "main waiting at barrier.\n";
    barrier.wait();
  }

  barrier.wait();  // wait for threads to exit
  for(auto &e : tid) e.join();
  std::cout << Time() + " main exiting.\n";
}
