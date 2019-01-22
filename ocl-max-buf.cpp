// OpenCL 1.+
// #define CL_ENABLE_EXCEPTIONS
// #include <CL/cl.hpp>

// OpenCL 2.+
#define CL_HPP_TARGET_OPENCL_VERSION 200
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>

/**   Doxygen documentation for this header is available here:
 *       http://khronosgroup.github.io/OpenCL-CLHPP/
 *   The latest version of this header can be found on the GitHub releases page:
 *       https://github.com/KhronosGroup/OpenCL-CLHPP/releases
 **/

// OpenCL cl_khr kernel extensions available on both GeForce 1070 and Radeon 580 (reported by this program)
#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_global_int32_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
#pragma OPENCL EXTENSION cl_khr_icd : enable
#pragma OPENCL EXTENSION cl_khr_gl_sharing : enable

// AMD OpenCL kernel extensions available on Radeon 580 (reported by this program)
#pragma OPENCL EXTENSION cl_khr_int64_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_int64_extended_atomics : enable
#pragma OPENCL EXTENSION cl_khr_3d_image_writes : enable
#pragma OPENCL EXTENSION cl_khr_fp16 : enable
#pragma OPENCL EXTENSION cl_khr_gl_sharing : enable
#pragma OPENCL EXTENSION cl_khr_subgroups : enable
#pragma OPENCL EXTENSION cl_khr_depth_images : enable
#pragma OPENCL EXTENSION cl_amd_device_attribute_query : enable
#pragma OPENCL EXTENSION cl_amd_media_ops : enable
#pragma OPENCL EXTENSION cl_amd_media_ops2 : enable
#pragma OPENCL EXTENSION cl_amd_copy_buffer_p2p : enable
#pragma OPENCL EXTENSION cl_amd_assembly_program : enable

// NVidia OpenCL kernel extensions available on GeForce 1070 (reported by this program)
#pragma OPENCL EXTENSON cl_nv_compiler_options : enable
#pragma OPENCL EXTENSON cl_nv_device_attribute_query : enable
#pragma OPENCL EXTENSON cl_nv_pragma_unroll : enable
#pragma OPENCL EXTENSON cl_nv_copy_opts : enable
#pragma OPENCL EXTENSON cl_nv_create_buffer : enable


#include <algorithm>   // sort
#include <iostream>
#include <vector>
using namespace std;
// using namespace cl;

// binary search is one of the most buggy code ever written
// search for T in sorted table A[]:  https://en.wikipedia.org/wiki/Binary_search_algorithm
//   step 1 Set L to 0 and R to n-1.
//   step 2 If L = R go to step 6.
//   step 3 Set m (the position of the middle element) to the ceiling of (L+R)/2 which is the least integer greater than or equal to (L+R)/2    calc (L+R+1)/2 
//   step 4 If A[m] > T set R to m−1 and go to step 2.
//   step 5 Set L to m and go to step 2.
//   step 6 Now L = R, the search is done. If A[L] = T, return L. Otherwise, the search terminates as unsuccessful.

