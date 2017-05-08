#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>  // use string for lock-free writing to cout
#include <cstdlib> // rand
#include <chrono>  // clocks

using namespace std;

class Barrier { // http://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11
  std::mutex m_mutex;
  std::condition_variable m_cv;

  size_t m_count;
  const size_t m_initial;

  enum State : unsigned char { Up, Down };
  State m_state;
public:
  explicit Barrier(std::size_t count) 
    : m_count{ count }, 
      m_initial{ count }, 
      m_state{ State::Down }
    { }

  /// Blocks until all N threads reach here
  void wait()
  {
    std::unique_lock<std::mutex> lock{ m_mutex };

    if (m_state == State::Down) { // Counting down the number of waiting threads
      if (--m_count == 0) {
        m_state = State::Up;
        m_cv.notify_all();
      } else {
        m_cv.wait(lock, [this] { return m_state == State::Up; });
      }
    } else { // (m_state == State::Up) // Counting back up for auto reset
      if (++m_count == m_initial) {
        m_state = State::Down;
        m_cv.notify_all();
      } else {
        m_cv.wait(lock, [this] { return m_state == State::Down; });
      }
    }
  }
};

const int NUM_THREADS = 5;
Barrier barrier(NUM_THREADS+1); // +1 for main

std::chrono::time_point<std::chrono::high_resolution_clock> start;
auto Time = [start] () -> string { // syntax error without ()
  std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
  typedef std::chrono::duration<int,std::micro> microsecs_t; 
  microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(now-start) ) ;
  auto t = duration_get.count();
  return to_string( t ) + " ";
};


void tFun(int tid, int loopCount)
{
  // auto tid = this_thread::get_id();
  auto nap = 1000 + rand() % 2000;
  cout << Time() + string("thread ")  + to_string(tid) + " alive.\n";

  this_thread::sleep_for(chrono::milliseconds(nap));

  for(int count = 0; count < loopCount; count++) {
    cout << Time() + string("thread ")  + to_string(tid) + " count=" + to_string(count) + " at barrier.\n";
    barrier.wait();
    cout << Time() + string("thread ")  + to_string(tid) + " passed barrier.\n";
    this_thread::sleep_for(chrono::milliseconds(nap));
  }
  barrier.wait(); // everyone exit at once
  cout << Time() + string("thread ")  + to_string(tid) + " exiting.\n";

}

int main(int argc, char*argv[])
{
  start = std::chrono::high_resolution_clock::now();  // set start time

  thread tid[NUM_THREADS];
  const int loopCount = 4;

  for(int t = 0; t < NUM_THREADS; t++) {
    tid[t] = thread(tFun, t, loopCount);
    // thread(tFun, t, loopCount).detach();
  }

  for(int count = 0; count < loopCount; count++) {
    cout << Time() + "main waiting at barrier.\n";
    barrier.wait();
  }

  barrier.wait();  // wait for threads to exit
  for(auto &e : tid) e.join();
  cout << Time() + " main exiting.\n";

}
