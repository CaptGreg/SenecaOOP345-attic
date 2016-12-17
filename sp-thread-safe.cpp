// http://en.cppreference.com/w/cpp/memory/shared_ptr

// GB above reference page says shared_ptr are thread safe.  They use atomics internally.
// GB passed a stress test with 10,000 threads messing with copies of the shared_ptr...

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
 
struct Base
{
    Base() { std::cout << "  Base::Base()\n"; }
    // Note: non-virtual destructor is OK here
    ~Base() { std::cout << "  Base::~Base()\n"; }
};
 
struct Derived: public Base
{
    Derived() { std::cout << "  Derived::Derived()\n"; }
    ~Derived() { std::cout << "  Derived::~Derived()\n"; }
};
 
void thr(std::shared_ptr<Base> p)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::shared_ptr<Base> lp = p; // thread-safe, even though the
                                  // shared use_count is incremented
    {
        static std::mutex io_mutex;   // GB one global copy shared by all invocations of this function.
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << "local pointer in a thread:"
                  << "  lp.get() = " << lp.get()
                  << ", lp.use_count() = " << lp.use_count() << '\n';
    }
}
 
int main()
{
    std::shared_ptr<Base> p = std::make_shared<Derived>();
 
    std::cout << "Created a shared Derived (as a pointer to Base)\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';

  #define STRESS_TEST // GB see if it breaks easily
  #ifndef STRESS_TEST
    const int NTHREADS = 3;
    std::thread t1(thr, p), t2(thr, p), t3(thr, p);
  #else
    const int NTHREADS = 10 * 1000;
    std::thread tid[NTHREADS];
    for(auto &t : tid)
      t = std::thread(thr, p);
  #endif

    p.reset(); // release ownership from main
    std::cout << "Shared ownership between " << NTHREADS << " threads and released\n"
              << "ownership from main:\n"
              << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';

  #ifndef STRESS_TEST
    t1.join(); t2.join(); t3.join();
  #else
    for(auto &t : tid)
      t.join();
  #endif
    std::cout << "All threads completed, the last one deleted Derived\n";
    std::cout << "  p.get() = " << p.get()
              << ", p.use_count() = " << p.use_count() << '\n';
}
