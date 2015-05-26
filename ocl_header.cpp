// This is the example ocl code from CL/cl.hpp

// for i in g++-5 g++ clang++ ; do $i -std=c++11 -Ofast -I/opt/AMDAPPSDK-3.0-0-Beta/include/ ocl_header.cpp -lOpenCL -o ocl_header && ./ocl_header ; done 

// NOTE compiler warnings: clCreateSampler + clCreateCommandQueue deprecated
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" // ignore Khronos cl.hpp header file warnings
#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
  #include <OpenCL/cl.hpp>
#else
  #include <CL/cl.hpp>
#endif
#include <iostream>
 
const char * helloStr  = 
R"theKernel(
  __kernel
  void hello(void) { } 
)theKernel";

int main(void)
{
   cl_int err = CL_SUCCESS;
   try {
     std::vector<cl::Platform> platforms;
     cl::Platform::get(&platforms);
     if (platforms.size() == 0) {
         std::cout << " No platforms found. Check OpenCL installation!\n";
         return -1;
     }

     cl_context_properties properties[] = 
        { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
     cl::Context context(CL_DEVICE_TYPE_CPU, properties); 

     std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

     cl::Program::Sources source(1, std::make_pair(helloStr,strlen(helloStr)));
     cl::Program program_ = cl::Program(context, source);
     program_.build(devices);

     cl::Kernel kernel(program_, "hello", &err);

     cl::Event event;
     cl::CommandQueue queue(context, devices[0], 0, &err);
     queue.enqueueNDRangeKernel(
         kernel, 
         cl::NullRange, 
         cl::NDRange(4,4),
         cl::NullRange,
         NULL,
         &event); 

     event.wait();
   } catch (cl::Error err) {
      std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")\n";
   }

  return EXIT_SUCCESS;
}
