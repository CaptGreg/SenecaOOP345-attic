// http://www.codeproject.com/Articles/540912/Cplusplus-Threads-Make-your-multitasking-life-e

#include <thread>
#include <future>
#include <iostream>

std::promise<int> sex;
void foo()
  {
  // do stuff
  sex.set_value(1); // After this call, future::get() will return this value. 
  // sex.set_exception(std::make_exception_ptr(std::runtime_error("broken_condom"))); // After this call, future::get() will throw this exception
  }

int main()
  {
  std::future<int> makesex = sex.get_future();
  std::thread t(foo);
  
  // do stuff
  try
    {
    std::cout << "makesex returned " << makesex.get() << "\n";
    std::cout << "hurray\n";
    }
  catch(...)
    {
    // She dumped us :(
    }
  t.join();
  return 0;
  }  
