/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * version that uses OpenCL kernel to do computation, as discussed in draft
 * Appendix D, but adds command-line arguments to allow varying some things.
 * 
 * command-line arguments:
 * iterations_per_workitem 
 * workgroup_size_factor (N>0 to use N times the preferred/quantum size, 
 *   up to maximum)
 * (optional) CPU to run kernel on CPU (default is GPU)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* copied from not-strictly-standard part of math.h */
#define M_PI		3.14159265358979323846

#include <CL/cl.h>
#include "timer.h"
#include "cmdline.h"
#include "utility.h"

#define NUM_STEPS 400000000

/*
 * kernel for computational device:
 *
 * compute partial sum for computation of pi
 */
const char *kernel_source = "\n" \
"__kernel void pi(                                                      \n" \
"   const int          nsteps,                                          \n" \
"   const int          work_groups,                                     \n" \
"   const float        step_size,                                       \n" \
"   __local  float*    local_sums,                                      \n" \
"   __global float*    partial_sums)                                    \n" \
"{                                                                      \n" \
"   int num_work_items  = get_local_size(0);                            \n" \
"   int local_id = get_local_id(0);                                     \n" \
"   int group_id = get_group_id(0);                                     \n" \
"   int global_id = get_global_id(0);                                   \n" \
"   float x, sum, accum = 0.0;                                          \n" \
"   int iskip  = num_work_items * work_groups;                          \n" \
"   for(int i = global_id; i < nsteps; i += iskip) {                    \n" \
"       x = (i+0.5f)*step_size;                                         \n" \
"       accum += 4.0f/(1.0f+x*x);                                       \n" \
"   }                                                                   \n" \
"   local_sums[local_id] = accum;                                       \n" \
"   barrier(CLK_LOCAL_MEM_FENCE);                                       \n" \
"   if (local_id == 0) {                                                \n" \
"      sum = 0.0f;                                                      \n" \
"      for(int i = 0; i < num_work_items; ++i) {                        \n" \
"          sum += local_sums[i];                                        \n" \
"      }                                                                \n" \
"      partial_sums[group_id] = sum;                                    \n" \
"   }                                                                   \n" \
"}                                                                      \n" \
"\n";

