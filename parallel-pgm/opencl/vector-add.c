/*
 * Vector addition in OpenCL:
 * Compute C=A+B using GPU.
 *
 * Original version by Tim Mattson, June 2010
 * Modified by B.L. Massingill, October 2013
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <CL/cl.h>
#include "timer.h"
#include "utility.h"

#define TOL    (0.001)   /* tolerance used in floating point comparisons */
#define LENGTH (1024)    /* length of vectors a, b, and c */

/*
 * kernel for computational device:
 *
 * computes elementwise sum c = a + b
 */
const char *kernel_source = "\n" \
"__kernel void vadd(                                                    \n" \
"   __global float* a,                                                  \n" \
"   __global float* b,                                                  \n" \
"   __global float* c,                                                  \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count)                                                       \n" \
"       c[i] = a[i] + b[i];                                             \n" \
"}                                                                      \n" \
"\n";

/* main program */
int main(int argc, char* argv[]) 
{
    cl_float a_data[LENGTH];
    cl_float b_data[LENGTH];
    cl_float c_res [LENGTH];

    cl_int rval;

    size_t domain_size[1];    /* global domain size */
    size_t workgroup_size[1];

    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
    cl_program program_obj;
    cl_kernel kernel;
    
    cl_mem a_in;     /* device memory for input vector a */
    cl_mem b_in;     /* device memory for input vector b */
    cl_mem c_out;    /* device memory for output vector c */
    
    cl_uint count = LENGTH;

    double start_time, end_time;

    /* fill input vectors with random values */
    for(int i = 0; i < count; ++i){
        a_data[i] = rand() / (cl_float)RAND_MAX;
        b_data[i] = rand() / (cl_float)RAND_MAX;
    }

    start_time = get_time();
    
    cl_platform_id platform;
    rval = clGetPlatformIDs(1, &platform, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not get platform", rval);
    rval = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not get device ID", rval);
    printf("Compute device:\n");
    output_device_info(stdout, device_id, CL_FALSE);
  
    /* create compute context */
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &rval);
    if ((rval != CL_SUCCESS) || (context == NULL))
        error_exit("Could not create compute context", rval);

    /* create command queue */
    commands = clCreateCommandQueue(context, device_id, 0, &rval);
    if ((rval != CL_SUCCESS) || (commands == NULL))
        error_exit("Could not create command queue", rval);

    /* create compute program object from source buffer */
    program_obj = clCreateProgramWithSource(context, 
            1, (const char **) &kernel_source, NULL, &rval);
    if ((rval != CL_SUCCESS) || (program_obj == NULL))
        error_exit("Could not create program object from source", rval);

    /* build program object */
    rval = clBuildProgram(program_obj, 0, NULL, NULL, NULL, NULL);
    if (rval != CL_SUCCESS) {
        size_t len;
        char buffer[2048];

        describe_error("Could not build executable", rval, stderr);
        clGetProgramBuildInfo(program_obj, device_id, CL_PROGRAM_BUILD_LOG, 
                sizeof(buffer), buffer, &len);
        fprintf(stderr, "%s\n", buffer);
        return EXIT_FAILURE;
    }

    /* create compute kernel from program object */
    kernel = clCreateKernel(program_obj, "vadd", &rval);
    if ((rval != CL_SUCCESS) || (kernel  == NULL))
        error_exit("Could not create compute kernel", rval);

    end_time = get_time();
    printf("\nInitialization time %f seconds\n", end_time - start_time);

    start_time = get_time();

    /* create input (a, b) and output (c) arrays in device memory  */
    a_in = clCreateBuffer(context,  CL_MEM_READ_ONLY,  
            sizeof(cl_float) * count, NULL, NULL);
    b_in = clCreateBuffer(context,  CL_MEM_READ_ONLY,  
            sizeof(cl_float) * count, NULL, NULL);
    c_out= clCreateBuffer(context,  CL_MEM_WRITE_ONLY, 
            sizeof(cl_float) * count, NULL, NULL);
    if ((a_in == NULL) || (b_in == NULL) || (c_out== NULL))
        error_exit("Could not allocate device memory", rval);
    
    /* write a and b vectors into compute device memory */
    rval = clEnqueueWriteBuffer(commands, a_in, CL_TRUE, 0, 
            sizeof(cl_float) * count, a_data, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not copy a_data to device memory", rval);
    rval = clEnqueueWriteBuffer(commands, b_in, CL_TRUE, 0, 
            sizeof(cl_float) * count, b_data, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not copy b_data to device memory", rval);

    end_time = get_time();
    printf("Time to copy data %f seconds\n", end_time - start_time);
    
    /* set arguments to compute kernel */
    rval = 0;
    rval  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &a_in);
    rval |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &b_in);
    rval |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &c_out);
    rval |= clSetKernelArg(kernel, 3, sizeof(count), &count);
    if (rval != CL_SUCCESS)
        error_exit("Could not set kernel arguments", rval);

    /* get maximum work group size for executing kernel on device */
    rval = clGetKernelWorkGroupInfo(kernel, device_id, 
            CL_KERNEL_WORK_GROUP_SIZE, 
            sizeof(workgroup_size[0]), workgroup_size, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not retrieve kernel work group info", rval);

    start_time = get_time();
    
    /* execute kernel over entire range of 1d input data set
       using maximum number of work group items for device */
    domain_size[0] = count;
    rval = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, 
            domain_size, workgroup_size, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Failed to execute kernel", rval); 

    /* wait for commands to complete before reading back results */
    clFinish(commands);

    end_time = get_time();
    printf("TIme for kernel computation %f seconds\n", end_time - start_time);

    /* read back results from compute device */
    rval = clEnqueueReadBuffer(commands, c_out, CL_TRUE, 0, 
            sizeof(cl_float) * count, c_res, 0, NULL, NULL);  
    if (rval != CL_SUCCESS)
        error_exit("Could not read output array", rval);
    
    /* check results */
    int correct = 0;
    cl_float diff;
    for(cl_uint i = 0; i < count; ++i)
    {
        diff = a_data[i] + b_data[i];
        diff -= c_res[i];
        if(diff*diff < TOL*TOL)
            ++correct;
        else {
            printf("difference %f in element %d:  a %f b %f c %f\n",
                    diff, i, a_data[i], b_data[i], c_res[i]);
        }
    }
    
    /* summarize results */
    printf("\nC = A+B:  %d out of %d results were correct\n", 
            correct, count);
    
    /* clean up */
    clReleaseMemObject(a_in);
    clReleaseMemObject(b_in);
    clReleaseMemObject(c_out);
    clReleaseProgram(program_obj);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return EXIT_SUCCESS;
}

