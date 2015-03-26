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
  cout << "main thread ID=" << main_thread_id << endl;
  auto sleepS  = [] (int nap) { this_thread::sleep_for (std::chrono::seconds(nap)); };
  auto sleepMS = [] (int nap) { this_thread::sleep_for (std::chrono::milliseconds(nap)); };
  auto newThread = [=] () { cout << (this_thread::get_id() == main_thread_id? "*** MAIN THREAD\n": "*** NEW THREAD\n"); };

  /////////////////////////////////////////////////////////////////////////////
  /// PACKAGED TASK

  auto dfun = [](double x) -> double { return 2*x; };
  auto ifun = [](int    x) -> int    { return 2*x; };

  packaged_task<double(double)> double_packaged_task( dfun ); // double(double) => function returns double, double arg
  double_packaged_task(42.75);
  auto fut = double_packaged_task.get_future();
  cout << "double_packaged_task...get=" << fut.get() << endl;

  packaged_task<int(int)> int_packaged_task( ifun ); // int(int) => function returns int, int arg
  int_packaged_task(40);
  cout << "int_packaged_task...get=" << int_packaged_task.get_future().get() << endl;

  auto funSleepMS = [sleepMS](double x) -> double { sleepMS(1500); return 2*x; };

  cout << "creating packaged task\n";
  timer.Reset();
  packaged_task<double(double)> pt_sf( funSleepMS ); // pt_sf = package_task sleep function
  timer.Print("create pt_sf ");

  cout << "launching packaged task\n";
  timer.Reset();
  pt_sf(50.);
  timer.Print("pt_sf(50.); launched in ");

  cout << "accessing packaged task result\n";
  timer.Reset();
  cout << "pt_sf.get_future().get() =" << pt_sf.get_future().get() << endl;
  timer.Print("pt_sf.get_future().get() took ");

  std::cout << "********************* END OF MAIN\n";
}
