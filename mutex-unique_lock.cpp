// GB adapted from
// http://www.cplusplus.com/reference/mutex/unique_lock/
// GB  added no lock case

// unique_lock example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::unique_lock

std::mutex mtx;           // mutex for critical section

void print_block (int n, char c) {
  std::cout << "thread " << std::this_thread::get_id() << " start\n";
  for (int i=0; i<n; ++i) { 
    std::this_thread::sleep_for(std::chrono::milliseconds(i%10));
    std::cout << c; 
  }
  std::cout << "\nthread " << std::this_thread::get_id() << " all done\n";
}

void print_block_unique_lock (int n, char c) {
  // critical section (exclusive access to std::cout signaled by lifetime of lck):
  std::unique_lock<std::mutex> lck (mtx);
  print_block (n, c);
}

void print_block_lock_guard (int n, char c) {
  // critical section (exclusive access to std::cout signaled by lifetime of lck):
  std::lock_guard<std::mutex> lck (mtx);
  print_block (n, c);
}

int main (int argc, char**argv)
{
  std::cout << "\nwithout lock:\n";
  std::thread th1 (print_block,50,'*');
  std::thread th2 (print_block,50,'$');

  th1.join();
  th2.join();

  std::cout << "\nwith unique_lock:\n";
  std::thread th3 (print_block_lock_guard,50,'*');
  std::thread th4 (print_block_lock_guard,50,'$');

  th3.join();
  th4.join();

  std::cout << "\nwith lock_guard:\n";
  std::thread th5 (print_block_unique_lock,50,'*');
  std::thread th6 (print_block_unique_lock,50,'$');

  th5.join();
  th6.join();

  return 0;
}
