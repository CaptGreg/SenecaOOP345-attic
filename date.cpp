#include <iostream>
#include <chrono>
using namespace std;

// #include "date.h" // http://howardhinnant.github.io/date_v2.html
// Not quite sure what "date.h" brings to the table.

// (http://en.cppreference.com/w/cpp/chrono)

// What does std::chrono bring to the C++11 table?
// clocks, durations, and time_point
// C-style date and time library (e.g. std::time) 

// clocks: system_clock, stready_clock, high_resolution_clock

// Date and time utilities
//  
// C++
//  
// Utilities library
//  
// Date and time utilities
//  
// 
// C++ includes support for two types of time manipulation:
// 
//     The chrono library, a flexible collection of types that track time with varying degrees of precision (e.g. std::chrono::time_point).
//     C-style date and time library (e.g. std::time) 
// 
// chrono library
// 
// The chrono library defines three main types (durations, clocks, and time points) as well as utility functions and common typedefs.
// Duration
// 
// A duration consists of a span of time, defined as some number of ticks of some time unit. For example, "42 seconds" could be represented by a duration consisting of 42 ticks of a 1-second time unit.
// Defined in header <chrono>
// Defined in namespace std::chrono
// duration (C++11)
// 	a time interval
// (class template)
// Clocks
// 
// A clock consists of a starting point (or epoch) and a tick rate. For example, a clock may have an epoch of January 1, 1970 and tick every second. C++ defines three clock types:
// Defined in header <chrono>
// Defined in namespace std::chrono
// system_clock (C++11)
// 	wall clock time from the system-wide realtime clock
// (class)
// steady_clock (C++11)
// 	monotonic clock that will never be adjusted
// (class)
// high_resolution_clock (C++11)
// 	the clock with the shortest tick period available
// (class)
// Time point
// 
// A time point is a duration of time that has passed since the epoch of specific clock.
// Defined in header <chrono>
// Defined in namespace std::chrono
// time_point (C++11)
// 	a point in time
// (class template)
// C-style date and time library
// 
// Also provided are the C-style date and time functions, such as std::time_t, std::difftime, and CLOCKS_PER_SEC.
// Example
// 
// This example displays information about the execution time of a function call:
// Run this code
// 
// #include <iostream>
// #include <chrono>
// #include <ctime>
//  
// long fibonacci(unsigned n)
// {
//     if (n < 2) return n;
//     return fibonacci(n-1) + fibonacci(n-2);
// }
//  
// int main()
// {
//     std::chrono::time_point<std::chrono::system_clock> start, end;
//     start = std::chrono::system_clock::now();
//     std::cout << "f(42) = " << fibonacci(42) << '\n';
//     end = std::chrono::system_clock::now();
//  
//     std::chrono::duration<double> elapsed_seconds = end-start;
//     std::time_t end_time = std::chrono::system_clock::to_time_t(end);
//  
//     std::cout << "finished computation at " << std::ctime(&end_time)
//               << "elapsed time: " << elapsed_seconds.count() << "s\n";
// }
// 
// Possible output:
// 
// f(42) = 267914296
// finished computation at Mon Jul 29 08:41:09 2013
// elapsed time: 0.670427s
// 
// 
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

// c-code functions and structures 
//   struct tm {
//       int tm_sec;    /* Seconds (0-60) */
//       int tm_min;    /* Minutes (0-59) */
//       int tm_hour;   /* Hours (0-23) */
//       int tm_mday;   /* Day of the month (1-31) */
//       int tm_mon;    /* Month (0-11) */
//       int tm_year;   /* Year - 1900 */
//       int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
//       int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
//       int tm_isdst;  /* Daylight saving time */
//   };

