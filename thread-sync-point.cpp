// spinlock vs future promise sync point

#include <chrono>       // std::this_thread::sleep_for(std::chrono::seconds(2));
#include <exception> 
#include <future>       // std::promise, std::future_error, std::future_category, includes <thread>
#include <iostream>     // std::cout, std::cerr

class Timer {
  decltype(std::chrono::high_resolution_clock::now()) start, stop;
public:
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t millisecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::milli> >(stop-start).count(); }
  uint64_t microsecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::micro> >(stop-start).count(); }
  uint64_t nanosecs () { return std::chrono::duration_cast<std::chrono::duration<int,std::nano>  >(stop-start).count(); }
};

Timer timer;

bool spinLockBool = false;
int  spinLockInt;

void spin_lock(int *p) // gcc spinlock compare_and_swap functions for spinlock
{
    while(!__sync_bool_compare_and_swap(p, 0, 1));
}

void spin_unlock(int volatile *p)
{
    asm volatile ("":::"memory"); // acts as a memory barrier.
    *p = 0;
}

int main ()
{
  std::promise<int> prom;

  try {
    std::cout << "thread sync mehods\n";
    std::cout << "  1. promise-future\n";
    std::cout << "  2. spinlock-spin\n";
    std::cout << "  3. spinlock-yield\n";
    std::cout << "  4. spinlock-pause\n";
    std::cout << "  5. spinlock gcc compare and swap\n";

    auto fut = prom.get_future();
    // prom.set_value(42);

    auto function = [] (std::promise<int> * prom) { 
      std::this_thread::sleep_for(std::chrono::seconds(2));

      timer.Start();
      prom->set_value(88); 

      std::this_thread::sleep_for(std::chrono::seconds(2));

      // prom->set_value(888);  // std::future_error: Promise already satisfied 
      timer.Start();
      spinLockBool = true;

      std::this_thread::sleep_for(std::chrono::seconds(2));
      // prom->set_value(888);  // std::future_error: Promise already satisfied 

      timer.Start();
      spinLockBool = true;

      std::this_thread::sleep_for(std::chrono::seconds(2));

      timer.Start();
      spinLockBool = true;

      std::this_thread::sleep_for(std::chrono::seconds(2));

      timer.Start();
      spinLockInt = 1;

      std::this_thread::sleep_for(std::chrono::seconds(2));

      return 8888; 
    }; // function to be run by thread

    // set promise a second time throws: what():  std::future_error: Promise already satisfied

    // auto tid = std::thread(function,&prom);
    // std::cout << fut.get() << "\n";
    // tid.join();

    auto asyncFut = async(std::launch::async, function, &prom);
    auto fg = fut.get();
    timer.Stop();
    std::cout << "main and thread sync achieved: promise future get() = " << fg << "\n";
    std::cout << "1. " << timer.nanosecs() << " ns for promise-future sync\n";  // future-get sync 30-50 usec for 

    while(!spinLockBool);
    timer.Stop();
    std::cout << "2. " << timer.nanosecs() << " ns for spinlock-spin sync\n";  // spinlock sync sub 1 us
    spinLockBool =(&spinLockInt);

    // set promise a second time throws: what():  std::future_error: Promise already satisfied

    // auto tid = std::thread(function,&prom);
    // std::cout << fut.get() << "\n";
    // tid.join();

    while(!spinLockBool) std::this_thread::yield();
    timer.Stop();
    std::cout << "3. " << timer.nanosecs() << " ns for spinlock-yield sync\n";  // spinlock-yield sync 1 us

    asm( "pause" );
    while(!spinLockBool) asm( "pause" );  // see Intel docs w.r.t. spinloops usage
    timer.Stop();
    std::cout << "4. " << timer.nanosecs() << " ns for spinlock-pause sync\n";  // spinlock-yield sync 1 us

    spin_lock(&spinLockInt);
    timer.Stop();
    std::cout << "5. " << timer.nanosecs() << " ns for spinlock gcc compare and sync\n";  // spinlock-asemble sync 1 us

    auto afg =  asyncFut.get();
    std::cout << "async future get() = " << afg << "\n";
    std::cout << "so far, so good ... \n";

    // THESE ALL FAIL
    //    std::cout << "calling fut.get() a second time\n";
    //    std::cout << fut.get() << "\n"; // std::future_error: No associated state


    //    std::cout << "set promise value a second time\n";
    //    prom.set_value(42);  // std::future_error: Promise already satisfied
    //    std::cout << "second set_value worked " <<  fut.get() << "\n";

    //    prom.get_future();   // std::future_error: Future already retrieved 

    return EXIT_SUCCESS;
  } catch (const std::future_error& e) {
    // if (e.code().category() == std::future_category())
    std::cerr << "future_error exception " << e.what() << "\n";
  } catch (const std::exception& e) {
    std::cerr << "std::exception " << e.what() << "\n";
  } catch (...) {
    std::cerr << "... thrown\n";
  }
  return EXIT_FAILURE;
}
