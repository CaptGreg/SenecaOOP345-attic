// GB, not sure how effective this code is at utilizing the GPU
// GB - try two pass, P1 = elements rounded to 256, P2 = balance

// GB timings May 20, 2015 T2108 hot machine, no OpenCL, Ubuntu 14.04 
// +++++++++++++++++++++++++
// g++ compiler version 4.9.2
// GenuineIntel Intel(R) Core(TM) i7-4771 CPU @ 3.50GHz
// This is a 8 core machine.
// +++++++++++++++++++++++++
// threw cl::Error: clGetPlatformIDs(-1001)
// +++++++++++++++++++++++++
// 24025 microseconds for vector setup of 20000000 floats
// 19173 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 24284 microseconds for vector setup of 20000000 floats
// 552601 microseconds for adding 20000000 vector elements, using 8 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 23793 microseconds for vector setup of 20000000 floats
// all threads flying
// 521487 microseconds for starting up 8 threads
// 21696 microseconds to add 20000000 vector elements, using 8 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 24119 microseconds for valarray setup of 20000000 floats
// 19723 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// +++++++++++++++++++++++++
// clang++ compiler version 4.2.1 Compatible Ubuntu Clang 3.4 (tags/RELEASE_34/final)
// GenuineIntel Intel(R) Core(TM) i7-4771 CPU @ 3.50GHz
// This is a 8 core machine.
// +++++++++++++++++++++++++
// threw cl::Error: clGetPlatformIDs(-1001)
// +++++++++++++++++++++++++
// 23886 microseconds for vector setup of 20000000 floats
// 20822 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 23985 microseconds for vector setup of 20000000 floats
// 510437 microseconds for adding 20000000 vector elements, using 8 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 23758 microseconds for vector setup of 20000000 floats
// all threads flying
// 492450 microseconds for starting up 8 threads
// 22297 microseconds to add 20000000 vector elements, using 8 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 24176 microseconds for valarray setup of 20000000 floats
// 19416 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++


// GB timings May 20, 2015 OpenLAB machine, no OpenCL, Ubuntu 14.04 
// +++++++++++++++++++++++++
// clang++ compiler version 4.2.1 Compatible Ubuntu Clang 3.4 (tags/RELEASE_34/final)
// GenuineIntel Intel(R) Core(TM)2 Quad CPU    Q9550  @ 2.83GHz
// This is a 4 core machine.
// +++++++++++++++++++++++++
// threw cl::Error: clGetPlatformIDs(-1001)
// +++++++++++++++++++++++++
// 200177 microseconds for vector setup of 20000000 floats
// 234508 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 199985 microseconds for vector setup of 20000000 floats
// 7629568 microseconds for adding 20000000 vector elements, using 4 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 202076 microseconds for vector setup of 20000000 floats
// all threads flying
// 7569754 microseconds for starting up 4 threads
// 80855 microseconds to add 20000000 vector elements, using 4 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 194200 microseconds for valarray setup of 20000000 floats
// 390204 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++


// GB timings May 19, 2015 on 
//  a 2010 vintage AMD 1100T 6-core CPU 
//  a 2010 vintage Radeon HD 6970 GPU
//  g++-5 -Wall -std=c++11 -Ofast -pthread  ocl_c++11.cpp -lOpenCL -o ocl_c++11 && ./ocl_c++11 
// +++++++++++++++++++++++++
// g++ compiler version 5.1.0
// AuthenticAMD AMD Phenom(tm) II X6 1100T Processor
// This is a 6 core machine.
// +++++++++++++++++++++++++
// 481938 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 112918 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 91863 microseconds for vector setup of 20000000 floats
// 54214 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 93778 microseconds for vector setup of 20000000 floats
// 1197533 microseconds for adding 20000000 vector elements, with 6 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 92231 microseconds for valarray setup of 20000000 floats
// 50321 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
//  g++ -Wall -std=c++11 -Ofast -pthread  ocl_c++11.cpp -lOpenCL -o ocl_c++11 && ./ocl_c++11 ; !2018
// +++++++++++++++++++++++++
// g++ compiler version 4.9.2
// AuthenticAMD AMD Phenom(tm) II X6 1100T Processor
// This is a 6 core machine.
// +++++++++++++++++++++++++
// 466709 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 114916 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 94316 microseconds for vector setup of 20000000 floats
// 51819 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 94116 microseconds for vector setup of 20000000 floats
// 1357244 microseconds for adding 20000000 vector elements, with 6 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 94430 microseconds for valarray setup of 20000000 floats
// 53021 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// clang++ compiler version 4.2.1 Compatible Clang 3.3 (tags/RELEASE_33/final)
// AuthenticAMD AMD Phenom(tm) II X6 1100T Processor
// This is a 6 core machine.
// +++++++++++++++++++++++++
// 506386 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 78573 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 93596 microseconds for vector setup of 20000000 floats
// 67436 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 94592 microseconds for vector setup of 20000000 floats
// 1178448 microseconds for adding 20000000 vector elements, using 6 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 93573 microseconds for vector setup of 20000000 floats
// all threads flying
// 1138494 microseconds for starting up 6 threads
// 44623 microseconds to add 20000000 vector elements, using 6 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 90942 microseconds for valarray setup of 20000000 floats
// 53305 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++

