// use std::copy to read a file using istreambuf_iterator
// unfortunately it is about 120 times slower than our best algorithm

// see http://www.cplusplus.com/reference/algorithm/copy/

// copy algorithm example
#include <algorithm>    // std::copy
#include <chrono>       // std::chrono
#include <fstream>      // std::fstream
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <string>       // std::string
#include <vector>       // std::vector

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
  // Now use a macro to return milli, micro, and nano seconds 
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

// http://www.cplusplus.com/reference/algorithm/copy/
template<class InputIterator, class OutputIterator>
OutputIterator copyDIY (InputIterator first, InputIterator last, OutputIterator result)
{
  while (first!=last) {
    *result = *first;
    ++result; ++first;
  }
  return result;
}

int main (int argc, char* argv[]) 
{
  std::string dataFile { argv[argc == 1? 0 : 1] };
  Timer t;
  {
    int myints[]={10,20,30,40,50,60,70};
    std::vector<int> myvector (7);

    std::copy ( myints, myints+7, myvector.begin() );

    std::cout << "std::copy myvector contains:";
    for(auto e: myvector) 
      std::cout << ' ' << e; 
    std::cout << '\n';
  }

  // test copyDIY
  {
    int myints[]={10,20,30,40,50,60,70};
    std::vector<int> myvector (7);

    copyDIY ( myints, myints+7, myvector.begin() );

    std::cout << "copyDIY   myvector contains:";
    for(auto e: myvector) 
      std::cout << ' ' << e; 
    std::cout << '\n';
  }
  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary);

    t.Start();
    if(in.is_open()) {
      std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()); // needs extra ()
      in.close();
      std::cout << "string via istreambuf_iterator: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "string via istreambuf_iterator: " << t.microsecs() << " usec (slowest)\n";
  }

  // try using string iterator initialization to read a file
  // https://stackoverflow.com/questions/195323/what-is-the-most-elegant-way-to-read-a-text-file-with-c
  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary);

    t.Start();
    if(in.is_open()) {
      std::string data { std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };
      in.close();

      std::cout << "string via istreambuf_iterator: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "string via istreambuf_iterator: " << t.microsecs() << " usec (slow)\n";

  }

  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary);

    t.Start();
    if(in.is_open()) {
      std::string data(static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str());
      in.close();
      std::cout << "string via stringstream and rdbuf: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "string via stringstream and rdbuf: " << t.microsecs() << " usec (fast)\n";
  }

  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary | std::ios::ate);

    t.Start();
    if(in.is_open()) {
      std::ifstream::pos_type fileSize = in.tellg();
      in.seekg(0, std::ios::beg);
      std::vector<char> bytes(fileSize);
      in.read(bytes.data(), fileSize);
      std::string data(std::move(bytes.data()), fileSize); // GB: assume C++11 or later, use move
      in.close();
      std::cout << "string via vector read string: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "string via vector read string: " << t.microsecs() << " usec (fastest)\n";
  }

  // try using copy to read a file into a string
  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary | std::ios::ate);

    t.Start();
    if(in.is_open()) {
      std::ifstream::pos_type fileSize = in.tellg();
      std::string data((size_t) fileSize, '\0');
      in.seekg(0, std::ios::beg);
      std::copy ( 
        std::istreambuf_iterator<char>( in ),   // iterator interface to a istream
        std::istreambuf_iterator<char>(),       // end of stream iterator
        data.begin() 
      );
      in.close();

      std::cout << "std::copy via istreambuf_iterator: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "std::copy via istreambuf_iterator: " << t.microsecs() << " usec (slow)\n";
  }

  return 0;
}

/*
  small 248K file
  std::copy myvector contains: 10 20 30 40 50 60 70
  copyDIY   myvector contains: 10 20 30 40 50 60 70
  string via istreambuf_iterator: size of './stdcopy' is 249184 BYTES
  string via istreambuf_iterator: 14062 usec (slowest)
  string via istreambuf_iterator: size of './stdcopy' is 249184 BYTES
  string via istreambuf_iterator: 13819 usec (slow)
  string via stringstream and rdbuf: size of './stdcopy' is 249184 BYTES
  string via stringstream and rdbuf: 217 usec (fast)
  string via vector read string: size of './stdcopy' is 249184 BYTES
  string via vector read string: 48 usec (fastest)
  std::copy via istreambuf_iterator: size of './stdcopy' is 249184 BYTES
  std::copy via istreambuf_iterator: 12840 usec (slow)

  large 0.5GB (481.1MB) file
  std::copy myvector contains: 10 20 30 40 50 60 70
  copyDIY   myvector contains: 10 20 30 40 50 60 70
  string via istreambuf_iterator: size of 'CppCon 2015 - Kate Gregory “Stop Teaching C'-YnWhqhNdYyk.mkv' is 481120300 BYTES
  string via istreambuf_iterator: 25866286 usec (slowest)
  string via istreambuf_iterator: size of 'CppCon 2015 - Kate Gregory “Stop Teaching C'-YnWhqhNdYyk.mkv' is 481120300 BYTES
  string via istreambuf_iterator: 25876051 usec (slow)
  string via stringstream and rdbuf: size of 'CppCon 2015 - Kate Gregory “Stop Teaching C'-YnWhqhNdYyk.mkv' is 481120300 BYTES
  string via stringstream and rdbuf: 294888 usec (fast)
  string via vector read string: size of 'CppCon 2015 - Kate Gregory “Stop Teaching C'-YnWhqhNdYyk.mkv' is 481120300 BYTES
  string via vector read string: 198100 usec (fastest)
  std::copy via istreambuf_iterator: size of 'CppCon 2015 - Kate Gregory “Stop Teaching C'-YnWhqhNdYyk.mkv' is 481120300 BYTES
  std::copy via istreambuf_iterator: 24418362 usec (slow)

  real	1m16.655s
  user	1m15.960s
  sys	0m0.692s
*/
