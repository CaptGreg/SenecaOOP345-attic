 #define __CL_ENABLE_EXCEPTIONS
 #define __NO_STD_VECTOR
 #define __NO_STD_STRING
 
 #include <CL/cl.hpp>
 #include <cstdio>
 #include <cstdlib>
 #include <iostream>
 
const char * helloStr  = "__kernel void hello(void) {   } ";
 
int main(int argc, char **argv)
{
     cl_int err = CL_SUCCESS;
     try {
        cl::Context context(CL_DEVICE_TYPE_CPU, 0, NULL, NULL, &err); 

        cl::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

        cl::Program::Sources source(1, std::make_pair(helloStr,strlen(helloStr)));

        cl::Program program = cl::Program(context, source);

        program.build(devices);

        cl::Kernel kernel(program, "hello", &err);

        cl::CommandQueue cmdQ(context, devices[0], 0, &err);

        cl::Event event;
        cmdQ.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(4,4), cl::NDRange(2,2), NULL, &event);
        event.wait();
    }
     catch (cl::Error err) {
        std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
     }
 
    return EXIT_SUCCESS;
}
