#include <fstream>
#include <chrono>
#include <string>

class Timer {

  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
  bool bActive;

public:
  Timer() : bActive(false) {}

  void Start() { bActive = true;  start = std::chrono::high_resolution_clock::now(); }

  void Stop()  { bActive = false; stop  = std::chrono::high_resolution_clock::now(); }

  uint64_t Print_ms(std::ostream& os, std::string msg) {
    if(bActive) Stop();
    typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    uint64_t tDuration = duration_get.count();
    os << msg << tDuration << " msecs\n";
    return tDuration;
  }
  
  uint64_t Print_ms(std::string msg) {
    return Print_ms(std::cout, msg);
  }

  uint64_t Print_us(std::ostream& os, std::string msg) {
    if(bActive) Stop();
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t tDuration = duration_get.count();
    os << msg << tDuration << " usecs\n";
    return tDuration;
  }
  
  uint64_t Print_us(std::string msg) {
    return Print_us(std::cout, msg);
  }

  uint64_t Print_ns(std::ostream& os, std::string msg) {
    if(bActive) Stop();
    typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    uint64_t tDuration = duration_get.count();
    os << msg << tDuration << " nsecs\n";
    return tDuration;
  }
  
  uint64_t Print_ns(std::string msg) {
    return Print_ns(std::cout, msg);
  }
};
