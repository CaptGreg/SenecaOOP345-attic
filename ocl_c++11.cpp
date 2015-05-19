// GB, not sure how effective this code is at utilizing the GPU
// GB - try two pass, P1 = elements rounded to 256, P2 = balance

// GB timings April 19, 2015 on 
//  a 2010 vintage AMD 1100T 6-core CPU 
//  a 2010 vintage Radeon HD 6970 GPU
// 2017744 microseconds for setup of 100000000 floats, compiling the OpenCL kernel
// 4391718 microseconds for adding 100000000 floats // cl::Context(CL_DEVICE_TYPE_GPU);

// GB timings May 18, 2015 on 
//  a 2010 vintage AMD 1100T 6-core CPU 
//  a 2010 vintage Radeon HD 6970 GPU
// clang++ -Wall -std=c++11  -Ofast  ocl_c++11.cpp -lOpenCL -o ocl_c++11 -pthread
// 2017744 microseconds for setup of 100000000 floats, compiling the OpenCL kernel
// +++++++++++++++++++++++++
// clang++ compiler 4.2.1 Compatible Clang 3.3 (tags/RELEASE_33/final)
// +++++++++++++++++++++++++
// 7643138 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 109509 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 90243 microseconds for vector setup of 20000000 floats
// 63154 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 90140 microseconds for vector setup of 20000000 floats
// 1136568 microseconds for adding 20000000 vector elements, with 6 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 92864 microseconds for valarray setup of 20000000 floats
// 46313 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++


// GB timings May 18, 2015 on 
// on an inexpensive AMD E1, dual core laptop with integrated GPU/CPU (no need to copy CPU<->GPU):

// g++ -std=c++11 -pthread -I /opt/AMDAPPSDK-3.0-0-Beta/include/  ocl_c++11.cpp -lOpenCL -o ocl_c++11 && ./ocl_c++11 
// +++++++++++++++++++++++++
// 1610079 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 282017 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 552950 microseconds for vector setup of 20000000 floats
// 517257 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 393789 microseconds for valarray setup of 20000000 floats
// 834395 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++

// g++ -std=c++11 -Ofast -pthread -I /opt/AMDAPPSDK-3.0-0-Beta/include/  ocl_c++11.cpp -lOpenCL -o ocl_c++11 && ./ocl_c++11 
// +++++++++++++++++++++++++
// 1474919 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 212118 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 338470 microseconds for vector setup of 20000000 floats
// 130814 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 331069 microseconds for valarray setup of 20000000 floats
// 125577 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++

// +++++++++++++++++++++++++
// 1610079 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 282017 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 552950 microseconds for vector setup of 20000000 floats
// 517257 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 393789 microseconds for valarray setup of 20000000 floats
// 834395 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++

// clinfo says 2 compute units and may be 256 compute elements per compute unit
//   Max compute units:				 2
//   Kernel Preferred work group size multiple:	 64
//   Max work group size:			 256
//   Max memory allocation:			 142344192  // GB: only 142 MB of memory, or 142M x 4-byte memory elements?
//                                                          // GB: our 3 20M float element buffers are 240 MBytes
//                                                          // GB: 3 35M float element buffers are 420 MBytes
//                                                          // GB: 3 36M float element buffers are 426 MBytes <-- throws

