// http://www.codeproject.com/Articles/540912/Cplusplus-Threads-Make-your-multitasking-life-e

#include <thread>
#include <future>
#include <iostream>

std::promise<int> stuff;
void foo()
  {
  // do stuff
  stuff.set_value(1); // After this call, future::get() will return this value. 
  // stuff.set_exception(std::make_exception_ptr(std::runtime_error("broken_condom"))); // After this call, future::get() will throw this exception
  }

int main()
  {
  std::future<int> makestuff = stuff.get_future();
  std::thread t(foo);
  
  // do stuff
  try
    {
    std::cout << "makestuff returned " << makestuff.get() << "\n";
    std::cout << "hurray\n";
    }
  catch(...)
    {
    // She dumped us :(
    }
  t.join();
  return 0;
  }  
