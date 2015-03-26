/*
 * Vector computation in OpenCL:
 * Apply function (defined in vector-compute-aux.h) elementwise to vector.
 * Also times the same code on the host, sequentially and with OpenMP.
 * Must compile with "-fopenmp".  
 *
 * Command-line arguments:
 * (*) GPU | CPU to say whether to run the OpenCL kernel on the GPU or CPU
 * (*) workgroup_size_factor to vary workgroup size -- a value of 0 lets
 *    the implementation decide, while N>0 gives N times the preferred/quantum
 *    size (up to maximum)
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <CL/cl.h>

#include "timer.h"
#include "cmdline.h"
#include "utility.h"

#define TOL    (0.001)   /* tolerance used in floating point comparisons */
#define LENGTH (4096)    /* length of vectors */

#include "vector-compute-aux.h" /* function to apply */
const char* auxiliary_source_filename = "vector-compute-aux.h";

void compute_host_seq(const cl_uint count, cl_float in[], cl_float host_out[]);
void compute_host_par(const cl_uint count, cl_float in[], cl_float out[]);
void compute_with_kernel(cl_device_type device_type,
        const cl_uint count, const long workgroup_size_factor,
        cl_float in[], cl_float host_out[], cl_float out[]);

/*
 * kernel for computational device:
 *
 * applies function elementwise
 */
const char *kernel_source = "\n" \
"__kernel void apply_foobar(                                            \n" \
"   __global float* in,                                                 \n" \
"   __global float* out,                                                \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count)                                                       \n" \
"       out[i] = foobar(in[i]);                                         \n" \
"}                                                                      \n" \
"\n";

/* main program */
int main(int argc, char* argv[]) 
{
    cl_device_type device_type;

    char* usage_msg = "usage:  %s GPU|CPU workgroup_size_factor\n";
    if (argc < 3) {
        fprintf(stderr, usage_msg, argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "GPU") == 0) {
        device_type = CL_DEVICE_TYPE_GPU;
    } else if (strcmp(argv[1], "CPU") == 0) {
        device_type = CL_DEVICE_TYPE_CPU;
    } else {
        fprintf(stderr, usage_msg, argv[0]);
        return EXIT_FAILURE;
    }

    long workgroup_size_factor = get_integer_arg(argc, argv, 2,
            0, "workgroup_size_factor", usage_msg);

    cl_float in[LENGTH];
    cl_float out[LENGTH];
    cl_float host_out[LENGTH];
   
    cl_uint count = LENGTH;

    /* fill input vectors with random values */
    for(cl_uint i = 0; i < count; ++i){
        in[i] = rand() / (cl_float)RAND_MAX;
    }
    /* compute output values sequentially in host */
    compute_host_seq(count, in, host_out);

    /* compute output values with OpenMP in host */
    puts("");
    compute_host_par(count, in, out);

    /* compute output values with OpenCL kernle and check */
    puts("");
    compute_with_kernel(device_type, count, workgroup_size_factor, 
            in, host_out, out);

    return EXIT_SUCCESS;
}

/* computational subroutines */

void compute_host_seq(const cl_uint count, cl_float in[], cl_float host_out[]) {
    double start_host = get_time();
    for(cl_uint i = 0; i < count; ++i){
        host_out[i] = foobar(in[i]);
    }
    printf("Time to compute in host (sequential) %f seconds\n", 
            get_time() - start_host);
}

void compute_host_par(const cl_uint count, cl_float in[], cl_float out[]) {
    int nthreads;
    double start_host_par = get_time();
    #pragma omp parallel
    {
        #pragma omp single
        {
            nthreads = omp_get_num_threads();
        }
        #pragma omp for
        for(cl_uint i = 0; i < count; ++i){
            out[i] = foobar(in[i]);
        }
    }
    printf("Time to compute in host (%d threads) %f seconds\n", 
            nthreads, get_time() - start_host_par);
}

