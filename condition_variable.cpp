// http://www.cplusplus.com/reference/condition_variable/condition_variable/
// GB add sleep + prints

// condition_variable example
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

std::mutex mtxCout;

void print_id (int id) {
  { std::unique_lock<std::mutex> lock(mtx); // GB
    std::cout << "thread " << id << " waiting\n";
  } // GB
  std::unique_lock<std::mutex> lck(mtx);
  while (!ready) cv.wait(lck);
  // ...
  // { std::unique_lock<std::mutex> lock(mtx);
    std::cout << "thread " << id << '\n';
  // }
}

void go() {
  std::cout << "GO!\n";
  std::unique_lock<std::mutex> lck(mtx);
  ready = true;
  // GB either notify_all, or notify_one:
  // cv.notify_all();
  for (int i=0; i<10; ++i)
    cv.notify_one();
}

int main ()
{
  std::thread threads[10];
  // spawn 10 threads:
  for (int i=0; i<10; ++i)
    threads[i] = std::thread(print_id,i);

  std::this_thread::sleep_for (std::chrono::milliseconds(1000));

  std::cout << "\nafter sleeping for 1000 msec, the 10 threads should be ready to race...\n\n";
  go();                       // go!

  for (auto& th : threads) th.join();

  return 0;
}