//   #include <time.h>
// 
//   char *asctime(const struct tm *tm);
//   char *asctime_r(const struct tm *tm, char *buf);
// 
//   char *ctime(const time_t *timep);
//   char *ctime_r(const time_t *timep, char *buf);
// 
//   struct tm *gmtime(const time_t *timep);
//   struct tm *gmtime_r(const time_t *timep, struct tm *result);
// 
//   struct tm *localtime(const time_t *timep);
//   struct tm *localtime_r(const time_t *timep, struct tm *result);
//
//   time_t mktime(struct tm *tm);

//   The ctime(), gmtime() and localtime() functions all take an argument of data type time_t, 
//   which represents calendar time.  When interpreted as an absolute time value, it represents the
//   number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).

//   The call ctime(t) is equivalent to asctime(localtime(t)).  It converts the calendar time t into a null-terminated string of the form
//   "Wed Jun 30 21:49:08 1993\n"

//   #include <time.h>
//   time_t time(time_t *t);         time - get time in seconds
//   time() returns the time as the number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
//   If t is non-NULL, the return value is also stored in the memory pointed to by t.

int main1()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));

    std::cout << "24 hours ago, the time was "
#if 0
              // GB IS std::put_time C++14?? 
              << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
#else
              << std::ctime(&now_c)
#endif
              << '\n';
 
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::cout << "Hello World\n";
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Printing took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "us."
              << "\n"
              << "\n";
  return 0;
}

// GB no support for recursive lambdas
long fibonacci(unsigned n)  // GB 42! is about 267 million
{ // recursive fibonacci calculation
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}
 
int main2()
{
    std::cout<< "On this " << 8*sizeof(void*) << "-bit machine, sizeof(long) = " << sizeof(long) << " bytes\n";
    // std::chrono::time_point<std::chrono::system_clock> start, end;

    // start = std::chrono::system_clock::now();
    auto start = std::chrono::system_clock::now();
    std::cout << "recursive fibonacci(42) = " << fibonacci(42) << '\n';
    // end = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << "\n"
              << "\n";


    // long fib = [] (long n)                <<<<----- syntax error
    auto fib = [] (long n) -> long 
    { // non-recursive fibonacci calculation
      if(n <= 1) return n;
      long f = 1;
      long fPrev = 1;
      for(int i = 2; i < n; ++i) {
        long temp = f;
        f += fPrev;
        fPrev = temp;
      }
      return f;
    };

    start = std::chrono::system_clock::now();
    std::cout << "non-recursive fibonacci(42) = " << fib(42) << '\n';
    end = std::chrono::system_clock::now();

    elapsed_seconds = end-start;
    end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << "\n"
              << "\n";
  return 0;
}

#include <ctime>
int main3()
{
  struct tm* gmt;
  struct tm* loc;
  time_t t = time(0);

  cout << "NOTE: --- THERE IS A BIG GOTCHA WITH localtime AND gmtime\n";
  cout << "gmtime and localtime return the same local internal storage address.\n";
  cout << "   gmtime(&t)   : " << (void*) gmtime(&t) << "\n";
  cout << "   localtime(&t): " << (void*) localtime(&t) << "\n";

  gmt = gmtime(&t);
  loc = localtime(&t);
  cout << "localtime called last:\n";
  cout << "   loc->tm_hour=" << loc->tm_hour << "\n";
  cout << "   gmt->tm_hour=" << gmt->tm_hour << "\n";

  loc = localtime(&t);
  gmt = gmtime(&t);
  cout << "gmtime called last:\n";
  cout << "   loc->tm_hour=" << loc->tm_hour << "\n";
  cout << "   gmt->tm_hour=" << gmt->tm_hour << "\n";

  cout << "call gmtime and use result:\n";
  gmt = gmtime(&t);
  cout << "   gmt->tm_hour=" << gmt->tm_hour << "\n";
  cout << "call localtime and use result:\n";
  loc = localtime(&t);
  cout << "   loc->tm_hour=" << loc->tm_hour << "\n";
  return 0;
}

int main()
{
  main1();
  main2();
  main3();
  return 0;
}
