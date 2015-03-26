// http://www.cplusplus.com/reference/mutex/timed_mutex/try_lock_until/ 

// timed_mutex::try_lock_until example
#include <iostream>       // std::cout
#include <chrono>         // std::chrono::system_clock
#include <thread>         // std::thread
#include <mutex>          // std::timed_mutex
#include <ctime>          // std::time_t, std::tm, std::localtime, std::mktime

std::timed_mutex cinderella;

// gets time_point for next midnight:
std::chrono::time_point<std::chrono::system_clock> midnight() {
  using std::chrono::system_clock;
  std::time_t tt = system_clock::to_time_t (system_clock::now());
  struct std::tm * ptm = std::localtime(&tt);
  ++ptm->tm_mday; ptm->tm_hour=0; ptm->tm_min=0; ptm->tm_sec=0;
  return system_clock::from_time_t (mktime(ptm));
}

void carriage() {
  if (cinderella.try_lock_until(midnight())) {
    std::cout << "ride back home on carriage\n";
    cinderella.unlock();
  }
  else
    std::cout << "carriage reverts to pumpkin\n";
}

void ball() {
  cinderella.lock();
  std::cout << "at the ball...\n";
  cinderella.unlock();
}

int main ()
{
  std::thread th1 (ball);
  std::thread th2 (carriage);

  th1.join();
  th2.join();

  return 0;
}