/* main program */
int main(int argc, char *argv[]) {

    char* usage_msg = 
        "usage:  %s iterations_per_workitem workgroup_size_factor [\"CPU\"]\n";
    int iterations_per_workitem = get_integer_arg(argc, argv, 1,
            1, "iterations_per_workitem", usage_msg);
    int workgroup_size_factor = get_integer_arg(argc, argv, 2,
            1, "workgroup_size_factor", usage_msg);
    cl_device_type device_type = CL_DEVICE_TYPE_GPU;
    if ((argc > 3) && (strcmp(argv[3], "CPU") == 0)) {
        device_type = CL_DEVICE_TYPE_CPU;
    }

    double start_time, end_time;
    cl_float pi;
    cl_int nsteps;
    cl_int work_groups;
    cl_float step_size;
    cl_float *partial_sums_host; /* partial sums from compute device */

    cl_int rval;

    cl_uint compute_units;

    size_t workgroup_size_quantum, max_workgroup_size;
    size_t domain_size[1];    /* global domain size */
    size_t workgroup_size[1];

    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
    cl_program program_obj;
    cl_kernel kernel;

    cl_mem partial_sums; /* device memory for partial sums */

    /* record start time */
    start_time = get_time();

    /* create context, command queue, and program object */
    const char* kernel_sources[] = { kernel_source };
    initialize(device_type,
            &device_id, &context, &commands, &program_obj, 1, kernel_sources);
    printf("Compute device:\n");
    output_device_info(stdout, device_id, CL_FALSE);
    puts("");
 
    /* create compute kernel from program object */
    kernel = clCreateKernel(program_obj, "pi", &rval);
    if ((rval != CL_SUCCESS) || (kernel == NULL))
        error_exit("Could not create compute kernel", rval);

    /* get info about compute units, work group sizes */
    rval = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS,
            sizeof(compute_units), &compute_units, NULL);
    if (rval != CL_SUCCESS) {
        compute_units = 0;
        describe_error("Could not retrieve device info", rval, stderr);
    }
    rval = clGetKernelWorkGroupInfo(kernel, device_id, 
            CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
            sizeof(workgroup_size_quantum), &workgroup_size_quantum, NULL);
    if (rval != CL_SUCCESS) 
        error_exit("Could not retrieve kernel work group info", rval);
    rval = clGetKernelWorkGroupInfo(kernel, device_id, 
            CL_KERNEL_WORK_GROUP_SIZE, 
            sizeof(max_workgroup_size), &max_workgroup_size, NULL);
    if (rval != CL_SUCCESS) 
        error_exit("Could not retrieve kernel work group info", rval); 

    /* set workgroup size */
    workgroup_size[0] = workgroup_size_quantum * workgroup_size_factor;
    if (workgroup_size[0] > max_workgroup_size) {
        fprintf(stderr, "Computed workgroup size %d exceeds maximum %d\n",
                (int)workgroup_size[0], (int)max_workgroup_size);
        exit(EXIT_FAILURE);
    }

    /* set number of work groups, number of steps, step size */
    cl_int iterations_per_workgroup = workgroup_size[0]*iterations_per_workitem;
    work_groups = 
        (NUM_STEPS+iterations_per_workgroup-1)/iterations_per_workgroup;
    if (work_groups < 1) work_groups = 1;
    nsteps = NUM_STEPS;
    step_size = 1.0f/(cl_float)nsteps;
    partial_sums_host = malloc(sizeof(*partial_sums_host)*work_groups);
    if (partial_sums_host == NULL) 
        error_exit("Could not allocate memory for partial sums", CL_SUCCESS);

    /* create output buffer to hold partial sums */
    partial_sums = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            sizeof(cl_float) * work_groups, NULL, NULL);
    if (partial_sums == NULL)
        error_exit("Could not allocate device memory", rval);

    /* set arguments to compute kernel */
    /* note argument 2 is a local array so all we do here is define its size */
    rval = 0;
    rval  = clSetKernelArg(kernel, 0, sizeof(nsteps), &nsteps);
    rval  = clSetKernelArg(kernel, 1, sizeof(work_groups), &work_groups);
    rval |= clSetKernelArg(kernel, 2, sizeof(step_size), &step_size);
    rval |= clSetKernelArg(kernel, 3, sizeof(cl_float)*workgroup_size[0], NULL);
    rval |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &partial_sums);
    if (rval != CL_SUCCESS)
        error_exit("Could not set kernel arguments", rval);

    /* execute kernel over entire range of 1d input data set */
    domain_size[0] = work_groups * workgroup_size[0];
    rval = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, 
            domain_size, workgroup_size, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not execute kernel", rval); 

    /* wait for commands to complete before reading back results */
    clFinish(commands);

    /* read back results from compute device */
    rval = clEnqueueReadBuffer(commands, partial_sums, CL_TRUE, 0, 
            sizeof(cl_float) * work_groups, partial_sums_host, 0, NULL, NULL );  
    if (rval != CL_SUCCESS)
        error_exit("Could not read output array", rval);

    /* complete sum and compute final integral value */
    pi = 0.0;
    for (int i=0; i < (int)work_groups; ++i)
        pi += partial_sums_host[i];
    pi *= step_size;

    /* record end time */
    end_time = get_time();

    /* print results */
    printf("OpenCL program result with %d steps, %d iterations per work item\n",
            nsteps, iterations_per_workitem);
    printf("  %d compute units, %d work groups of size %d, %d work items\n", 
            compute_units,
            work_groups, (int)workgroup_size[0],
            work_groups * (int)workgroup_size[0]);
    printf("computed pi = %g  (%.15f)\n",pi, pi);
    printf("difference between computed pi and math.h M_PI = %.15f\n", 
            fabs(pi - M_PI));
    printf("time to compute = %g seconds\n", end_time - start_time);

    /* clean up then shut down */
    free(partial_sums_host);
    clReleaseMemObject(partial_sums);
    clReleaseKernel(kernel);
    finalize(context, commands, program_obj);
 
    return EXIT_SUCCESS;
}
