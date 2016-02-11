// this  sample code came from the top of CL/cl.hpp

#define __CL_ENABLE_EXCEPTIONS

#if defined(__APPLE__) || defined(__MACOSX)
  #include <OpenCL/cl.hpp>
#else
  #include <CL/cl.hpp>
#endif
#include <cstdio>
#include <cstdlib>
#include <iostream>

 const char * helloStr  = R"kernel(
__kernel void hello(void)
{
  
}
)kernel";

 int main(int argc, char** argv)
 {
    std::cout << argv[0] << "\n";
    cl_int err = CL_SUCCESS;
    try {
 
      std::vector<cl::Platform> platforms;
      cl::Platform::get(&platforms);
      if (platforms.size() == 0) {
          std::cout<<" No platforms found. Check OpenCL installation!\n";
          return -1;
      }
      std::cout << "Platform size " << platforms.size() << "\n";
 
      cl_context_properties properties[] = 
         { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
      cl::Context context(CL_DEVICE_TYPE_GPU, properties); 

      std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

      cl::Program::Sources source(1,
          std::make_pair(helloStr,strlen(helloStr)));
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
    }
    catch (cl::Error err) {
       std::cerr 
          << "ERROR: " << err.what() << "(" << err.err() << ")" << "\n";
    }

   return EXIT_SUCCESS;
 }
