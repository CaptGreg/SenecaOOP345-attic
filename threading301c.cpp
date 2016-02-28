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

  auto dfun = [](double x) -> double { return 2*x; };
  auto ifun = [](int    x) -> int    { return 2*x; };

  /////////////////////////////////////////////////////////////////////////////
  /// PACKAGED TASK
  packaged_task<double(double)> double_packaged_task( dfun ); // double(double) => function returns double, double arg
  auto fut = double_packaged_task.get_future();

  thread t1 = thread(std::move( double_packaged_task), 42.75);
  t1.join();
  // double_packaged_task(42.75);
  cout << "double_packaged_task...get=" << fut.get() << "\n";

  packaged_task<int(int)> int_packaged_task( ifun ); // int(int) => function returns int, int arg
  auto fut2 = int_packaged_task.get_future();
  thread t2 = thread( std::move( int_packaged_task ), 40); // int(int) => function returns int, int arg
  t2.join();
  // int_packaged_task(40);
  cout << "int_packaged_task...get=" << fut2.get() << "\n";

  #if 0
  auto funSleepMS = [sleepMS](double x) -> double { sleepMS(1500); return 2*x; };

  /////////////////////////////////////////////////////////////////////////////
  /// PACKAGED TASK
  cout << "creating packaged task\n";
  timer.Reset();
  packaged_task<double(double)> pt_sf( funSleepMS ); // pt_sf = package_task sleep function
  timer.Print("create pt_sf ");

  cout << "launching packaged task\n";
  timer.Reset();
  pt_sf(50.);
  thread tid = thread(std::move(pt_sf), 50.);
  timer.Print("thread pt_sf(50.); launched in ");

  cout << "accessing packaged task result\n";
  timer.Reset();
  tid.join();
  cout << "pt_sf.get_future().get() =" << pt_sf.get_future().get() << "\n";
  timer.Print("pt_sf.get_future().get() took ");
  #endif

  std::cout << "********************* END OF MAIN\n";
}