// clinfo
//   Platform Profile:				 FULL_PROFILE
//   Platform Version:				 OpenCL 1.2 AMD-APP (1214.3)
//   Platform Name:				 AMD Accelerated Parallel Processing
//   Platform Vendor:				 Advanced Micro Devices, Inc.
//   Platform Extensions:			 cl_khr_icd cl_amd_event_callback cl_amd_offline_devices
//   Platform Name:				 AMD Accelerated Parallel Processing
//   Device Type:				 CL_DEVICE_TYPE_GPU
//   Board name:				 AMD Radeon HD 6900 Series
//   Max compute units:				 24
//   Max work items dimensions:			 3
//     Max work items[0]:			 256
//     Max work items[1]:			 256
//     Max work items[2]:			 256
//   Max work group size:			 256
//   Max clock frequency:			 880Mhz
//   Max memory allocation:			 536870912
//   Global memory size:			 2075131904
//   Constant buffer size:			 65536
//   Local memory size:				 32768
//   Kernel Preferred work group size multiple:	 64
//   Platform ID:				 0x00007f70428dffc0
//   Name:					 Cayman
//   Vendor:					 Advanced Micro Devices, Inc.
//   Device OpenCL C version:			 OpenCL C 1.2 
//   Driver version:				 1214.3 (VM)
//   Profile:					 FULL_PROFILE
//   Version:					 OpenCL 1.2 AMD-APP (1214.3)
//   Extensions:				 cl_khr_fp64 cl_amd_fp64 cl_khr_global_int32_base_atomics cl_khr_global_int32_extended_atomics cl_khr_local_int32_base_atomics cl_khr_local_int32_extended_atomics cl_khr_3d_image_writes cl_khr_byte_addressable_store cl_khr_gl_sharing cl_ext_atomic_counters_32 cl_amd_device_attribute_query cl_amd_vec3 cl_amd_printf cl_amd_media_ops cl_amd_media_ops2 cl_amd_popcnt cl_amd_image2d_from_buffer_read_only 

// 
// GB timings May 19, 2015 on 
//  a 2010 MSII gx660 gaming laptop with an Intel I5-460M 4-core mobile CPU 
//  and a Radeon HD 5870 GPU
// +++++++++++++++++++++++++
// g++ compiler 4.9.2
// +++++++++++++++++++++++++
// 295768 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 81969 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 62395 microseconds for vector setup of 20000000 floats
// 40387 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 68573 microseconds for vector setup of 20000000 floats
// 779080 microseconds for adding 20000000 vector elements, with 4 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 68927 microseconds for valarray setup of 20000000 floats
// 43695 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++

