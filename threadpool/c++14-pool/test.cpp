// http://roar11.com/2016/01/a-platform-independent-thread-pool-using-c14/
// GB hacked a bit to add a main, a timer class, print thread number, times

#include <vector>
#include <chrono>
#include <iostream>
#include <string>

#include "threadpool.hpp" // includes "threadsafequeue.hpp"

class Timer { // GB
public:
  auto tick() { return std::chrono::high_resolution_clock::now(); }

  uint64_t usecs( auto ticks) {
    typedef std::chrono::duration<int,std::micro> usecs_t;
    usecs_t duration_get( std::chrono::duration_cast<usecs_t>(ticks) ) ;
    uint64_t us = duration_get.count();
    return us;
  }
};


int main() // GB
{
  Timer timer;
  auto startTime = timer.tick();
  std::vector<MyNamespace::ThreadPool::TaskFuture<void>> v;

  unsigned num = 21u;
  for(uint32_t i = 0u; i < num; ++i)
    v.push_back(
      MyNamespace::DefaultThreadPool::submitJob(
        [i]() { 
          std::cout << std::to_string(i) + "\n";
          std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }
      )
    );
  for(auto& item: v)
      item.get();

  auto dt = timer.tick() - startTime;
  std::cout << "ran " << num << " threads in " << 1.e-6 * timer.usecs(dt) << " sec\n";

  startTime = timer.tick();
  v.clear();

  auto lengthyProcess = [] { std::this_thread::sleep_for(std::chrono::seconds(1)); };
  v.push_back(
    MyNamespace::DefaultThreadPool::submitJob(
      [lengthyProcess] () { 
        std::cout << std::string("lengthyProcess()\n");
        lengthyProcess();
      }
    )
  );

  auto lengthyProcessWithArguments = [](int i, double d) { std::this_thread::sleep_for(std::chrono::seconds(2)); };
  v.push_back(
    MyNamespace::DefaultThreadPool::submitJob(
      [lengthyProcessWithArguments] (int a, float b) { 
        std::cout << std::string("lengthyProcessWithArguments(") + std::to_string(a) + ", " + std::to_string(b) + ")\n";
        lengthyProcessWithArguments(a, b); 
      }, 5, 10.0f
    )
  );

  for(auto& item: v)
      item.get();

  dt = timer.tick() - startTime;
  std::cout << "ran 2 'lengthyProcess' threads with and without args in " << 1.e-6 * timer.usecs(dt) <<  " sec\n";

  startTime = timer.tick();
  auto f = MyNamespace::DefaultThreadPool::submitJob(
    [lengthyProcess] { 
      std::cout << std::string("lengthyProcess()\n");
      lengthyProcess();
    }
  );

  auto f2 = MyNamespace::DefaultThreadPool::submitJob(
    [lengthyProcessWithArguments] (int a, float b) { 
      std::cout << std::string("lengthyProcessWithArguments(") + std::to_string(a) + ", " + std::to_string(b) + ")\n";
      lengthyProcessWithArguments(a, b); 
    }, 5, 10.0f
  );
  f.get();
  f2.get();
  dt = timer.tick() - startTime;
  std::cout << "ran by hand 2 'lengthyProcess' threads with and without args in " << 1.e-6 * timer.usecs(dt) <<  " sec\n";


  // If submitting a reference for an argument, it is important to remember to wrap it with std::ref or std::cref.

  class MyObject {
  public:
    void hello() const { std::cout << "hello\n"; }
  };

  MyObject object;

  auto lengthyProcessThatNeedsToReadObject = [] (const MyObject& obj) {obj.hello();};

  auto f3 = MyNamespace::DefaultThreadPool::submitJob(
    [lengthyProcessThatNeedsToReadObject] (const MyObject& object) { 
      lengthyProcessThatNeedsToReadObject(object); 
    }, std::cref(object)
  );
  f3.get();

}
