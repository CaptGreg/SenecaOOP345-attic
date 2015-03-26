/*
 * Vector addition in OpenCL:
 * Compute C=A+B using GPU.
 *
 * (This version is more complicated than it needs to be, to show use
 * of events.  Actually computes 2C=(A+A)+(B+B).)
 *
 * Original version by Tim Mattson, June 2010
 * Modified by B.L. Massingill, October 2013
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    cl_kernel kernel1;  /* kernel to compute t1 = a+a */
    cl_kernel kernel2;  /* kernel to compute t2 = b+b */
    cl_kernel kernel3;  /* kernel to compute c = t1+t2 */
    
    cl_mem a_in;     /* device memory for input vector a */
    cl_mem b_in;     /* device memory for input vector b */
    cl_mem c_out;    /* device memory for output vector c */
    cl_mem t1;       /* device memory for intermediate output vector t1 */
    cl_mem t2;       /* device memory for intermediate output vector t2 */

    cl_event events[2]; /* events marking completion of kernel1, kernel2 */
    
    cl_uint count = LENGTH;

    /* fill input vectors with random values */
    for(cl_uint i = 0; i < count; ++i){
        a_data[i] = rand() / (cl_float)RAND_MAX;
        b_data[i] = rand() / (cl_float)RAND_MAX;
    }

    double start_init = get_time();

    /* create context, command queue, program object */
    const char* kernel_sources[] = { kernel_source };
    initialize(CL_DEVICE_TYPE_GPU,
            &device_id, &context, &commands, &program_obj, 1, kernel_sources);
    printf("Compute device:\n");
    output_device_info(stdout, device_id, CL_FALSE);
 
    /* create compute kernels from program object */
    kernel1 = clCreateKernel(program_obj, "vadd", &rval);
    if ((rval != CL_SUCCESS) || (kernel1 == NULL))
        error_exit("Could not create compute kernel", rval);
    kernel2 = clCreateKernel(program_obj, "vadd", &rval);
    if ((rval != CL_SUCCESS) || (kernel2 == NULL))
        error_exit("Could not create compute kernel", rval);
    kernel3 = clCreateKernel(program_obj, "vadd", &rval);
    if ((rval != CL_SUCCESS) || (kernel3 == NULL))
        error_exit("Could not create compute kernel", rval);

    /* create input and output arrays in device memory  */
    a_in = clCreateBuffer(context, CL_MEM_READ_ONLY,  
            sizeof(cl_float) * count, NULL, NULL);
    b_in = clCreateBuffer(context, CL_MEM_READ_ONLY,  
            sizeof(cl_float) * count, NULL, NULL);
    c_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            sizeof(cl_float) * count, NULL, NULL);
    t1 = clCreateBuffer(context, CL_MEM_READ_WRITE, 
            sizeof(cl_float) * count, NULL, NULL);
    t2 = clCreateBuffer(context, CL_MEM_READ_WRITE, 
            sizeof(cl_float) * count, NULL, NULL);
    if ((a_in == NULL) || (b_in == NULL) || (c_out== NULL) ||
            (t1 == NULL) || (t2 == NULL))
        error_exit("Could not allocate device memory", rval);
 
    printf("\nInitialization time %f seconds\n", get_time() - start_init);

    double start_input = get_time();
    
    /* write a and b vectors into compute device memory */
    rval = clEnqueueWriteBuffer(commands, a_in, CL_TRUE, 0, 
            sizeof(cl_float) * count, a_data, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not copy a_data to device memory", rval);
    rval = clEnqueueWriteBuffer(commands, b_in, CL_TRUE, 0, 
            sizeof(cl_float) * count, b_data, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not copy b_data to device memory", rval);

    printf("Time to copy input data %f seconds\n", get_time() - start_input);

    double start_compute = get_time();
    
    /* set arguments to compute kernels */
    rval = 0;
    rval  = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &a_in);
    rval |= clSetKernelArg(kernel1, 1, sizeof(cl_mem), &a_in);
    rval |= clSetKernelArg(kernel1, 2, sizeof(cl_mem), &t1);
    rval |= clSetKernelArg(kernel1, 3, sizeof(count), &count);
    if (rval != CL_SUCCESS)
        error_exit("Could not set kernel arguments", rval);
    rval = 0;
    rval  = clSetKernelArg(kernel2, 0, sizeof(cl_mem), &b_in);
    rval |= clSetKernelArg(kernel2, 1, sizeof(cl_mem), &b_in);
    rval |= clSetKernelArg(kernel2, 2, sizeof(cl_mem), &t2);
    rval |= clSetKernelArg(kernel2, 3, sizeof(count), &count);
    if (rval != CL_SUCCESS)
        error_exit("Could not set kernel arguments", rval);
    rval = 0;
    rval  = clSetKernelArg(kernel3, 0, sizeof(cl_mem), &t1);
    rval |= clSetKernelArg(kernel3, 1, sizeof(cl_mem), &t2);
    rval |= clSetKernelArg(kernel3, 2, sizeof(cl_mem), &c_out);
    rval |= clSetKernelArg(kernel3, 3, sizeof(count), &count);
    if (rval != CL_SUCCESS)
        error_exit("Could not set kernel arguments", rval);


    /* get maximum work group size for executing kernels on device */
    rval = clGetKernelWorkGroupInfo(kernel3, device_id, 
            CL_KERNEL_WORK_GROUP_SIZE, 
            sizeof(workgroup_size[0]), workgroup_size, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not retrieve kernel work group info", rval);

    /* set up to execute kernels over entire range of 1d input data set */
    domain_size[0] = count;

    /* put kernel1, kernel2 on queue and have them generate events on
     * completion */
    rval = clEnqueueNDRangeKernel(commands, kernel1, 1, NULL, 
            domain_size, workgroup_size, 0, NULL, &events[0]);
    if (rval != CL_SUCCESS)
        error_exit("Could not execute kernel1", rval); 
    rval = clEnqueueNDRangeKernel(commands, kernel2, 1, NULL, 
            domain_size, workgroup_size, 0, NULL, &events[1]);
    if (rval != CL_SUCCESS)
        error_exit("Could not execute kernel2", rval); 

    /* put kernel3 on queue and have it wait for kernel1, kernel2 events */
    rval = clEnqueueNDRangeKernel(commands, kernel3, 1, NULL, 
            domain_size, workgroup_size, 2, events, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not execute kernel3", rval); 

    /* wait for all commands to complete before reading back results */
    clFinish(commands);

    printf("TIme for kernel computation %f seconds\n", 
            get_time() - start_compute);

    double start_output = get_time();

    /* read back results from compute device */
    rval = clEnqueueReadBuffer(commands, c_out, CL_TRUE, 0, 
            sizeof(cl_float) * count, c_res, 0, NULL, NULL);  
    if (rval != CL_SUCCESS)
        error_exit("Could not read output array", rval);

    printf("Time to copy output data %f seconds\n", get_time() - start_output);

    printf("Total time %f seconds\n", get_time() - start_init);
    
    /* check results */
    cl_uint correct = 0;
    cl_float diff;
    for(cl_uint i = 0; i < count; ++i)
    {
        diff = a_data[i] + a_data[i] + b_data[i] + b_data[i];
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
    clReleaseMemObject(t1);
    clReleaseMemObject(t2);
    clReleaseKernel(kernel1);
    clReleaseKernel(kernel2);
    clReleaseKernel(kernel3);
    finalize(context, commands, program_obj);

    return EXIT_SUCCESS;
}