//clinfo for HD5870 laptop GPU
//  Platform Profile:				 FULL_PROFILE
//  Platform Version:				 OpenCL 2.0 AMD-APP (1642.5)
//  Platform Name:				 AMD Accelerated Parallel Processing
//  Platform Vendor:				 Advanced Micro Devices, Inc.
//  Platform Name:				 AMD Accelerated Parallel Processing
//  Device Type:				 CL_DEVICE_TYPE_GPU
//  Vendor ID:					 1002h
//  Board name:					 AMD Mobility Radeon HD 5800 Series
//  Max compute units:				 10
//  Max work group size:			 256
//  Max memory allocation:			 134217728
//  Cache size:					 0
//  Global memory size:				 536870912
//  Constant buffer size:			 65536
//  Local memory size:				 32768
//  Kernel Preferred work group size multiple:	 64
//  Unified memory for Host and Device:		 0
//  Platform ID:				 0x7f9343f47fd0
//  Name:					 Juniper
//  Vendor:					 Advanced Micro Devices, Inc.
//  Device OpenCL C version:			 OpenCL C 1.2 
//  Driver version:				 1642.5
//  Profile:					 FULL_PROFILE
//  Version:					 OpenCL 1.2 AMD-APP (1642.5)
//  Extensions:					 cl_khr_global_int32_base_atomics cl_khr_global_int32_extended_atomics cl_khr_local_int32_base_atomics cl_khr_local_int32_extended_atomics cl_khr_3d_image_writes cl_khr_byte_addressable_store cl_khr_gl_sharing cl_ext_atomic_counters_32 cl_amd_device_attribute_query cl_amd_vec3 cl_amd_printf cl_amd_media_ops cl_amd_media_ops2 cl_amd_popcnt cl_amd_image2d_from_buffer_read_only cl_khr_spir cl_khr_gl_event 



// GB timings May 18, 2015 on 
// on an inexpensive AMD E1, dual core laptop with integrated GPU/CPU (no need to copy CPU<->GPU):

// for i in g++-5 g++ clang++ ; do $i -std=c++11 -Ofast  -I /opt/AMDAPPSDK-3.0-0-Beta/include/  ocl_c++11.cpp -lOpenCL -o ocl_c++11 -pthread && ./ocl_c++11; done
// +++++++++++++++++++++++++
// g++ compiler version 5.1.0
// AuthenticAMD AMD E1-6010 APU with AMD Radeon R2 Graphics    
// This is a 2 core machine.
// +++++++++++++++++++++++++
// 1262528 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 181426 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 214985 microseconds for vector setup of 20000000 floats
// 94360 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 219469 microseconds for vector setup of 20000000 floats
// 660190 microseconds for adding 20000000 vector elements, with 2 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 213201 microseconds for valarray setup of 20000000 floats
// 94375 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// +++++++++++++++++++++++++
// g++ compiler version 4.9.2
// AuthenticAMD AMD E1-6010 APU with AMD Radeon R2 Graphics    
// This is a 2 core machine.
// +++++++++++++++++++++++++
// 1273511 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 183697 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 217390 microseconds for vector setup of 20000000 floats
// 95378 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 218317 microseconds for vector setup of 20000000 floats
// 933871 microseconds for adding 20000000 vector elements, with 2 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 219812 microseconds for valarray setup of 20000000 floats
// 92750 microseconds for 20000000 valarray elements, c = a + b
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// +++++++++++++++++++++++++
// clang++ compiler version 4.2.1 Compatible Ubuntu Clang 3.4 (tags/RELEASE_34/final)
// AuthenticAMD AMD E1-6010 APU with AMD Radeon R2 Graphics    
// This is a 2 core machine.
// +++++++++++++++++++++++++
// 1264073 microseconds for setup of 20000000 floats, compiling the OpenCL kernel
// 187613 microseconds for adding 20000000 floats
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 211946 microseconds for vector setup of 20000000 floats
// 107473 microseconds for adding 20000000 vector elements, for(...) c[i] = a[i] + b[i]
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 217798 microseconds for vector setup of 20000000 floats
// 755129 microseconds for adding 20000000 vector elements, with 2 threads
// 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75, 8.75
// +++++++++++++++++++++++++
// 212545 microseconds for valarray setup of 20000000 floats
// 92757 microseconds for 20000000 valarray elements, c = a + b
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
//   Local memory size:				 32768
//   Kernel Preferred work group size multiple:	 64
//   Unified memory for Host and Device:	 1
//   Name:					 Mullins
//   Device OpenCL C version:			 OpenCL C 2.0 
//   Profile:					 FULL_PROFILE
//   Version:					 OpenCL 2.0 AMD-APP (1642.5)
//   Extensions:				 cl_khr_fp64 cl_amd_fp64 cl_khr_global_int32_base_atomics cl_khr_global_int32_extended_atomics cl_khr_local_int32_base_atomics cl_khr_local_int32_extended_atomics cl_khr_int64_base_atomics cl_khr_int64_extended_atomics cl_khr_3d_image_writes cl_khr_byte_addressable_store cl_khr_gl_sharing cl_ext_atomic_counters_32 cl_amd_device_attribute_query cl_amd_vec3 cl_amd_printf cl_amd_media_ops cl_amd_media_ops2 cl_amd_popcnt cl_khr_image2d_from_buffer cl_khr_spir cl_khr_subgroups cl_khr_gl_event cl_khr_depth_images 
// 


