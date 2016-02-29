#include <chrono>
#include <iostream>
#include <string>
#include <cstring> // memcpy

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#ifdef __clang__
  // clang version 3.6.2 sets GNUC fields to version to 4.2.1
  #define PRIVATE_OMP_H 40201
#else
  // g++ (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010
  #define PRIVATE_OMP_H 50201  
#endif

#if GCC_VERSION == PRIVATE_OMP_H
  #include "/usr/lib/gcc/x86_64-linux-gnu/5/include/omp.h"
#else
  #include <omp.h>
#endif

using namespace std;

void DuffsDevice(float *from, float *to, int count)
{
  int n=(count+7)/8;
  switch(count%8){
  case 0: do {    *to++ = *from++;
  case 7:         *to++ = *from++;
  case 6:         *to++ = *from++;
  case 5:         *to++ = *from++;
  case 4:         *to++ = *from++;
  case 3:         *to++ = *from++;
  case 2:         *to++ = *from++;
  case 1:         *to++ = *from++;
	        } while(--n>0);
  }
}

class Timer { // use C++11 std::chrono features to create a stop-watch timer class
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop () { stop  = std::chrono::high_resolution_clock::now(); }
  // basic form to calculate time differences, illustrate with microseconds
  uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t us = duration_get.count();
    return us;
  }
  // Macro to define member functions that return milli, micro, and nano seconds 
  #define RET(UNITS) uint64_t UNITS##secs() { \
    typedef std::chrono::duration<int,std::UNITS> UNITS##secs_t ; \
    UNITS##secs_t duration_get( std::chrono::duration_cast<UNITS##secs_t>(stop-start) ) ; \
    uint64_t us = duration_get.count(); \
    return us; \
  }
  RET(milli) // creates member function 'uint64_t millisecs()' - which returns 'stop-start' in millisecs
  RET(micro) // creates member function 'uint64_t microsecs()' - which returns 'stop-start' in microsecs
  RET(nano)  // creates member function 'uint64_t nanosecs()'  - which returns 'stop-start' in nanosecs

};


  const int SIZE = .5*1000*1000*1000;
  float from[SIZE];
  float to[SIZE];

int main(int argc, char** argv)
{
  Timer    t;

  auto PrintTime = [&t] (std::string msg) -> uint64_t {
      t.Stop();
      uint64_t elapsedMicroseconds = t.microsecs();
      std::cout << msg << " " << 1e-6*elapsedMicroseconds << " secs\n";
      return elapsedMicroseconds;
  };

  t.Start();
  DuffsDevice(from, to, SIZE);
  PrintTime (std::string("Duff Device"));

  t.Start();
  memmove(to, from, sizeof(*from) * SIZE);
  PrintTime (std::string("memmove"));

  t.Start();
  memcpy(to, from, sizeof(*from) * SIZE);
  PrintTime (std::string("memcpy"));

  t.Start();
  for(int i = 0; i < SIZE; i++) to[i] =  from[i];
  PrintTime (std::string("for loop"));

  t.Start();
#pragma omp parallel for 
  for(int i = 0; i < SIZE; i++) to[i] =  from[i];
  PrintTime (std::string("omp for loop"));

  t.Start();
  std::copy(from, from+SIZE, to);
  PrintTime (std::string("std::copy"));
}

/*

g++  -Wall -std=c++11  -ggdb -fopenmp DuffsDevice.cpp -o DuffsDevice -lrt -pthread

Duff Device 1818454 usecs
memmove      519961 usecs
memcpy       521334 usecs
for loop    2089405 usecs
omp for loop 447924 usecs
std::copy    513170 usecs

g++ -Ofast -Wall -std=c++11  -ggdb -fopenmp DuffsDevice.cpp -o DuffsDevice -lrt -pthread

Duff Device 1316391 usecs
memmove      542370 usecs
memcpy       540780 usecs
for loop     542649 usecs
omp for loop 338114 usecs
std::copy    500315 usecs

clang++ -Wall -std=c++11  -ggdb -fopenmp DuffsDevice.cpp -o DuffsDevice -lrt -pthread

Duff Device  2461728 usecs
memmove       610164 usecs
memcpy        600569 usecs
for loop     2225044 usecs
omp for loop 2242320 usecs
std::copy     582771 usecs

clang++ -Ofast -Wall -std=c++11  -ggdb -fopenmp DuffsDevice.cpp -o DuffsDevice -lrt -pthread

Duff Device 1228481 usecs
memmove      519989 usecs
memcpy       522559 usecs
for loop     533571 usecs
omp for loop 527695 usecs
std::copy    529786 usecs

*/
