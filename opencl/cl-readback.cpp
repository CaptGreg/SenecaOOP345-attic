// WIN7, I7 980, HD 6970
//
// MAP: In host queue: 11674ns, In device queue: 10234854ns, Execution time: 111ns, UNMAP: In host queue: 54681ns, In device queue: 542124ns, Execution time: 1240555ns
// MAP: In host queue: 48230ns, In device queue: 147652ns, Execution time: 111ns, UNMAP: In host queue: 48845ns, In device queue: 429269ns, Execution time: 1251112ns
// MAP: In host queue: 4915ns, In device queue: 386653ns, Execution time: 111ns, UNMAP: In host queue: 7373ns, In device queue: 906374ns, Execution time: 964778ns
// MAP: In host queue: 34099ns, In device queue: 33792ns, Execution time: 0ns, UNMAP: In host queue: 4916ns, In device queue: 2485144ns, Execution time: 962555ns
// MAP: In host queue: 31641ns, In device queue: 112435ns, Execution time: 0ns, UNMAP: In host queue: 4916ns, In device queue: 2348453ns, Execution time: 954555ns
// MAP: In host queue: 69734ns, In device queue: 35328ns, Execution time: 0ns, UNMAP: In host queue: 5530ns, In device queue: 2412088ns, Execution time: 947445ns
// MAP: In host queue: 34713ns, In device queue: 32871ns, Execution time: 0ns, UNMAP: In host queue: 6144ns, In device queue: 2433719ns, Execution time: 1002000ns
// MAP: In host queue: 31028ns, In device queue: 35327ns, Execution time: 0ns, UNMAP: In host queue: 5222ns, In device queue: 2433568ns, Execution time: 963444ns
// MAP: In host queue: 34713ns, In device queue: 33178ns, Execution time: 0ns, UNMAP: In host queue: 5529ns, In device queue: 2435332ns, Execution time: 954000ns
// MAP: In host queue: 35942ns, In device queue: 32256ns, Execution time: 0ns, UNMAP: In host queue: 5529ns, In device queue: 2426576ns, Execution time: 1039556ns



#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <iostream>

using namespace std;

#define NO_PRINT
// #define USE_PRINT

const char * clSrcNoPrint  = R"kernel(
   __kernel void hello(__global float* a, __global float* b, __global float* c)
   {
      size_t i = get_global_id(0);
      c[i] = a[i] + b[i];
   }
)kernel";

