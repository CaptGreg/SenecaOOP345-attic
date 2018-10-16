#include <chrono>

class Timer {

  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;

public:

  void Start() { start = std::chrono::high_resolution_clock::now(); }

  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }

  uint64_t millisecs()
  {
   typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    return duration_get.count();
  }

  uint64_t microsecs()
  {
   typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    return duration_get.count();
  }

  uint64_t nanosecs()
  {
   typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    return duration_get.count();
  }
};