//  clinfo for E1 laptop:
// Number of platforms:				 1
//   Platform Profile:				 FULL_PROFILE
//   Platform Version:				 OpenCL 2.0 AMD-APP (1642.5)
//   Platform Name:				 AMD Accelerated Parallel Processing
//   Platform Vendor:				 Advanced Micro Devices, Inc.
//   Platform Extensions:			 cl_khr_icd cl_amd_event_callback cl_amd_offline_devices 
//   Platform Name:				 AMD Accelerated Parallel Processing
// Number of devices:				 2
//   Device Type:				 CL_DEVICE_TYPE_GPU
//   Board name:				 AMD Radeon(TM) R2 Graphics  
//   Device Topology:				 PCI[ B#0, D#1, F#0 ]
//   Max compute units:				 2
//   Max work group size:			 256
//   Max memory allocation:			 142344192
//   Cache size:				 16384
//   Global memory size:			 569376768
//   Constant buffer size:			 65536
//   Max number of constant args:		 8
//   Local memory type:				 Scratchpad
//   Local memory size:				 32768
//   Kernel Preferred work group size multiple:	 64
//   Unified memory for Host and Device:	 1
//   Name:					 Mullins
//   Device OpenCL C version:			 OpenCL C 2.0 
//   Profile:					 FULL_PROFILE
//   Version:					 OpenCL 2.0 AMD-APP (1642.5)
//   Extensions:				 cl_khr_fp64 cl_amd_fp64 cl_khr_global_int32_base_atomics cl_khr_global_int32_extended_atomics cl_khr_local_int32_base_atomics cl_khr_local_int32_extended_atomics cl_khr_int64_base_atomics cl_khr_int64_extended_atomics cl_khr_3d_image_writes cl_khr_byte_addressable_store cl_khr_gl_sharing cl_ext_atomic_counters_32 cl_amd_device_attribute_query cl_amd_vec3 cl_amd_printf cl_amd_media_ops cl_amd_media_ops2 cl_amd_popcnt cl_khr_image2d_from_buffer cl_khr_spir cl_khr_subgroups cl_khr_gl_event cl_khr_depth_images 
// 


// NOTE compiler warnings: clCreateSampler + clCreateCommandQueue deprecated
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <vector>       // want std::vector, not cl::vector
#include <iostream>     // std::cout
#include <exception>    // std::bad_alloc + friends
#include <chrono>       // timers
#include <iterator>     // ostream_iterator
#include <valarray>     // C++11 optimized array
#include <thread>       // C++11 threads
// #include <algorithm> // compiles without

// using namespace cl;
// using namespace std;

std::string now() {
    time_t t = time(0);
    char buffer[9] = {0};

    strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&t));
    return std::string(buffer);
}

class Timer { // use C++11 std::chrono features to create a stop-watch timer class
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
  std::cout << "+++++++++++++++++++++++++\n";
  #ifdef __GNUC__
    #ifdef __clang__
      std::cout << "clang++ compiler " << __VERSION__ << "\n";
    #else
      std::cout << "g++ compiler " << __VERSION__ << "\n";
    #endif
  #endif