const char * clSrcPrint  = R"kernel(
   #pragma OPENCL EXTENSION cl_amd_printf : enable
   __kernel void hello(__global int printIndex[4], __global float* a, __global float* b, __global float* c)
   {
      size_t i = get_global_id(0);
      if(i == printIndex[0])
      {
           printf(\"\\n\"); // see OpenCl Prog Guide, table 6.11.11, pg 557
           printf(\"cl get_work_dim()       = %d \\n\", get_work_dim());
           printf(\"cl get_global_size(0)   = %d \\n\", get_global_size(0));
           printf(\"cl get_global_id(0)     = %d \\n\", get_global_id(0));
           printf(\"cl get_local_size(0)    = %d \\n\", get_local_size(0));
           printf(\"cl get_local_id(0)      = %d \\n\", get_local_id(0));
           printf(\"cl get_num_groups(0)    = %d \\n\", get_num_groups(0));
           printf(\"cl get_group_id(0)      = %d \\n\", get_group_id(0));
           printf(\"cl get_global_offset(0) = %d \\n\", get_global_offset(0));
           printf(\"\\n\");
      }
      c[i] = a[i] + b[i];
   }
)kernel";


// const int ARRAY_SIZE = 1 << 23 - 1;
// const int ARRAY_SIZE = 0xFFFFF;

// cl_float *a, *b, *c;

// const int ARRAY_SIZE = 16*1024*1024;
// const int ARRAY_SIZE = 8*1024*1024;
// const int ARRAY_SIZE = 4*1024*1024;
// const int ARRAY_SIZE = 2*1024*1024;
// const int ARRAY_SIZE = 1*1024*1024;
// const int ARRAY_SIZE = 512*1024;  // segfault calling destructors
// const int ARRAY_SIZE = 128*1024;  // works on gpu, segfaults on cpu
// const int ARRAY_SIZE = 64*1024;   // works on gpu, segfaults on cpu
// const int ARRAY_SIZE = 32*1024;   // works on gpu, segfaults on cpu
// const int ARRAY_SIZE = 64*1024-1;    // works on gpu, segfaults on cpu
const int ARRAY_SIZE = 32*1024*1024;

// float a[ARRAY_SIZE], abc, b[ARRAY_SIZE], c[ARRAY_SIZE];
 
#ifdef __GNUC__
// stack call-back trace
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>       // exit
#include <sys/ucontext.h>

/* get REG_EIP from ucontext.h */
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <ucontext.h>

#ifndef REG_EIP
#define REG_EIP 14  // see ucontext.h and count!
#endif

void bt_sighandler(int sig, siginfo_t *info, void *secret)  // stack backtrace signal handler
{

  #define TRACE_DEPTH 20
  void *trace[TRACE_DEPTH];
  char **messages = (char **)NULL;
  int i, trace_size = 0;
  ucontext_t *uc = (ucontext_t *)secret;

  /* Do something useful with siginfo_t */
  cout << "+++++++++++++++++++++++++++++++++++++++++++++++\n";
  if (sig == SIGSEGV) {
    cout << "caught signal " << sig << ", faulty address is " <<  (void*) info->si_addr << ", from " << (void*) uc->uc_mcontext.gregs[REG_EIP] <<  "\n";
    // printf("caught signal %d, faulty address is %p, from %p\n", sig, (void*) info->si_addr, (void*) uc->uc_mcontext.gregs[REG_EIP]);
  } else {
    cout << "caught signal " << sig <<  "\n";
    // printf("caught signal %d\n", sig);
  }
    
  trace_size = backtrace(trace, TRACE_DEPTH);
  /* overwrite sigaction with caller's address */
  trace[1] = (void *) uc->uc_mcontext.gregs[REG_EIP];

  messages = backtrace_symbols(trace, trace_size);
  /* skip first stack frame (points here) */
  // printf("backtrace [bt] Execution path:\n");
  cout << "backtrace [bt] Execution path:\n";
  for (i=1; i<trace_size; ++i) {
    cout << "[bt] " <<  messages[i] << endl;
    // printf("[bt] %s\n", messages[i]);
  }

  exit(0);
}
#endif

int main(int argc, char **argv)
{
#ifdef __GNUC__
  /* Install our signal handler */
  struct sigaction sa;
  sa.sa_sigaction = bt_sighandler;
  sigemptyset (&sa.sa_mask);
  sa.sa_flags = SA_RESTART | SA_SIGINFO;

  sigaction(SIGSEGV, &sa, NULL);
  sigaction(SIGUSR1, &sa, NULL);
  /* ... add any other signal here */
#endif

   char                        *kernelName;
   vector<cl::Platform>   platforms;
   vector<cl::Device>     devices;
   cl::Program                 program;
   cl_int                      offset = 0;
   cl_int                      work_unit_divisor = 1;
#ifdef USE_PRINT
   int                         printIndex[4]; // 128 bit var for xfer to DEVICE
#endif
   float                       *a;
   float                       *b;
   float                       *c;

#ifdef __GNUC__
// #define NEW_DELETE
#define POSIX_MEMALIGN
#else
#define NEW_DELETE
#endif

#ifdef NEW_DELETE
   cout << "allocating buffers with new\n";
   a = new float [ARRAY_SIZE];
   b = new float [ARRAY_SIZE];
   c = new float [ARRAY_SIZE];
#endif
 
#ifdef POSIX_MEMALIGN
   cout << "allocating 128-bit/16-byte aligned buffers with posix_memaligned\n";
   posix_memalign( (void**) &a , 128/8, sizeof( *a ) * ARRAY_SIZE );
   posix_memalign( (void**) &b , 128/8, sizeof( *b ) * ARRAY_SIZE );
   posix_memalign( (void**) &c , 128/8, sizeof( *c ) * ARRAY_SIZE );
#endif

   cl_int                      mySize = ARRAY_SIZE;


   kernelName = (char *) "hello";
   cl_device_type deviceType = CL_DEVICE_TYPE_CPU;
#ifdef USE_PRINT
   printIndex[0] = 0;
#endif

   for(int i = 1; i < argc; i++) {
       if(*argv[i] == '-') {
            switch(*(argv[i]+1)) { 
            case 'c': deviceType = CL_DEVICE_TYPE_CPU; break;
            case 'g': deviceType = CL_DEVICE_TYPE_GPU; break;
            case 'o': if( *(argv[i]+2)  ) { offset = atoi( argv[i]+2 ); } 
                      else if(i+1 < argc) { offset = atoi( argv[++i] ); }
                      break;
#ifdef USE_PRINT
            case 'p': if( *(argv[i]+2)  ) { printIndex[0] = atoi( argv[i]+2 ); } 
                      else if(i+1 < argc) { printIndex[0] = atoi( argv[++i] ); }
                      break;
#endif
            case 's': if( *(argv[i]+2)  ) { mySize = atoi( argv[i]+2 ); } 
                      else if(i+1 < argc) { mySize = atoi( argv[++i] ); }
                      if(mySize>ARRAY_SIZE) mySize=ARRAY_SIZE;
                      break;
            case 'w': if( *(argv[i]+2)  ) { work_unit_divisor = atoi( argv[i]+2 ); }  // TODO: finish
                      else if(i+1 < argc) { work_unit_divisor = atoi( argv[++i] ); }
                      break;
 
            }
        }
   }
   cl_int err = CL_SUCCESS;
   try {

     cl::Platform::get(&platforms);
     if (platforms.size() == 0) {
         cout << "Platform size 0\n";
         return -1;
     }

     int count = 0;
     const char *want    =               "AMD Accelerated Parallel Processing";
     int found = -1;
     cout << endl;
     cout << "Number of platforms:\t " << platforms.size() << endl;
     for (vector<cl::Platform>::iterator i = platforms.begin(); i != platforms.end(); ++i) {
                cout << "CL Platform number " << count << endl;
                cout << "CL_PLATFORM_PROFILE    = " << (*i).getInfo<CL_PLATFORM_PROFILE>()    << endl;
                cout << "CL_PLATFORM_VERSION    = " << (*i).getInfo<CL_PLATFORM_VERSION>()    << endl;
                cout << "CL_PLATFORM_NAME       = " << (*i).getInfo<CL_PLATFORM_NAME>()       << endl;
                cout << "CL_PLATFORM_VENDOR     = " << (*i).getInfo<CL_PLATFORM_VENDOR>()     << endl;
                cout << "CL_PLATFORM_EXTENSIONS = " << (*i).getInfo<CL_PLATFORM_EXTENSIONS>() << endl;

          // pick a platform and do something
           if(strcmp(want, (*i).getInfo<CL_PLATFORM_NAME>().c_str() ) == 0) 
               found = count;
           count++;
     }     
     cout << endl;

     cl_context_properties properties[] = 
        { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[found])(), 0};
     cl::Context context(deviceType, properties); 
 
     cout << endl;
     cout << "devices\n";
     devices = context.getInfo<CL_CONTEXT_DEVICES>();
 
    cout << "Number of CL_DEVICE_TYPE " << deviceType << (deviceType == CL_DEVICE_TYPE_GPU? " (GPU)": " (CPU)") << " devices  : " << devices.size() << endl;
    cl_int  clDeviceMaxWorkGroupSize;

    count = 0;
    for (VECTOR_CLASS<cl::Device>::iterator it = devices.begin(); it != devices.end(); ++it)
    {
        cout << "device " << count << ":"                                                                                << endl;
        cout << "CL_DEVICE_VENDOR                        = " << (*it).getInfo<CL_DEVICE_VENDOR>()                        << endl;
        cout << "CL_DEVICE_VENDOR_ID                     = " << (*it).getInfo<CL_DEVICE_VENDOR_ID>()                     << endl;
        cout << "CL_DEVICE_NAME                          = " << (*it).getInfo<CL_DEVICE_NAME>()                          << endl;
        cout << "CL_DEVICE_PLATFORM                      = " << (*it).getInfo<CL_DEVICE_PLATFORM>()                      << endl;
        cout << "CL_DEVICE_VERSION                       = " << (*it).getInfo<CL_DEVICE_VERSION>()                       << endl;
        // cout << "CL_DEVICE_OPENCL_C_VERSION              = " << (*it).getInfo<CL_DEVICE_OPENCL_C_VERSION>().c_str()      << endl; // not defined in cl.hpp
        cout << "CL_DRIVER_VERSION                       = " << (*it).getInfo<CL_DRIVER_VERSION>()                       << endl;
        cout << "CL_DEVICE_PROFILE                       = " << (*it).getInfo<CL_DEVICE_PROFILE>().c_str()               << endl;
        cout << "CL_DEVICE_TYPE                          = " << (*it).getInfo<CL_DEVICE_TYPE>()                          << endl;

        cout << "CL_DEVICE_EXTENSIONS                    = " << (*it).getInfo<CL_DEVICE_EXTENSIONS>().c_str()            << endl;
        cout << "CL_DEVICE_ADDRESS_BITS                  = " << (*it).getInfo<CL_DEVICE_ADDRESS_BITS>()                  << endl;
        cout << "CL_DEVICE_AVAILABLE                     = " << (*it).getInfo<CL_DEVICE_AVAILABLE>()                     << endl;
        cout << "CL_DEVICE_COMPILER_AVAILABLE            = " << (*it).getInfo<CL_DEVICE_COMPILER_AVAILABLE>()            << endl;
        cout << "CL_DEVICE_ENDIAN_LITTLE                 = " << (*it).getInfo<CL_DEVICE_ENDIAN_LITTLE>()                 << endl;
        cout << "CL_DEVICE_ERROR_CORRECTION_SUPPORT      = " << (*it).getInfo<CL_DEVICE_ERROR_CORRECTION_SUPPORT>()      << endl;
        cout << "CL_DEVICE_EXECUTION_CAPABILITIES        = " << (*it).getInfo<CL_DEVICE_EXECUTION_CAPABILITIES>()        << endl;
        cout << "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE         = " << (*it).getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_SIZE>()         << endl;
        cout << "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE         = " << (*it).getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_TYPE>()         << endl;
        cout << "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE     = " << (*it).getInfo<CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE>()     << endl;
        cout << "CL_DEVICE_GLOBAL_MEM_SIZE               = " << (*it).getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>()               << endl;
        cout << "CL_DEVICE_IMAGE_SUPPORT                 = " << (*it).getInfo<CL_DEVICE_IMAGE_SUPPORT>()                 << endl;
        cout << "CL_DEVICE_IMAGE2D_MAX_HEIGHT            = " << (*it).getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT>()            << endl;
        cout << "CL_DEVICE_IMAGE2D_MAX_WIDTH             = " << (*it).getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH>()             << endl;
        cout << "CL_DEVICE_IMAGE3D_MAX_DEPTH             = " << (*it).getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH>()             << endl;
        cout << "CL_DEVICE_IMAGE3D_MAX_HEIGHT            = " << (*it).getInfo<CL_DEVICE_IMAGE3D_MAX_HEIGHT>()            << endl;
        cout << "CL_DEVICE_IMAGE3D_MAX_WIDTH             = " << (*it).getInfo<CL_DEVICE_IMAGE3D_MAX_WIDTH>()             << endl;
        cout << "CL_DEVICE_LOCAL_MEM_SIZE                = " << (*it).getInfo<CL_DEVICE_LOCAL_MEM_SIZE>()                << endl;
        cout << "CL_DEVICE_LOCAL_MEM_TYPE                = " << (*it).getInfo<CL_DEVICE_LOCAL_MEM_TYPE>()                << endl;
        cout << "CL_DEVICE_MAX_CLOCK_FREQUENCY           = " << (*it).getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << " MHz" << endl;
        cout << "CL_DEVICE_MAX_COMPUTE_UNITS             = " << (*it).getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>()             << endl;

        cout << "CL_DEVICE_MAX_CONSTANT_ARGS             = " << (*it).getInfo<CL_DEVICE_MAX_CONSTANT_ARGS>()             << endl;
        cout << "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE      = " << (*it).getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>()      << endl;
        cout << "CL_DEVICE_MAX_MEM_ALLOC_SIZE            = " << (*it).getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>()            << endl;
        cout << "CL_DEVICE_MAX_PARAMETER_SIZE            = " << (*it).getInfo<CL_DEVICE_MAX_PARAMETER_SIZE>()            << endl;
        cout << "CL_DEVICE_MAX_READ_IMAGE_ARGS           = " << (*it).getInfo<CL_DEVICE_MAX_READ_IMAGE_ARGS>()           << endl;
        cout << "CL_DEVICE_MAX_SAMPLERS                  = " << (*it).getInfo<CL_DEVICE_MAX_SAMPLERS>()                  << endl;
        cout << "CL_DEVICE_MAX_WORK_GROUP_SIZE           = " << (*it).getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << "*****"<< endl;
        clDeviceMaxWorkGroupSize           = (*it).getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
        cout << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS      = " << (*it).getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>()      << endl;
        cout << "CL_DEVICE_MAX_WRITE_IMAGE_ARGS          = " << (*it).getInfo<CL_DEVICE_MAX_WRITE_IMAGE_ARGS>()          << endl;
        cout << "CL_DEVICE_MEM_BASE_ADDR_ALIGN           = " << (*it).getInfo<CL_DEVICE_MEM_BASE_ADDR_ALIGN>()           << endl;
        cout << "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE      = " << (*it).getInfo<CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE>()      << endl;
        cout << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR   = " << (*it).getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR>()   << endl;
        cout << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE = " << (*it).getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE>() << endl;
        cout << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT  = " << (*it).getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT>()  << endl;
        cout << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT    = " << (*it).getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT>()    << endl;
        cout << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG   = " << (*it).getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG>()   << endl;
        cout << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT  = " << (*it).getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT>()  << endl;
        cout << "CL_DEVICE_PROFILING_TIMER_RESOLUTION    = " << (*it).getInfo<CL_DEVICE_PROFILING_TIMER_RESOLUTION>() << " nsec" << endl;
        cout << "CL_DEVICE_QUEUE_PROPERTIES              = " << (*it).getInfo<CL_DEVICE_QUEUE_PROPERTIES>()              << endl;
        cout << "CL_DEVICE_SINGLE_FP_CONFIG              = " << (*it).getInfo<CL_DEVICE_SINGLE_FP_CONFIG>()              << endl;

        count++;
    }
    cout << endl;
 
#ifdef NO_PRINT
    cl::Program::Sources source(1, make_pair(clSrcNoPrint, strlen(clSrcNoPrint)));
#endif
#ifdef USE_PRINT
    cl::Program::Sources source(1, make_pair(clSrcPrint, strlen(clSrcPrint)));
#endif
    program = cl::Program(context, source);
    const char *options = "-I .";
    program.build(devices, options);

    cl::Kernel kernel(program, kernelName, &err);

    cout << endl;
    //see what we can learn about the compiled kernel code
    cout << "CL_KERNEL_FUNCTION_NAME                      = " << kernel.getInfo<CL_KERNEL_FUNCTION_NAME>() << endl;
    cout << "CL_KERNEL_NUM_ARGS                           = " << kernel.getInfo<CL_KERNEL_NUM_ARGS>()      << endl;
    size_t size; cl_long l;
    kernel.getWorkGroupInfo( devices[0], CL_KERNEL_WORK_GROUP_SIZE, &size );
    cout << "CL_KERNEL_WORK_GROUP_SIZE                    = " << size      << endl;
    // kernel.getWorkGroupInfo( devices[0], CL_KERNEL_COMPILE_WORK_GROUP_SIZE, sizes ); // run time error: bad parameter
    // cout << "CL_KERNEL_COMPILE_WORK_GROUP_SIZE      = " << sizes[0] << " " << sizes[1] << " " << sizes[2]      << endl;
    kernel.getWorkGroupInfo( devices[0], CL_KERNEL_LOCAL_MEM_SIZE, &l );
    cout << "CL_KERNEL_LOCAL_MEM_SIZE                     = " << l << " bytes per work item"   << endl;
    kernel.getWorkGroupInfo( devices[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, &l ); // not defined
    cout << "CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE = " << l      << endl;
    kernel.getWorkGroupInfo( devices[0], CL_KERNEL_PRIVATE_MEM_SIZE, &l );
    cout << "CL_KERNEL_PRIVATE_MEM_SIZE                   = " << l << " bytes per work item"     << endl;
    cout << endl;


 
    cl::CommandQueue queue(context, devices[0], 0, &err);


    cout << "host buffer addresses: a=" << a << " b=" << b << " c=" << c << endl;

    for(int i = 0; i < mySize; i++) {
        a[i] = i;
        b[i] = i / 2.;
    }

#ifdef USE_PRINT
    cout << "ro printIndexBuf\n"; cl::Buffer printIndexBuf(context, CL_MEM_READ_ONLY,  sizeof(printIndex));
#endif
    cout << "ro aBuf\n";          cl::Buffer          aBuf(context, CL_MEM_READ_ONLY,  mySize * sizeof(a[0]));
    cout << "ro bBuf\n";          cl::Buffer          bBuf(context, CL_MEM_READ_ONLY,  mySize * sizeof(b[0]));
    cout << "wo cBuf\n";          cl::Buffer          cBuf(context, CL_MEM_WRITE_ONLY, mySize * sizeof(c[0]));

#ifdef USE_PRINT
    cout << "host printIndex to device\n";
    queue.enqueueWriteBuffer(printIndexBuf, CL_TRUE, 0, sizeof(printIndex), &printIndex);
#endif
    cout << "host a to device\n";
    queue.enqueueWriteBuffer(aBuf, CL_TRUE, 0, mySize * sizeof(a[0]), a);
    cout << "host b to device\n";
    queue.enqueueWriteBuffer(bBuf, CL_TRUE, 0, mySize * sizeof(b[0]), b);

    cout << "assigning kernel args to buffers\n";
    int arg = 0;
#ifdef USE_PRINT
    cout << "arg " << arg << "\n"; kernel.setArg(arg++, printIndexBuf);
#endif
    cout << "arg " << arg << "\n"; kernel.setArg(arg++, aBuf);
    cout << "arg " << arg << "\n"; kernel.setArg(arg++, bBuf);
    cout << "arg " << arg << "\n"; kernel.setArg(arg++, cBuf);
    cout << "kernel args assigned\n";

    if(offset > mySize-1) offset = mySize - 1;
    cout << "running kernel, offset=" << offset <<", global count=" << mySize-offset << endl;
    queue.enqueueNDRangeKernel(
        kernel, 
        offset ?                       // offset
            cl::NDRange(offset) :
            cl::NullRange,
        cl::NDRange(mySize-offset),    // global
        cl::NullRange                  // local
    );

    cout << "device c to host\n";
    queue.enqueueReadBuffer(cBuf, CL_TRUE, 0, mySize * sizeof(c[0]), (void *) c);

    cout << "ARRAY_SIZE=" << mySize << endl;

    for(int i = 0; i < mySize; i += mySize/16) {
         cout << "cl RESULTS read back i=" << i << " " << "cl val=" << c[i] << " err=" << 1.5*i - c[i] << endl;
    }

    cout << "deleting a,b,c host buffers\n";
#ifdef NEW_DELETE
    cout << "deleting buffers allocated with new\n";
    delete [] a; // float *a = new float[ARRAY_SIZE];
    delete [] b; // float *b = new float[ARRAY_SIZE];
    delete [] c; // float *c = new float[ARRAY_SIZE];
#endif

#ifdef POSIX_MEMALIGN
    cout << "freeing 128-bit/16-byte aligned posix_memaligned buffers\n";
    free(a); // posix_memalign( (void**) &a , 128/8, sizeof( *a ) * ARRAY_SIZE );
    free(b); // posix_memalign( (void**) &b , 128/8, sizeof( *b ) * ARRAY_SIZE );
    free(c); // posix_memalign( (void**) &c , 128/8, sizeof( *c ) * ARRAY_SIZE );
#endif

    cout << endl;

     cout << "C++ will now call the out-of-scope destructors at the bottom of the 'try {} catch{}' block\n";  
   } catch (cl::Error err) {
      cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << endl;
      if( CL_BUILD_PROGRAM_FAILURE == err.err() )
      {
         cout << "Build log ouput:\n";
         cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]); 
      }
   } catch (const char *errMsg) {
      cout << "error = " << errMsg << "\n";
   }
    cout << "C++ invoked destructors without incident\n";
 
  return EXIT_SUCCESS;
}
