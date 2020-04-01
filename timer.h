#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>

class Timer {
  decltype(std::chrono::high_resolution_clock::now()) now, start, stop;
public:
  void PrintNow()   { 
      std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
      time_t tt = std::chrono::system_clock::to_time_t(now);
      tm local_tm = *::localtime(&tt);
      std::cout << std::setw(2) << std::setfill('0') << local_tm.tm_hour << ':'
                << std::setw(2) << std::setfill('0') << local_tm.tm_min  << ':'
                << std::setw(2) << std::setfill('0') << local_tm.tm_sec  << ' ';
  }
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t millisecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::milli> >(stop-start).count(); }
  uint64_t microsecs() { return std::chrono::duration_cast<std::chrono::duration<int,std::micro> >(stop-start).count(); }
  uint64_t nanosecs () { return std::chrono::duration_cast<std::chrono::duration<int,std::nano>  >(stop-start).count(); }
};

