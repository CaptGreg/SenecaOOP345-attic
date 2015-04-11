// g++ -std=c++11 cl-offset.cpp -o cl-offset -lOpenCL ; ./cl-offset -c ; ./cl-offset -g ; rm cl-offset

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <iostream>

using namespace std;

const char * clSrc  = R"kernel(
__kernel void hello(__global float* a, __global float* b, __global float* c)
{
  size_t i = get_global_id(0);
  c[i] = a[i] + b[i];
}
)kernel";

int main(int argc, char **argv)
{
   char                        *kernelName;
   std::vector<cl::Platform>   platforms;
   std::vector<cl::Device>     devices;
   cl::Program                 program_;
   cl_int                      offset = 0;

   kernelName = (char *) "hello";
   cl_device_type deviceType = CL_DEVICE_TYPE_CPU;

   for(int i = 1; i < argc; i++) {
       if(*argv[i] == '-') {
            switch(*(argv[i]+1)) { 
            case 'c': deviceType = CL_DEVICE_TYPE_CPU; break;
            case 'g': deviceType = CL_DEVICE_TYPE_GPU; break;
            case 'o': if( *(argv[i]+2) )  { offset = atoi( argv[i]+2 ); } 
                      else if(i+1 < argc) { offset = atoi( argv[++i] ); }
                      break;
            }
        }
   }
   cl_int err = CL_SUCCESS;
   try {

     cl::Platform::get(&platforms);
     if (platforms.size() == 0) {
         std::cout << "Platform size 0\n";
         return -1;
     }

     int count = 0;
     const char *want    =               "AMD Accelerated Parallel Processing";
     int found = -1;
     std::cout << "Number of platforms:\t " << platforms.size() << std::endl;
     for (std::vector<cl::Platform>::iterator i = platforms.begin(); i != platforms.end(); ++i) {
          // pick a platform and do something
           if(strcmp(want, (*i).getInfo<CL_PLATFORM_NAME>().c_str() ) == 0) 
               found = count;
           count++;
     }     

     cl_context_properties properties[] = 
        { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[found])(), 0};
     cl::Context context(deviceType, properties); 
 
     devices = context.getInfo<CL_CONTEXT_DEVICES>();
 
     cl::Program::Sources source(1, std::make_pair(clSrc, strlen(clSrc)));
     program_ = cl::Program(context, source);
     const char *options = "-I .";
     program_.build(devices, options);
 
     cl::Kernel kernel(program_, kernelName, &err);
 
     cl::CommandQueue queue(context, devices[0], 0, &err);

     const int SIZE = 64*1024-1;

     // float a[SIZE], b[SIZE], c[SIZE];
     float a[SIZE], b[SIZE], c[SIZE]; // destroy any accidental 128 bit alignment

     for(int i = 0; i < SIZE; i++) {
         a[i] = i;
         b[i] = i / 2.;
     }

     cl::Buffer aBuf(context, CL_MEM_READ_ONLY,  SIZE * sizeof(a[0]));
     cl::Buffer bBuf(context, CL_MEM_READ_ONLY,  SIZE * sizeof(b[0]));
     cl::Buffer cBuf(context, CL_MEM_WRITE_ONLY, SIZE * sizeof(c[0]));

     queue.enqueueWriteBuffer(aBuf, CL_TRUE, 0, SIZE * sizeof(a[0]), a);
     queue.enqueueWriteBuffer(bBuf, CL_TRUE, 0, SIZE * sizeof(b[0]), b);


     kernel.setArg(0, aBuf);
     kernel.setArg(1, bBuf);
     kernel.setArg(2, cBuf);

     if(offset > SIZE-1) offset = SIZE - 1;
     queue.enqueueNDRangeKernel(
         kernel, 
         offset ?                       // offset
             cl::NDRange(offset) :
             cl::NullRange,
         cl::NDRange(SIZE-offset),      // global
         cl::NullRange                  // local
     );

     queue.enqueueReadBuffer(cBuf, CL_TRUE, 0, SIZE * sizeof(c[0]), (void *) c);

     for(int i = 0; i < SIZE; i += SIZE/16) {
         std::cout << i << " " << c[i] << std::endl;
     }
     cout << "C++ will now call the out-of-scope destructors at the bottom of the 'try {} catch{}' block\n";
   } catch (cl::Error e) {
      std::cerr << "ERROR: " << e.what() << "(" << e.err() << ")" << std::endl;
      if( CL_BUILD_PROGRAM_FAILURE == e.err() )
      {
         std::cout << "Build log ouput:\n";
         std::cout << program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]); 
      }
   } catch (const char *e) {
      std::cout << "error = " << e << "\n";
   }
   cout << "C++ invoked destructors without incident\n";
 
  return EXIT_SUCCESS;
}
