// 1. how to thread a for loop
// 2. how do you start up a thread with a class member function?
// 3. sometimes threading makes things worse

// threading includes
#include <thread>
#include <mutex>

// application includes
#include <vector>
#include <chrono>
#include <iostream>
#include <exception>

// using namespace std;

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t tWait = duration_get.count();
    return tWait;
  }
  uint64_t nsecs() {
    typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    uint64_t tWait = duration_get.count();
    return tWait;
  }
};

class Application {
  std::vector<uint64_t> v;
  std::mutex vLock;
public:
  void Reset() { v.clear(); }
  uint64_t Size() { return v.size(); }
  uint64_t Capacity() { return v.capacity(); }
  void calcItem(size_t item, bool needLock)
  {
    uint64_t itemValue;
    itemValue = 123; // ... (a lengthy calculation)  // 123 to make it compile

    if(needLock)
      { // acquire smartLock
        std::unique_lock<std::mutex> smartLock (vLock);
        v.push_back(itemValue);
      } // release smartLock
    else
      {
        v.push_back(itemValue);
      }
  }
  void doWork(size_t start, size_t end, bool needLock)
  {
    for(size_t item = start; item < end; item++)
      calcItem(item,needLock);
  }
  void runProcess() // executes on one thread
  {
    size_t start = 0;
    size_t end   = 50LL * 1000LL * 1000LL;
    bool needLock = false;
    v.reserve(end);

    doWork(start, end, needLock);
  }
  void runProcessWithThreads()
  {
    const int NUM_THREADS = std::thread::hardware_concurrency();
    std::cout << "running with " << NUM_THREADS << " threads\n";

    std::vector<std::thread> t(NUM_THREADS);

    size_t start = 0;
    size_t end   = 50LL * 1000LL * 1000LL;
    v.reserve(end);

    size_t chunk = (end - start + (NUM_THREADS-1)) / NUM_THREADS;

    bool needLock = true;

    for(int i = 0; i < NUM_THREADS; i++) {
      size_t s = start + i * chunk;
      size_t e = s + chunk;
      if(i == NUM_THREADS - 1) e = end;

    #if 1 // call thread directly

      // Note how to start a thread with a class member function.
      t[i] = std::thread (&Application::doWork, this, s, e, needLock);

    #else // call thread with a functional pointer

      std::function<void(void)> b = std::bind( &Application::doWork, this, s, e, needLock);
      // auto b = std::bind( &Application::doWork, this, s, e, needLock); // also works...

      t[i] = std::thread ( b );

    #endif
    }
    for( auto& e : t ) e.join();
  }
}; 

int main()
{
  #ifdef __GNUC__
    #ifdef __clang__
      std::cout << "clang++ compiler version " << __VERSION__ << "\n";
    #else
      std::cout << "g++ compiler version " << __VERSION__ << "\n";
    #endif
  #endif

  try {
    Application app;
    Timer t;

    t.Start();
    app.runProcess();
    t.Stop();
    uint64_t tNoThreads = t.usecs();
    std::cout << "no threading " << tNoThreads << " usecs"
              << ", " << tNoThreads * 1e-6 << " secs\n";
    std::cout << "vector v size/capacity=" << app.Size() << "/" << app.Capacity() << "\n";
    

    app.Reset();  // reset vector v

    t.Start();
    app.runProcessWithThreads();
    t.Stop();
    uint64_t tThreads = t.usecs();
    std::cout << "threading " << tThreads << " usecs"
              << ", " << tThreads * 1e-6 << " secs\n";
    std::cout << "vector v size/capacity=" << app.Size() << "/" << app.Capacity() << "\n";

    double speedup = double(tNoThreads) / double(tThreads);
    if(speedup >= 1.)
      std::cout << "Threading speed up = " << speedup << " times\n";
    else
      std::cout << "Threading slow down = " << 1./speedup << " times\n";

  } catch (std::exception& e) {
    std::cerr << e.what() << "\n";
  }
  return 0;
}

// timings on 2011 vintage laptop with a 4 core Intel i5-460M mobile CPU, 4GB DDRIII mem

// uname -a
//   Linux msi 3.13.0-57-generic #95-Ubuntu SMP Fri Jun 19 09:28:15 UTC 2015 x86_64 x86_64 x86_64 GNU/Linux

// lsb_release -a
//   No LSB modules are available.
//   Distributor ID:	Ubuntu
//   Description:	Ubuntu 14.04.2 LTS
//   Release:	14.04
//   Codename:	trusty

// clang++ --version
//   Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)
//   Target: x86_64-pc-linux-gnu
//   Thread model: posix

//   clang++ -std=c++11 threading101i.cpp -o threading101i -pthread && ./threading101i
//     no threading 1821157 usecs
//     running with 4 threads
//     threading 26723729 usecs
//     Threading slow down = 14.674 times

//   clang++ -std=c++11 -Ofast threading101i.cpp -o threading101i -pthread && ./threading101i
//     no threading 640690 usecs
//     running with 4 threads
//     threading 13486095 usecs
//     Threading slow down = 21.0493 times

// g++ --version
//   g++ (Ubuntu 4.9.2-0ubuntu1~14.04) 4.9.2

//   g++ -std=c++11 threading101i.cpp -o threading101i -pthread && ./threading101i
//     no threading 3393726 usecs
//     running with 4 threads
//     threading 26701773 usecs
//     Threading slow down = 7.86798 times

//   g++ -std=c++11 -Ofast threading101i.cpp -o threading101i -pthread && ./threading101i
//     no threading 463719 usecs
//     running with 4 threads
//     threading 11403303 usecs
//     Threading slow down = 24.591 times

// g++-5 --version
//   g++-5 (Ubuntu 5.1.0-0ubuntu11~14.04.1) 5.1.0

//   g++-5 -std=c++11 threading101i.cpp -o threading101i -pthread && ./threading101i
//     no threading 1814138 usecs
//     running with 4 threads
//     threading 25927255 usecs
//     Threading slow down = 14.2918 times

//   g++-5 -std=c++11 -Ofast threading101i.cpp -o threading101i -pthread && ./threading101i
//     no threading 517545 usecs
//     running with 4 threads
//     threading 14106871 usecs
//     Threading slow down = 27.2573 times