  try {
    Timer stopWatch;

    std::cout << "+++++++++++++++++++++++++\n";
    stopWatch.Start();

    // ::size_t elements = 100 * 1000 * 1000; // too big for HP laptop
    // cl::Error: clCreateBuffer(-61)
    size_t elements = 20 * 1000 * 1000;
    if(argc  > 1) elements = atoi(argv[1]);

    std::vector<float> vecA(elements, 5.50f);
    std::vector<float> vecB(elements, 3.25f);

    // cl::Context(CL_DEVICE_TYPE_DEFAULT);   // 4405826 microseconds for adding 100000000 floats
    cl::Context(CL_DEVICE_TYPE_GPU);    // 4391718 microseconds for adding 100000000 floats
    // cl::Context(CL_DEVICE_TYPE_CPU);    // 4396792 microseconds for adding 100000000 floats

    bool readOnly;
    bool useHostPtr=false; // AMD E1 laptop: useHostPtr=true is 5 x slower 985 ms vs 170 ms
    cl::Buffer a(begin(vecA), end(vecA), readOnly=true, useHostPtr);
    cl::Buffer b(begin(vecB), end(vecB), readOnly=true, useHostPtr);
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

#define NAIVE
#ifdef NAIVE
    // all at once, runs on GCD(elements,256) compute elements per compute unit
    add(cl::EnqueueArgs(elements), a, b, c);
#else
    // two pass
    // not much diff between elements: 64, 128, or 256
    // size_t chunk =  64 * (elements /  64);   // 221997
    // size_t chunk = 128 * (elements / 128);   // 220362
    size_t chunk = 256 * (elements / 256);      // 215210
    add(cl::EnqueueArgs(chunk), a, b, c);  // max out compute units
    // add(cl::EnqueueArgs(elements - chunk), a+chunk, b+chunk, c+chunk);
    add(cl::EnqueueArgs(elements - chunk), a, b, c); // need to do balance, not re-do first bit
#endif

    std::vector<float> vecC(elements);
    cl::copy(c, begin(vecC), end(vecC));

    stopWatch.Stop();
    std::cout << stopWatch.usecs() << " microseconds for adding " << elements << " floats\n";

    if(vecC.size() < 100) // creating vectors with millions of entries.
      std::copy(begin(vecC), end(vecC), std::ostream_iterator<float>(std::cout, ", "));
    else { // print the first 10 entries
      std::cout<<vecC[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<vecC[i]; std::cout<<"\n";
    }

    std::cout << "+++++++++++++++++++++++++\n";
    {
      stopWatch.Start();
      std::vector<float>  vec_a(elements, 5.50f);
      std::vector<float>  vec_b(elements, 3.25f);
      std::vector<float>  vec_c(elements);
      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for vector setup of " << elements << " floats\n";
      stopWatch.Start();
      // vec_c = vec_a + vec_b; // works with std::valarray, not with std::vector
      for(int i = vec_a.size() - 1; i >= 0; i--)
          vec_c[i] = vec_a[i] + vec_b[i];
      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for adding " << elements << " vector elements, for(...) c[i] = a[i] + b[i]\n";

      if(vec_c.size() < 100) // creating vectors with millions of entries.
        std::copy(begin(vec_c), end(vec_c), std::ostream_iterator<float>(std::cout, ", "));
      else { // print the first 10 entries
        std::cout<<vec_c[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<vec_c[i]; std::cout<<"\n";
      }
    }
    std::cout << "+++++++++++++++++++++++++\n";
    {
      stopWatch.Start();
      std::vector<float>  vec_a(elements, 5.50f);
      std::vector<float>  vec_b(elements, 3.25f);
      std::vector<float>  vec_c(elements);
      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for vector setup of " << elements << " floats\n";
      stopWatch.Start();

      int num_procs = std::thread::hardware_concurrency();
      std::vector<std::thread> t(num_procs);
      size_t thread_chunk = elements / num_procs;
      for(int n = 0; n < num_procs; n++) {
          size_t s = n * thread_chunk;
          size_t e = s + thread_chunk;
          if(n == num_procs - 1) e = elements;
          auto f =  [vec_a, vec_b, &vec_c] (size_t s, size_t e) 
              {
                  for(size_t i = s; i < e; i++)
                      vec_c[i] = vec_a[i] + vec_b[i];
               };
          t[n] = std::thread(f, s, e);
      }
      for(auto& e: t) e.join();

      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for adding " 
                << elements << " vector elements, with " << num_procs << " threads\n";

      if(vec_c.size() < 100) // creating vectors with millions of entries.
        std::copy(begin(vec_c), end(vec_c), std::ostream_iterator<float>(std::cout, ", "));
      else { // print the first 10 entries
        std::cout<<vec_c[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<vec_c[i]; std::cout<<"\n";
      }
    }
    std::cout << "+++++++++++++++++++++++++\n";
    {
      stopWatch.Start();
      std::valarray<float>  va_a(5.50f, elements);
      std::valarray<float>  va_b(3.25f, elements);
      std::valarray<float>  va_c(elements);
      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for valarray setup of " << elements << " floats\n";
      stopWatch.Start();
      va_c = va_a + va_b;
      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for " << elements << " valarray elements, c = a + b\n";

      if(va_c.size() < 100) // creating vectors with millions of entries.
        std::copy(begin(va_c), end(va_c), std::ostream_iterator<float>(std::cout, ", "));
      else { // print the first 10 entries
        std::cout<<va_c[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<va_c[i]; std::cout<<"\n";
      }

    }
    std::cout << "+++++++++++++++++++++++++\n";
  } catch (const cl::Error& e) {
      std::cerr << "threw cl::Error: " << e.what() << "(" << e.err() << ")" << std::endl;
  } catch (const std::exception& e) {
      std::cerr << "threw std::exception: " << e.what() << "\n";
  }
}
