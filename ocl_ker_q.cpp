// for i in g++-5 g++ clang++ ; do $i -std=c++11 -Ofast -I /opt/AMDAPPSDK-3.0-0-Beta/include/ ocl_ker_q.cpp -lOpenCL -o ocl_ker_q && ./ocl_ker_q ; done

#include <vector>       // want std::vector, not cl::vector
#include <iostream>     // std::cout
#include <exception>    // want to catch std::bad_alloc + friends
#include <chrono>       // C++11 timers

#pragma GCC diagnostic ignored "-Wdeprecated-declarations" // ignore Khronos cl.hpp header file warnings
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

class Timer { // C++11 chrono stop-watch timer class. Needs "#include <chrono>".
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
 void Start() { start  = std::chrono::high_resolution_clock::now(); }
 void Stop()  { stop   = std::chrono::high_resolution_clock::now(); }
 uint64_t msecs() {
    typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    uint64_t ms = duration_get.count();
    return ms;
 }
 uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t us = duration_get.count();
    return us;
 }
 uint64_t nsecs() {
    typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    uint64_t ns = duration_get.count();
    return ns;
 }
};

int main(int argc, char* argv[]) 
{
  std::cout << "+++++++++++++++++++++++++ compiler version " << __VERSION__ << "\n";

  Timer stopWatch;

  // size_t elements = 100 * 1000 * 1000; // too big for HP laptop
  // cl::Error: clCreateBuffer(-61)
  size_t elements = 30 * 1000 * 1000;
  if(argc  > 1) elements = atoi(argv[1]);

  try {
    stopWatch.Start();
    std::vector<float> vecA(elements, 5.50f), vecB(elements, 3.25f), vecC(elements);

    cl::Context ctx = cl::Context(CL_DEVICE_TYPE_GPU);    // CL_DEVICE_TYPE_DEFAULT CL_DEVICE_TYPE_CPU

    bool readOnly, useHostPtr;
    cl::Buffer a(begin(vecA), end(vecA), readOnly=true, useHostPtr=false);
    cl::Buffer b(begin(vecB), end(vecB), readOnly=true, useHostPtr=false);
    cl::Buffer c(CL_MEM_READ_WRITE, elements * sizeof(float));

    bool build;
    cl::Program kernels(R"kernels(
        kernel
        void k1(   global const float * restrict const a,
                   global const float * restrict const b,
		   global       float * restrict const c) 
        { unsigned idx = get_global_id(0); c[idx] = a[idx] + b[idx]; }

        kernel
        void k2(   global const float * restrict const a,
                   global const float * restrict const b,
		   global       float * restrict const c) 
        { unsigned idx = get_global_id(0); c[idx] += a[idx] * b[idx]; }
    )kernels", build=true);

    auto k1   = cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer>(kernels, "k1");
    auto k2   = cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer>(kernels, "k2");

    stopWatch.Stop();
    std::cout << stopWatch.usecs() << " microseconds for setup of 3 vectors of " << elements/1e6 << " million floats each, compiling the kernels\n";

    //  run the kernels
    stopWatch.Start();
    k1(cl::EnqueueArgs(elements), a, b, c);
    cl::CommandQueue::getDefault().finish();  // wait for kernel 1
    stopWatch.Stop();
    uint64_t us_k1  = stopWatch.usecs();

    stopWatch.Start();
    k2(cl::EnqueueArgs(elements), a, b, c);
    cl::CommandQueue::getDefault().finish();  // wait for kernel 2
    stopWatch.Stop();
    uint64_t us_k2  = stopWatch.usecs();

    stopWatch.Start();
    k1(cl::EnqueueArgs(elements), a, b, c);
    k2(cl::EnqueueArgs(elements), a, b, c);
    cl::CommandQueue::getDefault().finish();  // wait for kernels 1+2
    stopWatch.Stop();
    uint64_t us_k12 = stopWatch.usecs();

    // read-back result
    stopWatch.Start();
    cl::copy(c, begin(vecC), end(vecC));
    stopWatch.Stop();
    uint64_t us_readback = stopWatch.usecs();

    std::cout << us_k1  << " microseconds for running kernel 1\n";
    std::cout << us_k2  << " microseconds for running kernel 2\n";
    std::cout << us_k12  << " microseconds for running kernels 1+2\n";
    std::cout << us_readback << " microseconds for read-back of " << elements/1e6 << " million floats\n";

    // print the first 10 entries
    std::cout<<vecC[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<vecC[i]; std::cout<<"\n";
    // cl::clReleaseMemObject :- deprecated in 1.2 ??
    // cl::clReleaseMemObject(a);
    // cl::clReleaseMemObject(b);
    // cl::clReleaseMemObject(c);
  } catch (const cl::Error& e) {
      std::cerr << "threw cl::Error: " << e.what() << "(" << e.err() << ")\n";
  }
}