void compute_with_kernel(cl_device_type device_type,
        const cl_uint count, const long workgroup_size_factor,
        cl_float in[], cl_float host_out[], cl_float out[])
{
    cl_int rval;

    size_t workgroup_size_quantum, max_workgroup_size;
    size_t domain_size[1];    /* global domain size */
    size_t workgroup_size[1]; 
    size_t* workgroup_size_p = workgroup_size; 
      /* set to NULL to let implementation choose */

    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
    cl_program program_obj;
    char* auxiliary_source;
    cl_kernel kernel;
    
    cl_mem in_device;     /* device memory for input vector */
    cl_mem out_device;    /* device memory for output vector */

    double start_init = get_time();

    /* load auxiliary function(s) from file */
    auxiliary_source = (char*) source_from_file(auxiliary_source_filename);
    if (auxiliary_source == NULL) {
        fprintf(stderr, "Could not read auxiliary functions from %s\n", 
                auxiliary_source_filename);
        exit(EXIT_FAILURE);
    }

    /* create context, command queue, program object */
    const char* kernel_sources[] = { auxiliary_source, kernel_source };
    initialize(device_type,
            &device_id, &context, &commands, &program_obj, 2, kernel_sources);
    printf("Compute device:\n");
    output_device_info(stdout, device_id, CL_FALSE);

    /* create compute kernel from program object */
    kernel = clCreateKernel(program_obj, "apply_foobar", &rval);
    if ((rval != CL_SUCCESS) || (kernel  == NULL)) {
        error_exit("Could not create compute kernel", rval);
    }

    /* create input (a, b) and output (c) arrays in device memory  */
    in_device = clCreateBuffer(context, CL_MEM_READ_ONLY,  
            sizeof(cl_float) * count, NULL, NULL);
    out_device = clCreateBuffer(context,  CL_MEM_WRITE_ONLY, 
            sizeof(cl_float) * count, NULL, NULL);
    if ((in_device == NULL) || (out_device == NULL)) {
        error_exit("Could not allocate device memory", rval);
    }

    /* get info about work group sizes */
    rval = clGetKernelWorkGroupInfo(kernel, device_id, 
            CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
            sizeof(workgroup_size_quantum), &workgroup_size_quantum, NULL);
    if (rval != CL_SUCCESS) {
        error_exit("Could not retrieve kernel work group info", rval);
    }
    rval = clGetKernelWorkGroupInfo(kernel, device_id, 
            CL_KERNEL_WORK_GROUP_SIZE, 
            sizeof(max_workgroup_size), &max_workgroup_size, NULL);
    if (rval != CL_SUCCESS) {
        error_exit("Could not retrieve kernel work group info", rval); 
    }

    /* compute/set sizes */
    domain_size[0] = count;
    if (workgroup_size_factor > 0) {
        workgroup_size[0] = workgroup_size_quantum * workgroup_size_factor;
        if (workgroup_size[0] > max_workgroup_size) {
            fprintf(stderr, "Computed workgroup size %d exceeds maximum %d\n",
                    (int)workgroup_size[0], (int)max_workgroup_size);
            exit(EXIT_FAILURE);
        }
        if ((domain_size[0] % workgroup_size[0]) != 0) {
            fprintf(stderr, "Computed workgroup size %d does not divide %d\n",
                    (int)workgroup_size[0], (int)count);
            exit(EXIT_FAILURE);
        }
    } else {
        workgroup_size_p = NULL;
    }

    /* set arguments to compute kernel */
    rval = 0;
    rval  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &in_device);
    rval |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &out_device);
    rval |= clSetKernelArg(kernel, 2, sizeof(count), &count);
    if (rval != CL_SUCCESS) {
        error_exit("Could not set kernel arguments", rval);
    }

    printf("\nInitialization time %f seconds\n", get_time() - start_init);

    double start_input = get_time();
    
    /* write input vector into compute device memory */
    rval = clEnqueueWriteBuffer(commands, in_device, CL_TRUE, 0, 
            sizeof(cl_float) * count, in, 0, NULL, NULL);
    if (rval != CL_SUCCESS) {
        error_exit("Could not copy input to device memory", rval);
    }

    printf("Time to copy input data %f seconds\n", get_time() - start_input);

    double start_compute = get_time();

    /* execute kernel over entire range of 1d input data set */
    rval = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, 
            domain_size, workgroup_size_p, 0, NULL, NULL);
    if (rval != CL_SUCCESS) {
        error_exit("Failed to execute kernel", rval);
    } 

    /* wait for commands to complete before reading back results */
    clFinish(commands);

    printf("Time for kernel computation (workgroup size ");
    if (workgroup_size_p == NULL) {
        printf("unspecified");
    } else {
        printf("%d", (int)workgroup_size[0]);
    }
    printf(") %f seconds\n", get_time() - start_compute);

    double start_output = get_time();

    /* read back results from compute device */
    rval = clEnqueueReadBuffer(commands, out_device, CL_TRUE, 0, 
            sizeof(cl_float) * count, out, 0, NULL, NULL);  
    if (rval != CL_SUCCESS) {
        error_exit("Could not read output array", rval);
    }
    
    printf("Time to copy output data %f seconds\n", get_time() - start_output);

    printf("Total time %f seconds\n", get_time() - start_init);
    
     /* check results */
    cl_uint correct = 0;
    cl_float diff;
    for(cl_uint i = 0; i < count; ++i)
    {
        diff = host_out[i];
        diff -= out[i];
        if(diff*diff < TOL*TOL)
            ++correct;
        else {
            printf("difference %f in element %d:  in %f out %f\n",
                    diff, i, in[i], out[i]);
        }
    }
    
    /* summarize results */
    printf("\n%d out of %d results were correct\n", correct, count);
    
    /* clean up */
    free(auxiliary_source);
    clReleaseMemObject(in_device);
    clReleaseMemObject(out_device);
    clReleaseKernel(kernel);
    finalize(context, commands, program_obj);
}

void compute_with_GPU(const cl_uint count, const long workgroup_size_factor,
        cl_float in[], cl_float host_out[], cl_float out[])
{
    puts("Computing using GPU:");
    compute_with_kernel(CL_DEVICE_TYPE_GPU, 
            count, workgroup_size_factor, in, host_out, out);
}

void compute_with_CPU(const cl_uint count, const long workgroup_size_factor,
        cl_float in[], cl_float host_out[], cl_float out[])
{
    puts("Computing using CPU:");
    compute_with_kernel(CL_DEVICE_TYPE_CPU, 
            count, workgroup_size_factor, in, host_out, out);
}
