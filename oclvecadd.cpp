// https://www.olcf.ornl.gov/tutorials/opencl-vector-addition/
// GB Hacked for c++11, unique_ptr
// for i in g++-5 g++ clang++ ; do $i -std=c++11 -Ofast  -I /opt/AMDAPPSDK-3.0-0-Beta/include/  oclvecadd.cpp -lOpenCL -o oclvecadd -pthread && ./oclvecadd; done

#define __CL_ENABLE_EXCEPTIONS
#define NULL nullptr // try to eliminate some <CL/cl.hpp> warnings w.r.t. NULL
#include <CL/cl.hpp>
#include <cstdlib>
#include <iostream>
#include <cmath>          // sinf/cosf
#include <exception>      // std::bsd_alloc
#include <memory>         // unique_ptr
 
// OpenCL kernel. Each work item takes care of one element of c
const char *kernelSource = R"kernel(
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void vecAdd(  __global double *a,
                       __global double *b,
                       __global double *c,
                       const unsigned int n)
{
    //Get our global thread ID
    int id = get_global_id(0);

    //Make sure we do not go out of bounds
    if (id < n)
        c[id] = a[id] + b[id];
}
)kernel";
 
int main(int argc, char *argv[])
{
 
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
    try {
	// Length of vectors
	unsigned int n = int(1e7);
	// Allocate memory for each vector on host
	// Host input vectors
	// std::unique_ptr<double> h_a(new double[n]);
	// std::unique_ptr<double> h_b(new double[n]);
	double* h_a = new double[n];
	double* h_b = new double[n];
	// Allocate memory for Host output vector
	// std::unique_ptr<double> h_c(new double[n]);
	double* h_c = new double[n];

 
        // Device input buffers
        cl::Buffer d_a;
        cl::Buffer d_b;
        // Device output buffer
        cl::Buffer d_c;
 
        // Size, in bytes, of each vector
        size_t bytes = n*sizeof(double);
 
 
        // Initialize vectors on host
        for(int i = 0; i < n; i++ )
        {
            h_a[i] = sinf(i)*sinf(i);
            h_b[i] = cosf(i)*cosf(i);
        }
     
        cl_int err = CL_SUCCESS;
 
        // Query platforms
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        if (platforms.size() == 0) {
            std::cout << "Platform size 0\n";
            return -1;
         }
 
        // Get list of devices on default platform and create context
        cl_context_properties properties[] =
           { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
        cl::Context context(CL_DEVICE_TYPE_GPU, properties);
        std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
 
        // Create command queue for first device
        cl::CommandQueue queue(context, devices[0], 0, &err);
 
        // Create device memory buffers
        d_a = cl::Buffer(context, CL_MEM_READ_ONLY, bytes);
        d_b = cl::Buffer(context, CL_MEM_READ_ONLY, bytes);
        d_c = cl::Buffer(context, CL_MEM_WRITE_ONLY, bytes);
 
        // Bind memory buffers
        // queue.enqueueWriteBuffer(d_a, CL_TRUE, 0, bytes, h_a.get());
        // queue.enqueueWriteBuffer(d_b, CL_TRUE, 0, bytes, h_b.get());
        queue.enqueueWriteBuffer(d_a, CL_TRUE, 0, bytes, h_a);
        queue.enqueueWriteBuffer(d_b, CL_TRUE, 0, bytes, h_b);

        // all done with input buffers
        // delete [] h_a.release();
        // delete [] h_b.release();
        delete [] h_a;
        delete [] h_b;
 
        //Build kernel from source string
        cl::Program::Sources source(1,
            std::make_pair(kernelSource,strlen(kernelSource)));
        cl::Program program_ = cl::Program(context, source);
        program_.build(devices);
 
        // Create kernel object
        cl::Kernel kernel(program_, "vecAdd", &err);
 
        // Bind kernel arguments to kernel
        kernel.setArg(0, d_a);
        kernel.setArg(1, d_b);
        kernel.setArg(2, d_c);
        kernel.setArg(3, n);
 
        // Number of work items in each local work group
        cl::NDRange localSize(64);
        // Number of total work items - localSize must be devisor
        cl::NDRange globalSize((int)(ceil(n/(float)64)*64));
 
        // Enqueue kernel
        cl::Event event;
        queue.enqueueNDRangeKernel(
            kernel,
            cl::NullRange,
            globalSize,
            localSize,
            NULL,
            &event);
 
        // Block until kernel completion
        event.wait();
 
        // Read back d_c
        // queue.enqueueReadBuffer(d_c, CL_TRUE, 0, bytes, h_c.get());
        queue.enqueueReadBuffer(d_c, CL_TRUE, 0, bytes, h_c);
 
	// Sum up vector c and print result divided by n, this should equal 1 within error
	double sum = 0.0; for(int i=0; i<n; i++) sum += h_c[i];
	std::cout<<"final result: "<<sum/n<<"\n";

	// all done with output buffer
	// delete [] h_c.release();
        delete [] h_c;
    } catch (cl::Error& err) {
         std::cerr << "ERROR: "<<err.what()<<"("<<err.err()<<")"<<"\n";
         return -1;
    } catch(std::exception& e) {
         std::cerr << "std::exception: " << e.what() << "\n";
         return -1;
    }

    return 0;
}
