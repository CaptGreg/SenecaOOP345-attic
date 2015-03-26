// http://en.cppreference.com/w/cpp/thread/async
// Notes
// 
// The implementation may extend the behavior of the first overload of std::async by enabling additional (implementation-defined) bits in the default launch policy.
// 
// If the std::future obtained from std::async has temporary object lifetime (not moved or bound to a variable), the destructor of the std::future will block at the end of the full expression until the asynchronous operation completes, essentially code such as the following synchronous:
// 
// std::async(std::launch::async, []{ f(); }); // temporary's dtor waits for f()
// std::async(std::launch::async, []{ g(); }); // does not start until f() completes

// GB NOT TRUE!  'g' DOES NOT WAIT FOR 'f' TO FINISH


#include <thread>
#include <future>
#include <iostream>
#include <sstream>


using namespace std;

void f(int id) 
{
  stringstream ss;
  ss << "function f " << id << " starting\n"; cout << ss.str(); ss.str("");

  this_thread::sleep_for (chrono::milliseconds(2000));

  ss << "function f " << id << " finished\n"; cout << ss.str(); ss.str("");
}

void g(int id) 
{
  stringstream ss;
  ss << "function g " << id << " starting\n"; cout << ss.str(); ss.str("");

  this_thread::sleep_for (chrono::milliseconds(200));

  ss << "function g " << id << " finished\n"; cout << ss.str(); ss.str("");
}

future<void> fFuture = async(launch::async, []{ f(1); });
future<void> gFuture = async(launch::async, []{ g(1); });

// Why bother with a lambda if all the lambda does is call a solitary funtions?
future<void> f2Future = async(launch::async, f, 2 );
future<void> g2Future = async(launch::async, g, 2 );

int main()
{
}
