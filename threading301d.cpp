#include <iostream>
#include <chrono>
#include <future>
#include <string>

using namespace std;

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Reset() { start = std::chrono::high_resolution_clock::now(); }
  void Print(string msg) {
    stop  = std::chrono::high_resolution_clock::now();
    typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    long tWait = duration_get.count();
    std::cout << msg << tWait << " msecs\n";
  }
};

thread::id main_thread_id = this_thread::get_id();  // in case we need to know main's thread id

int main(int argc, char**argv) // package_task + async
{
  Timer timer;
  cout << "main thread ID=" << main_thread_id << "\n";
  auto sleepS  = [] (int nap) { this_thread::sleep_for (std::chrono::seconds(nap)); };
  auto sleepMS = [] (int nap) { this_thread::sleep_for (std::chrono::milliseconds(nap)); };
  auto newThread = [=] () { cout << (this_thread::get_id() == main_thread_id? "*** MAIN THREAD\n": "*** NEW THREAD\n"); };

  /////////////////////////////////////////////////////////////////////////////
  /// PACKAGED TASK vs ASYNC
  /// http://stackoverflow.com/questions/18143661/what-is-the-difference-between-packaged-task-and-async

  auto sleep = [sleepMS,newThread]() { newThread(); sleepMS(1000); return 123; };

  /// PACKAGED TASK
  cout << "/// PACKAGED TASK\n";
  std::packaged_task<int()> task(sleep);

  auto fTask = task.get_future();
  timer.Reset();
  task(); // invoke the function
  timer.Print("task() time=");


  timer.Reset();
  std::cout << fTask.get() << "\n";
  timer.Print("fTask.get() time=");

  cout << "packaged_task<int()> task2(sleep);\n";
  packaged_task<int()> task2(sleep);
  cout << "auto fTask2 = task.get_future();\n";
  auto fTask2 = task2.get_future();
  cout << "thread t( std::move(task2) );\n";
  thread t( std::move(task2) );
  cout << "wait for new thread to run task2 and return result-->";
  cout << fTask2.get() << "\n";
  t.join();

  /// ASYNC
  cout << "/// ASYNC\n";
  timer.Reset();
  auto fAsync = std::async(std::launch::async, sleep);
  timer.Print("fAsync() time=");

  std::cout << "You can see this immediately!\n";

  timer.Reset();
  std::cout << fAsync.get() << "<-- This will be shown after a second!\n";
  timer.Print("fAsync.get() time=");

  std::cout << "********************* END OF MAIN\n";
}
