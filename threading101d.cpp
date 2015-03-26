#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

const int NUM_THREADS = 10;

void threadfunction_cout(int arg) 
{
  cout << "cout: " << arg << "\n";
}

void threadfunction_stringstream(int arg) 
{
  stringstream ss;
  ss << "stringstream+cout: " << arg << ", this_thread::get_id=" << this_thread::get_id() << "\n";
  cout << ss.str();
}

std::mutex coutLock;
void threadfunction_mutex_lock(int arg) 
{
  coutLock.lock();
    cout << "mutex lock: " << arg << ", this_thread::get_id=" << this_thread::get_id() << "\n";
  coutLock.unlock();
}

void threadfunction_mutex_try_lock(int arg) 
{
  long long count = 0;
  while(true) {
    if(coutLock.try_lock()) {
        cout << "mutex try_lock: " << arg << ", this_thread::get_id=" << this_thread::get_id() << ", tried " << count << " times before we attained the lock" << "\n";
      coutLock.unlock();
      break;
    } else { // lock is busy, do something else.  Let's count the number of times we tried.
      count++;
    }
  }
}

int main() 
{
  int arg;
  thread t[NUM_THREADS];

  cout << "\nthreads using cout:\n\n";
  // launch threads
  arg = 100;
  for (int i = 0; i < NUM_THREADS; ++i)
    t[i] = thread(threadfunction_cout, arg++); // pass i as arg to thread

  // wait for thread to complete
  for (int i = 0; i < NUM_THREADS; ++i)
    t[i].join();

  cout << "\nthreads using stringstream followed by cout:\n\n";
  arg = 200;
  for (auto& th:  t) th = thread(threadfunction_stringstream, arg++);
  for (auto& th:  t) th.join();  // range based for simplifies code...

  cout << "\nthreads using mutex lock:\n\n";
  arg = 300;
  for (auto& th:  t) th = thread(threadfunction_mutex_lock, arg++);
  for (auto& th:  t) th.join();

  cout << "\nthreads using mutex try_lock:\n\n";
  arg = 400; // arg to thread 
  for (auto& th:  t) th = thread(threadfunction_mutex_try_lock, arg++);
  for (auto& th:  t) th.join();

  return 0;
}
