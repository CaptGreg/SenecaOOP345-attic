// use std::copy to read a file using istreambuf_iterator
// unfortunately it is about 120 times slower than our best algorithm

// see http://www.cplusplus.com/reference/algorithm/copy/

// copy algorithm example
#include <algorithm>    // std::copy
#include <chrono>       // std::chrono
// #include <filesystem>   // C++17 filesystem functions  (not found)
#include <experimental/filesystem>  // Found!
// /usr/include/c++/7/experimental/filesystem
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
      std::string data { std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };
      in.close();
      std::cout << "1. string via istreambuf_iterator: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "1. string via istreambuf_iterator: " << 1.e-6*t.microsecs() << " sec (slowest)\n";
  }

  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary);

    t.Start();
    if(in.is_open()) {
      std::string data(static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str());
      in.close();
      std::cout << "2. string via stringstream rdbuf: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "2. string via stringstream rdbuf: " << 1.e-6*t.microsecs() << " sec (fast)\n";
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
      std::cout << "3. string via vector read string: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "3. string via vector read string: " << 1.e-6*t.microsecs() << " sec (fast)\n";
  }

  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary);

    t.Start();
    if(in.is_open()) {
      std::string data(std::istreambuf_iterator<char>{in}, {});
      in.close();
      std::cout << "4. 1 line string istreambuf_iterator: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "4. 1 line string istreambuf_iterator: " << 1.e-6*t.microsecs() << " sec (slow)\n";

  }
  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary);

    t.Start();
    if(in.is_open()) {
      std::string data(static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str());
      in.close();
      std::cout << "5. 1 line stringstream rdbuf: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "5. 1 line stringstream rdbuf: " << 1.e-6*t.microsecs() << " sec (slow)\n";

  }

  {
    std::fstream in(dataFile, std::ios::in | std::ios::binary | std::ios::ate);

    t.Start();
    if(in.is_open()) {
      // in.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
      std::ifstream::pos_type fileSize = in.tellg();
      // std::string data((size_t) fileSize, '\0'); //0.119338 sec, 0.118474 sec
      std::string data;
      // data.reserve((size_t) fileSize);
      data.resize((size_t) fileSize, '\0'); //0.117693 sec, 0.118642 sec 
      in.seekg(0, std::ios::beg);
      in.read(data.data(), data.size());
      in.close();
      std::cout << "6. resize read: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "6. resize read: " << 1.e-6*t.microsecs() << " sec (fastest)\n";

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

      std::cout << "7. std::copy via istreambuf_iterator: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    }
    t.Stop(); std::cout << "7. std::copy via istreambuf_iterator: " << 1.e-6*t.microsecs() << " sec (slow)\n";
  }
#if 0
  // g++-7 -std=C++17 doesn't compile
  {
    namespace fs = std::experimental::filesystem;        // ld undefined symbol
    // namespace fs = std::experimental::filesystem::v1; // ld undefined symbol
    
    // Open the stream to 'lock' the file.
    std::ifstream f{ dataFile };

    t.Start();

    // Obtain the size of the file.
    const auto sz = fs::file_size(dataFile);

    // Create a buffer.
    std::string data(sz, ' ');

    // Read the whole file into the buffer.
    f.read(data.data(), sz);

    std::cout << "8. C++17 filesystem: size of '" << dataFile << "' is " << data.size() << " BYTES\n";
    t.Stop(); std::cout << "8. C++17 filesystem: " << 1.e-6*t.microsecs() << " sec (slow)\n";
  }
  // Note: you may need to use <experimental/filesystem> and std::experimental::filesystem if your standard library 
  // doesn't yet fully support C++17. You might also need to replace result.data() with &result[0] if it doesn't 
  // support non-const std::basic_string data.
#endif

  return 0;
}
