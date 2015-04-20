// GB timings April 19, 2015 on AMD 1100T with Radeon HD 9670 GPU
// NOTE compiler warnings: clCreateSampler + clCreateCommandQueue deprecated
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <vector>       // want std::vector, not cl::vector
#include <iostream>
#include <exception>
#include <chrono>
#include <iterator>     // ostream_iterator
#include <algorithm> // compiles without

// using namespace cl;
// using namespace std;

std::string now() {
    time_t t = time(0);
    char buffer[9] = {0};

    strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&t));
    return std::string(buffer);
}

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
 void Start() { start  = std::chrono::high_resolution_clock::now(); }
 void Stop()  { stop   = std::chrono::high_resolution_clock::now(); }
 uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t us = duration_get.count();
    return us;
 }
};

int main(int argc, char* argv[]) 
{
  try {
    Timer stopWatch;

    stopWatch.Start();

    ::size_t elements = 100 * 1000 * 1000;
    if(argc  > 1) elements = atoi(argv[1]);

    std::vector<float> data5(elements, 5.5);
    std::vector<float> data6(elements, 3.25);

    cl::Context(CL_DEVICE_TYPE_DEFAULT);   // 4405826 microseconds for adding 100000000 floats
    // cl::Context(CL_DEVICE_TYPE_GPU);    // 4391718 microseconds for adding 100000000 floats
    // cl::Context(CL_DEVICE_TYPE_CPU);    // 4396792 microseconds for adding 100000000 floats



    bool readOnly;
    bool useHostPtr;
    cl::Buffer a(begin(data5) , end(data5) , readOnly=true, useHostPtr=false);
    cl::Buffer b(begin(data6) , end(data6) , readOnly=true, useHostPtr=false);
    cl::Buffer c(CL_MEM_READ_WRITE, elements * sizeof(float));

    bool build;
    cl::Program addProg(R"add(
        kernel
        void add(   global const float * restrict const a,
                    global const float * restrict const b,
                    global       float * restrict const c) 
        {
            unsigned idx = get_global_id(0);
            c[idx] = a[idx] + b[idx];
        }
    )add", build=true);

    auto add = cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer>(addProg, "add");

    stopWatch.Stop();
    std::cout << stopWatch.usecs() << " microseconds for setup of " << elements << " floats, compiling the OpenCL kernel\n";
    // 2017744 microseconds for setup of 100000000 floats, compiling the OpenCL kernel

    stopWatch.Start();

    add(cl::EnqueueArgs(elements), a, b, c);

    std::vector<float> result(elements);
    cl::copy(c, begin(result), end(result));

    stopWatch.Stop();
    std::cout << stopWatch.usecs() << " microseconds for adding " << elements << " floats\n";

    if(result.size() < 100) // creating vectors with millions of entries.
      std::copy(begin(result), end(result), std::ostream_iterator<float>(std::cout, ", "));
    else // print the first 10 entries
      std::cout<<result[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<result[i]; std::cout<<"\n";

  } catch (const cl::Error& e) {
      std::cerr << "threw cl::Error: " << e.what() << "(" << e.err() << ")" << std::endl;
  } catch (const std::exception& e) {
      std::cerr << "threw std::exception: " << e.what() << "\n";
  }
}