int app(int argc, char*argv[]) 
{
    cl_ulong globalMemSize;
    cl_ulong maxMemAllocSize;
    cl_uint maxClockFreq;
    cl_int clErr;
    // cl_context context;
    void* host_ptr = nullptr;
    cl_int err;

    cout << "\n";

    // get all platforms
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
 
    auto printSortedList = [] (string& s) {
      vector<string> v;
      string e;
      for(auto c: s) if(c == ' ') v.push_back(move(e)); else 
          e += c;
      if(not e.empty()) v.push_back(move(e));

      sort(begin(v), end(v));

      for(auto& line: v) cout << "   " << line << "\n";
    };

    for (int P = 0; P < platforms.size(); P++) {
 
        string param;
        // platforms[P].getInfo(CL_PLATFORM_NAME, param);
        cout << "platform " << P << " name                                          = " << platforms[P].getInfo<CL_PLATFORM_NAME>() << "\n";
        cout << "platform " << P << " vendor                                        = " << platforms[P].getInfo<CL_PLATFORM_VENDOR>() << "\n";
        cout << "platform " << P << " version                                       = " << platforms[P].getInfo<CL_PLATFORM_VERSION>() << "\n";
        cout << "platform " << P << " profile                                       = " << platforms[P].getInfo<CL_PLATFORM_PROFILE>() << "\n";
        string ext = platforms[P].getInfo<CL_PLATFORM_EXTENSIONS>();
        cout << "platform " << P << " extensions:\n";
        printSortedList(ext);
        
        vector<cl::Device> devices;
        platforms[P].getDevices(CL_DEVICE_TYPE_ALL, &devices);

        // for each device print critical attributes
        for (int D = 0; D < devices.size(); D++) {
            cout << "platform " << P << " device " << D << " name                                 = " << devices[D].getInfo<CL_DEVICE_NAME>() << "\n";
            cout << "platform " << P << " device " << D << " driver version                       = " << devices[D].getInfo<CL_DRIVER_VERSION>() << "\n";
            cout << "platform " << P << " device " << D << " OpenCL C version                     = " << devices[D].getInfo<CL_DEVICE_OPENCL_C_VERSION>() << "\n";
            cout << "platform " << P << " device " << D << " profile                              = " << devices[D].getInfo<CL_DEVICE_PROFILE>() << "\n";
            globalMemSize = devices[D].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
            cout << "platform " << P << " device " << D << " global mem size                      = " << globalMemSize << " (" << 1e-9*globalMemSize << " GB)\n";
            maxMemAllocSize = devices[D].getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();
            cout << "platform " << P << " device " << D << " max mem alloc size                   = " << maxMemAllocSize << " (" << 1e-9*maxMemAllocSize << " GB)\n";
            maxClockFreq = devices[D].getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();
            cout << "platform " << P << " device " << D << " max clock frequency                  = " << maxClockFreq << " MHz\n";
            cout << "platform " << P << " device " << D << " address bits                         = " << devices[D].getInfo<CL_DEVICE_ADDRESS_BITS>() << "\n";
        try {
          #ifdef CL_DEVICE_PROFILING_TIMER_OFFSET_AMD
         // cout << "platform " << P << " device " << D << " profiling timer offset AMD           = " << devices[D].getInfo<CL_DEVICE_PROFILING_TIMER_OFFSET_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_GLOBAL_FREE_MEMORY_AMD
         // auto vec = devices[D].getInfo<CL_DEVICE_GLOBAL_FREE_MEMORY_AMD>(); // cl2.hpp: returns vector<size_t>
         // cout << "platform " << P << " device " << D << " global free memory AMD               =\n"; 
         // for(auto& e: vec) cout << e << " "; cout << "\n";
         // cout << "platform " << P << " device " << D << " global free memory AMD               = " << devices[D].getInfo<CL_DEVICE_GLOBAL_FREE_MEMORY_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD
            cout << "platform " << P << " device " << D << " SIMD per compute unit AMD            = " << devices[D].getInfo<CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_SIMD_WIDTH_AMD
            cout << "platform " << P << " device " << D << " SIMD width AMD                       = " << devices[D].getInfo<CL_DEVICE_SIMD_WIDTH_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD
            cout << "platform " << P << " device " << D << " SIMD instruction width AMD           = " << devices[D].getInfo<CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_WAVEFRONT_WIDTH_AMD
            cout << "platform " << P << " device " << D << " wavefront width AMD                  = " << devices[D].getInfo<CL_DEVICE_WAVEFRONT_WIDTH_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD
            cout << "platform " << P << " device " << D << " global memory channels AMD           = " << devices[D].getInfo<CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD
            cout << "platform " << P << " device " << D << " global memory channel banks AMD      = " << devices[D].getInfo<CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD
            cout << "platform " << P << " device " << D << " global memory channel bank width AMD = " << devices[D].getInfo<CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD
            cout << "platform " << P << " device " << D << " local mem size per compute unit AMD  = " << devices[D].getInfo<CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_LOCAL_MEM_BANKS_AMD
            cout << "platform " << P << " device " << D << " local mem banks AMD                  = " << devices[D].getInfo<CL_DEVICE_LOCAL_MEM_BANKS_AMD>() << "\n";
          #endif
          #ifdef CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV
         // cout << "platform " << P << " device " << D << " compute capability major NV          = " << devices[D].getInfo<CL_DEVICE_COMPUTE_CAPABILITY_MAJOR_NV>() << "\n";
          #endif
          #ifdef CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV
         // cout << "platform " << P << " device " << D << " compute capability minor NV          = " << devices[D].getInfo<CL_DEVICE_COMPUTE_CAPABILITY_MINOR_NV>() << "\n";
          #endif
          #ifdef CL_DEVICE_REGISTERS_PER_BLOCK_NV
         // cout << "platform " << P << " device " << D << " registers per block NV               = " << devices[D].getInfo<CL_DEVICE_REGISTERS_PER_BLOCK_NV>() << "\n";
          #endif
          #ifdef CL_DEVICE_WARP_SIZE_NV
         // cout << "platform " << P << " device " << D << " warp size NV                         = " << devices[D].getInfo<CL_DEVICE_WARP_SIZE_NV>() << "\n";
          #endif
          #ifdef CL_DEVICE_GPU_OVERLAP_NV
         // cout << "platform " << P << " device " << D << " GPU overlap NV                       = " << devices[D].getInfo<CL_DEVICE_GPU_OVERLAP_NV>() << "\n";
          #endif
          #ifdef CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV
         // cout << "platform " << P << " device " << D << " kernel exec timeout NV               = " << devices[D].getInfo<CL_DEVICE_KERNEL_EXEC_TIMEOUT_NV>() << "\n";
          #endif
          #ifdef CL_DEVICE_INTEGRATED_MEMORY_NV
         // cout << "platform " << P << " device " << D << " integrated memory NV                 = " << devices[D].getInfo<CL_DEVICE_INTEGRATED_MEMORY_NV>() << "\n";
          #endif
        } catch(...) {
          
        }

          try {
            cout << "platform " << P << " device " << D << " cl2 max pipe args                    = " << devices[D].getInfo<CL_DEVICE_MAX_PIPE_ARGS>() << "\n";
            cout << "platform " << P << " device " << D << " cl2 pipe max active reservations     = " << devices[D].getInfo<CL_DEVICE_PIPE_MAX_ACTIVE_RESERVATIONS>() << "\n";
            size_t ps =  devices[D].getInfo<CL_DEVICE_PIPE_MAX_PACKET_SIZE>();
            cout << "platform " << P << " device " << D << " cl2 pipe max packet size             = " << ps << " (" << 1e-9*ps << " GB)\n";
          } catch(...) {
            cout << "platform " << P << " device " << D << " no pipes\n";
          }

            cl::Context context(devices[D]);
            // binary search for max mem allocation
            size_t L = 0;
            size_t R = 3*globalMemSize;
            while(1) {
              if(L == R) 
                break;
              size_t m = (L+R+1)/2;
              // cout << "trying " << m << " ";
              try {
                cl::Buffer buf = cl::Buffer(context, CL_MEM_READ_WRITE, m, host_ptr, &clErr); // DTOR releases
                if(clErr != CL_SUCCESS) throw nullptr;
                L = m;
                // cout << "WORKED\n";
              } catch(...) {
                R = m-1;
                // cout << "FAILED\n";
              }
            }
            if(L == R)
              cout << "platform " << P << " device " << D << " max size device buffer " << L  
                   << " BYTES (" << 1e-9 * L << " GB) found by binary search.\n";
            if( L > maxMemAllocSize)
              cout << "platform " << P << " device " << D << " *** DRIVER BROKEN *** - allocated buffer larger than max mem alloc size.\n";
            else if( L > globalMemSize)
              cout << "platform " << P << " device " << D << " *** DRIVER BROKEN *** - allocated buffer size exceeds card memory.\n";

            // how many 1 GB block can be allocated?
            std::vector<cl::Buffer> bufTable;
            for(int i = 0; i < globalMemSize/(1024*1024*1024) + 1; i++) {
              size_t size = 1024 * 1024 * 1024;
              try {
                cl::Buffer buf = cl::Buffer(context, CL_MEM_READ_WRITE, size, host_ptr, &clErr); // DTOR releases
                if(clErr != CL_SUCCESS) throw nullptr;
                bufTable.push_back(move(buf)); // cl::Buffer worked
              } catch(...) { // cl::Buffer failed
                break; // That's it.  We're done.
              }
              // cout << "allocated " << bufTable.size() << " GB of device memory in 1 GB blocks\n";
            }
            cout << "platform " << P << " device " << D << " allocated " << bufTable.size() 
                 << " blocks of 1 GB = " << bufTable.size()*1024*1024*1024 << " BYTES ("
                 << 1e-9*bufTable.size()*1024*1024*1024 << " GB)\n";
            if( bufTable.size() * 1024 * 1024 * 1024 > globalMemSize)
              cout << "platform " << P << " device " << D << " *** DRIVER BROKEN *** - allocated memory exceeds card memory.\n";
            bufTable.clear();

            string ext =  devices[D].getInfo<CL_DEVICE_EXTENSIONS>();
            cout << "platform " << P << " device " << D <<  " extensions:\n";
            printSortedList(ext);
        } // device for loop
        cout << "\n";
    } // platform for loop
}

int main(int argc, char*argv[]) 
{
  try {
    app(argc,argv);
  } catch(...) {
    cerr << "It threw!\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

