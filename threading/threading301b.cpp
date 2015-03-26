#include <iostream>
#include <chrono>
#include <future>
#include <string>

using namespace std;

thread::id main_thread_id = this_thread::get_id();  // in case we need to know main's thread id

int main(int argc, char**argv) // futures + async
{
  auto sleepS  = [] (int nap) { this_thread::sleep_for (std::chrono::seconds(nap)); };
  auto sleepMS = [] (int nap) { this_thread::sleep_for (std::chrono::milliseconds(nap)); };

  auto newThread = [=] () { 
    cout << (this_thread::get_id() == main_thread_id? 
    "*** MAIN THREAD  ": "*** NEW THREAD  "); 
  };

  auto DoWork  = [sleepS,newThread] (string id) { 
    cout << id << " "; newThread(); cout << " start\n"; 
    sleepS(1); 
    cout << id << " "; newThread(); cout << " stop\n"; 
  };

  /////////////////////////////////////////////////////////////////////////////
  /// ASYNC

  future<int> f = async([newThread]() -> int  { cout << "f "; newThread(); return 123; });
  cout << "f.get=" << f.get() << endl;

  future<int> futures[] = {
    async([newThread]() { cout << "\nf1: "; newThread(); return 123; }),
    async([newThread]() { cout << "\nf2: "; newThread(); return 456; }),
    async([newThread]() { cout << "\nf3: "; newThread(); return 789; })
  };
  // for(future<int> f: futures) cout << f.get() << " ";  cout << endl;
  cout << "\ncalling futures[0].get() " << futures[0].get() << " ";  
  cout << "\ncalling futures[1].get() " << futures[1].get() << " ";  
  cout << "\ncalling futures[2].get() " << futures[2].get() << " ";  
  cout << endl;

  // runs on current thread when you “get” value (i.e. lazy execution):
  future<int> f1 = std::async( std::launch::deferred, []() -> int {return 321;} );

  // runs now on a new, dedicated thread:
  future<int> f2 = std::async( std::launch::async, []() -> int {return 654;} );

  // let system decide (e.g. maybe you created enough work to keep system busy?):
  future<int> f3 = std::async( []() -> int {return 987;} );

  cout << "f1.get=" << f1.get() << endl;
  cout << "f2.get=" << f2.get() << endl;
  cout << "f3.get=" << f3.get() << endl;


  cout << "\nstd::async - std::launch::deferred\n"; 
  auto f1DoWork1 = std::async(std::launch::deferred, DoWork, "DoWork1");
  auto f1DoWork2 = std::async(std::launch::deferred, DoWork, "DoWork2");
  cout << "calling f1DoWork1.get();\n";
  f1DoWork1.get();
  cout << "f1DoWork1.get(); returned\n";
  cout << "calling f1DoWork2.get();\n";
  f1DoWork2.get();
  cout << "f1DoWork2.get(); returned\n";

  cout << "\nstd::async - std::launch::async\n"; 
  auto f2DoWork1 = std::async(std::launch::async, DoWork, "DoWork1");
  auto f2DoWork2 = std::async(std::launch::async, DoWork, "DoWork2");
  cout << "main sleeping for 1 sec.\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));
  cout << "calling f2DoWork1.get();\n";
  f2DoWork1.get();
  cout << "f2DoWork1.get(); returned\n";
  cout << "calling f2DoWork2.get();\n";
  f2DoWork2.get();
  cout << "f2DoWork2.get(); returned\n";

  cout << "\nstd::async - default, let the system decide\n"; 
  auto f3DoWork1 = std::async(DoWork, "DoWork1");
  auto f3DoWork2 = std::async(DoWork, "DoWork2");
  cout << "calling f3DoWork1.get();\n";
  f3DoWork1.get();
  cout << "f3DoWork1.get(); returned\n";
  cout << "calling f3DoWork2.get();\n";
  f3DoWork2.get();
  cout << "f3DoWork2.get(); returned\n";

  std::cout << "********************* END OF MAIN\n";
}