#include <vector>       // want std::vector, not cl::vector
#include <iostream>     // std::cout
#include <exception>    // std::bad_alloc + friends
#include <chrono>       // timers
#include <iterator>     // ostream_iterator
#include <valarray>     // C++11 optimized array
#include <thread>       // C++11 threads
#include <future>
#include <mutex>
#include <condition_variable>
#include <memory>        // unique_ptr

// NOTE compiler warnings: clCreateSampler + clCreateCommandQueue deprecated
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" // ignore Khronos cl.hpp header file warnings
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

// #include <algorithm> // compiles without

// using namespace cl;
// using namespace std;

std::string now() {
    time_t t = time(0);
    char buffer[9] = {0};

    strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&t));
    return std::string(buffer);
}

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

#ifdef __GNUC__
static inline void cpuid(uint32_t op, uint32_t *eax, uint32_t *ebx,
  uint32_t *ecx, uint32_t *edx)
{
  asm("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (op) : "cc");
}
static inline uint32_t cpuid_string(uint32_t op, uint32_t r[4]) 
{
  asm volatile("cpuid":"=a"(*r),"=b"(*(r+1)), "=c"(*(r+2)),"=d"(*(r+3)):"a"(op));
  return (uint32_t)r[0];
}
char *CPUVendor(void)
{
   uint32_t eax, ebx, ecx, edx;
   static uint32_t Vendor[4];

   eax = 0; cpuid(eax, &eax, &ebx, &ecx, &edx);
   Vendor[0] = ebx; Vendor[1] = edx; Vendor[2] = ecx; Vendor[3] = 0;
   return (char *) Vendor;
}
char *CPUName(void)
{
  uint32_t eax;
  static uint32_t Name[13];
  //                           &eax      &ebx      &ecx      &edx
  eax = 0x80000002; cpuid(eax, Name + 0, Name + 1, Name + 2, Name + 3);
  eax = 0x80000003; cpuid(eax, Name + 4, Name + 5, Name + 6, Name + 7);
  eax = 0x80000004; cpuid(eax, Name + 8, Name + 9, Name +10, Name +11);
  Name[12] = 0;

  return (char *) Name;
}

#endif

int main(int argc, char* argv[]) 
{
  std::cout << "+++++++++++++++++++++++++\n";
  #ifdef __GNUC__
    #ifdef __clang__
      std::cout << "clang++ compiler version " << __VERSION__ << "\n";
    #else
      std::cout << "g++ compiler version " << __VERSION__ << "\n";
    #endif
   std::cout << CPUVendor() << " " << CPUName() << "\n";

  #endif

  std::cout << "This is a " << std::thread::hardware_concurrency() << " core machine.\n";

  #ifdef CL_VERSION_2_0           
    std::cout << "CL_VERSION 2.0\n";
  #elif CL_VERSION_1_2
    std::cout << "CL_VERSION 1.2\n";
  #elif CL_VERSION_1_1
    std::cout << "CL_VERSION 1.1\n";
  #elif CL_VERSION_1_0
    std::cout << "CL_VERSION 1.0\n";
  #else
    std::cout << "Unknown CL_VERSION\n";
  #endif


  Timer stopWatch;

  // ::size_t elements = 100 * 1000 * 1000; // too big for HP laptop
  // cl::Error: clCreateBuffer(-61)
  size_t elements = 20 * 1000 * 1000;
  if(argc  > 1) elements = atoi(argv[1]);

  try {

    std::cout << "+++++++++++++++++++++++++\n";
    stopWatch.Start();

    std::vector<float> vecA(elements, 5.50f), vecB(elements, 3.25f), vecC(elements);

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

// #define NAIVE
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

    cl::copy(c, begin(vecC), end(vecC));

    stopWatch.Stop();
    std::cout << stopWatch.usecs() << " microseconds for adding " << elements << " floats\n";

    if(vecC.size() < 100) // creating vectors with millions of entries.
      std::copy(begin(vecC), end(vecC), std::ostream_iterator<float>(std::cout, ", "));
    else { // print the first 10 entries
      std::cout<<vecC[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<vecC[i]; std::cout<<"\n";
    }
  } catch (const cl::Error& e) {
      std::cerr << "threw cl::Error: " << e.what() << "(" << e.err() << ")" << "\n";
  } catch (const std::exception& e) {
      std::cerr << "threw std::exception: " << e.what() << "\n";
  }
  try {

    std::cout << "+++++++++++++++++++++++++\n";
    {
      stopWatch.Start();
      std::vector<float>  vec_a(elements, 5.50f), vec_b(elements, 3.25f), vec_c(elements);
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
      std::vector<float>  vec_a(elements, 5.50f), vec_b(elements, 3.25f), vec_c(elements);
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
          auto f =  [vec_a, vec_b, &vec_c, s, e] () 
              {
                  for(size_t i = s; i < e; i++)
                      vec_c[i] = vec_a[i] + vec_b[i];
               };
          t[n] = std::thread(f);
      }
      for(auto& e: t) e.join();

      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for adding " 
                << elements << " vector elements, using " << num_procs << " threads\n";

      if(vec_c.size() < 100) // creating vectors with millions of entries.
        std::copy(begin(vec_c), end(vec_c), std::ostream_iterator<float>(std::cout, ", "));
      else { // print the first 10 entries
        std::cout<<vec_c[0]; for(int i = 1 ; i < 10 ; i++ ) std::cout<<", "<<vec_c[i]; std::cout<<"\n";
      }
    }
    std::cout << "+++++++++++++++++++++++++\n";
    {
      // Start up threads and measure the time the threads worked on the problem.
      stopWatch.Start();
      std::vector<float>  vec_a(elements, 5.50f), vec_b(elements, 3.25f), vec_c(elements);
      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for vector setup of " << elements << " floats\n";
      stopWatch.Start();

      int num_procs = std::thread::hardware_concurrency();
      std::vector<std::thread> t(num_procs);

      // std::promise<bool>       p[num_procs]; // g++ fine, clang++ error
      std::promise<bool>*      p = new std::promise<bool> [num_procs];
      // std::unique_ptr<std::promise<bool>> p(new std::promise<bool> [num_procs]);
      std::mutex               cvMutex;
      std::condition_variable  cv;
      bool                     everyoneReady = false;

      size_t thread_chunk = elements / num_procs;
      for(int n = 0; n < num_procs; n++) {
          size_t s = n * thread_chunk;
          size_t e = s + thread_chunk;
          if(n == num_procs - 1) e = elements;
          auto f =  [&p, everyoneReady, &cvMutex, &cv, vec_a, vec_b, &vec_c] (int n, size_t s, size_t e)
          // auto f =  [&] (int n, size_t s, size_t e) // args to ensure value at time of call is used
          // [&] method hangs with clang
              {
                  // let main know we are flying
                  p[n].set_value(true);

                  // wait until told to go
                  { // acquire lock
                    std::unique_lock<std::mutex> lock(cvMutex);
                    if(!everyoneReady) cv.wait(lock);
                  }

                  // DO IT!
                  for(size_t i = s; i < e; i++)
                      vec_c[i] = vec_a[i] + vec_b[i];
               };
          t[n] = std::thread(f, n, s, e);
      }

      // wait for threads to start
      for(int n = 0 ; n < num_procs; n++)  {auto f = p[n].get_future(); f.get(); }
      std::cout << "all threads flying\n";

      // GO!
      { // acquire lock
        std::unique_lock<std::mutex> lock(cvMutex);
        everyoneReady = true;
        cv.notify_all();
      }

      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds for starting up " << num_procs << " threads\n";
      
      stopWatch.Start();
      for(auto& e: t) e.join();
      stopWatch.Stop();
      std::cout << stopWatch.usecs() << " microseconds to add " 
                << elements << " vector elements, using " << num_procs << " threads\n";
      delete [] p;

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
  } catch (const std::exception& e) {
      std::cerr << "threw std::exception: " << e.what() << "\n";
  }
}
