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

template <typename T>
void DuffsDevice(T *from, T *to, int count)
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

template <typename T>
void DuffsDeviceMACRO(T *from, T *to, int count)
{
// GB if you thought the above code was hard to understand, does this boost code simplify it for you?
// GB see /usr/include/boost/numeric/ublas/detail/duff.hpp
//
//  Copyright (c) 2000-2002
//  Joerg Walter, Mathias Koch
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  The authors gratefully acknowledge the support of
//  GeNeSys mbH & Co. KG in producing this work.
//

#ifndef _BOOST_UBLAS_DUFF_
#define _BOOST_UBLAS_DUFF_

#define DD_SWITCH(n, d, r, expr) \
    { \
        unsigned r = ((n) + (d) - 1) / (d); \
        switch ((n) % (d))  { \
        case 0: do { expr;
#define DD_CASE_I(i, expr) \
        case (i): expr;
#define DD_WHILE(r) \
            } while (-- (r) > 0); \
        } \
    }

#define DD_1T(n, d, r, expr) \
    DD_WHILE(r)
#define DD_2T(n, d, r, expr) \
    DD_CASE_I(1, expr) \
    DD_1T(n, d, r, expr)
#define DD_3T(n, d, r, expr) \
    DD_CASE_I(2, expr) \
    DD_2T(n, d, r, expr)
#define DD_4T(n, d, r, expr) \
    DD_CASE_I(3, expr) \
    DD_3T(n, d, r, expr)
#define DD_5T(n, d, r, expr) \
    DD_CASE_I(4, expr) \
    DD_4T(n, d, r, expr)
#define DD_6T(n, d, r, expr) \
    DD_CASE_I(5, expr) \
    DD_5T(n, d, r, expr)
#define DD_7T(n, d, r, expr) \
    DD_CASE_I(6, expr) \
    DD_6T(n, d, r, expr)
#define DD_8T(n, d, r, expr) \
    DD_CASE_I(7, expr) \
    DD_7T(n, d, r, expr)

#define DD(n, d, r, expr) \
    DD_SWITCH(n, d, r, expr) \
    DD_##d##T(n, d, r, expr)

#endif

DD_SWITCH(count, 8, r, *to++ = *from++)
DD_CASE_I(7, *to++ = *from++)
DD_CASE_I(6, *to++ = *from++)
DD_CASE_I(5, *to++ = *from++)
DD_CASE_I(4, *to++ = *from++)
DD_CASE_I(3, *to++ = *from++)
DD_CASE_I(2, *to++ = *from++)
DD_CASE_I(1, *to++ = *from++)
// DD_1T(count, 8, r, *to++ = *from++)
// DD_2T(count, 8, r, *to++ = *from++)
// DD_3T(count, 8, r, *to++ = *from++)
// DD_4T(count, 8, r, *to++ = *from++)
// DD_5T(count, 8, r, *to++ = *from++)
// DD_6T(count, 8, r, *to++ = *from++)
// DD_7T(count, 8, r, *to++ = *from++)
// DD_8T(count, 8, r, *to++ = *from++)
DD_WHILE(r)

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

  std::cout << "SIZE=" << SIZE << ", "
            << "sizeof(from/to) = " << sizeof(from) << "/" << double(sizeof(from)) << " Bytes, " 
            << sizeof(from) / sizeof(from[0]) << " " << sizeof(from[0]) << " BYTE elements\n";
  auto PrintTime = [&t] (std::string msg) -> uint64_t {
      t.Stop();
      uint64_t elapsedMicroseconds = t.microsecs();
      std::cout << msg << " " << 1e-6*elapsedMicroseconds << " secs\n";
      return elapsedMicroseconds;
  };

  t.Start();
  DuffsDevice(from, to, SIZE);
  PrintTime (std::string("Duff Device      "));

  t.Start();
  DuffsDeviceMACRO(from, to, SIZE);
  PrintTime (std::string("Duff Device MACRO"));

  // void *memmove(void *dest, const void *src, size_t n);
  // DESCRIPTION
       // The memmove() function copies n bytes from memory area src to memory area dest.  The memory areas may over‐
       // lap: copying takes place as though the bytes in src are first copied into a temporary array that  does  not
       // overlap src or dest, and the bytes are then copied from the temporary array to dest.

  t.Start();
  memmove(to, from, sizeof(*from) * SIZE);
  PrintTime (std::string("memmove          "));

  // void *memcpy(void *dest, const void *src, size_t n);
  // DESCRIPTION
       // The  memcpy()  function copies n bytes from memory area src to memory area dest.  The memory areas must not
       // overlap.  Use memmove(3) if the memory areas do overlap.

  t.Start();
  memcpy(to, from, sizeof(*from) * SIZE);
  PrintTime (std::string("memcpy           "));

  t.Start();
  for(int i = 0; i < SIZE; i++) to[i] =  from[i];
  PrintTime (std::string("for loop         "));

  t.Start();
#pragma omp parallel for 
  for(int i = 0; i < SIZE; i++) to[i] =  from[i];
  PrintTime (std::string("omp for loop     "));

  t.Start();
  std::copy(from, from+SIZE, to);
  PrintTime (std::string("std::copy        "));
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

2017-02-09 g++ (Ubuntu 5.4.1-2ubuntu1~16.04) 5.4.1 20160904 on Acer Aspire laptop with Intel i5 cpu
g++ -Wall -std=c++11  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  DuffsDevice.cpp -o DuffsDevice -lrt -pthread

Duff Device       1.34647  secs
Duff Device MACRO 1.20477  secs
memmove           0.135627 secs
memcpy            0.132426 secs
for loop          1.4535   secs
omp for loop      0.480604 secs
std::copy         0.133548 secs


2017-02-09 g++ (Ubuntu 5.4.1-2ubuntu1~16.04) 5.4.1 20160904 on Acer Aspire laptop with Intel i5 cpu
g++ -Wall -std=c++11  -Ofast -fopenmp -fmax-errors=1 -Wfatal-errors  DuffsDevice.cpp -o DuffsDevice -lrt -pthread && ./DuffsDevice

Duff Device       0.461805 secs
Duff Device MACRO 0.318118 secs
memmove           0.13866  secs
memcpy            0.133369 secs
for loop          0.131578 secs
omp for loop      0.132177 secs
std::copy         0.134879 secs


WHY IS THE MACRO CODE FASTER?

*/

// GB - a later BOOST example of using Duff's Device

// # /* Copyright (C) 2002
// #  * Housemarque Oy
// #  * http://www.housemarque.com
// #  *
// #  * Distributed under the Boost Software License, Version 1.0. (See
// #  * accompanying file LICENSE_1_0.txt or copy at
// #  * http://www.boost.org/LICENSE_1_0.txt)
// #  */
// #
// # /* Revised by Paul Mensonides (2002) */
// #
// # /* See http://www.boost.org for most recent version. */
// #
// # /* This example uses the preprocessor library to implement a generalized
// #  * macro for implementing Duff's Device.
// #  *
// #  * This example was inspired by an original generalized macro for
// #  * for implementing Duff's Device written by Joerg Walter.
// #  */
// #
// # include <assert.h>
// #
// # include <boost/preprocessor/repetition/repeat.hpp>
// # include <boost/preprocessor/tuple/elem.hpp>
// #
// # /* Expands to a Duff's Device. */
// # define DUFFS_DEVICE(UNROLLING_FACTOR, COUNTER_TYPE, N, STATEMENT) \
//    do { \
//       COUNTER_TYPE duffs_device_initial_cnt = (N); \
//       if (duffs_device_initial_cnt > 0) { \
//          COUNTER_TYPE duffs_device_running_cnt = (duffs_device_initial_cnt + (UNROLLING_FACTOR - 1)) / UNROLLING_FACTOR; \
//          switch (duffs_device_initial_cnt % UNROLLING_FACTOR) { \
//             do { \
//                BOOST_PP_REPEAT(UNROLLING_FACTOR, DUFFS_DEVICE_C, (UNROLLING_FACTOR, { STATEMENT })) \
//             } while (--duffs_device_running_cnt); \
//          } \
//       } \
//    } while (0) \
//    /**/
// #
// # define DUFFS_DEVICE_C(Z, I, UNROLLING_FACTOR_STATEMENT) \
//    case (I ? BOOST_PP_TUPLE_ELEM(2, 0, UNROLLING_FACTOR_STATEMENT) - I : 0): \
//       BOOST_PP_TUPLE_ELEM(2, 1, UNROLLING_FACTOR_STATEMENT); \
//    /**/
// #
// # ifndef UNROLLING_FACTOR
// # define UNROLLING_FACTOR 16
// # endif
// #
// # ifndef N
// # define N 1000
// # endif
// 
// int main(void) {
//    int i = 0;
//    DUFFS_DEVICE(UNROLLING_FACTOR, int, 0, ++i;);
//    assert(i == 0);
//    DUFFS_DEVICE(UNROLLING_FACTOR, int, N, ++i;);
//    assert(i == N);
//    return 0;
// }
