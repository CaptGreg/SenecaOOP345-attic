#include <chrono>

class Timer {
  decltype(std::chrono::high_resolution_clock::now()) start, stop;
public:
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t millisecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::milli> >(stop-start).count(); }
  uint64_t microsecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::micro> >(stop-start).count(); }
  uint64_t nanosecs () { return std::chrono::duration_cast<std::chrono::duration<int,std::nano>  >(stop-start).count(); }
};
