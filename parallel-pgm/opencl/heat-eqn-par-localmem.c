/*
 * heat diffusion example, as discussed in textbook.
 *
 * version that uses OpenCL kernel to do computation and copies to
 * local memory.
 *
 * disclaimer:  performance is quite disappointing, but it's beyond the
 * scope of this course this year to fix that.
 * 
 * command line arguments:
 *
 * number of points
 * maximum number of iterations
 * convergence threshold
 * points per workitem 
 * workgroup size factor (N>0 to use N times the preferred/quantum size, 
 *   up to maximum)
 * (optional) filename to print final values (values are not printed if no 
 *   filename is given)
 * (optional) CPU to run kernel on CPU (default is GPU)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <CL/cl.h>

#include "timer.h"
#include "cmdline.h"
#include "utility.h"

#define LEFTVAL 1.0f
#define RIGHTVAL 10.0f

#include "heat-eqn-functions.h"
#include "heat-eqn-par-functions.h"

void initialize_values(float uk[], int nx);

/*
 * kernel for computational device:
 *
 * compute next values, maximum difference
 */
/* macros to map between global indices and local-to-workgroup indices */
const char *kernel_source_macros = "\n" \
"#define GLOBAL(local_ix) ((local_ix) + group_id*workgroup_points)      \n" \
"#define LOCAL(global_ix) ((global_ix) - group_id*workgroup_points)     \n" \
"\n";
const char *kernel_source = "\n" \
"__kernel void do_step(                                                 \n" \
"   const int          num_points,                                      \n" \
"   const int          points_per_workitem,                             \n" \
"   const float        dx,                                              \n" \
"   const float        dt,                                              \n" \
"   __global float*    uk,                                              \n" \
"   __global float*    ukp1,                                            \n" \
"   __local  float*    local_uk,                                        \n" \
"   __local  float*    local_ukp1,                                      \n" \
"   __local  float*    local_maxdiff,                                   \n" \
"   __global float*    partial_maxdiff)                                 \n" \
"{                                                                      \n" \
"   int local_work_items  = get_local_size(0);                          \n" \
"   int local_id = get_local_id(0);                                     \n" \
"   int group_id = get_group_id(0);                                     \n" \
"   int global_id = get_global_id(0);                                   \n" \
"   int workgroup_points = local_work_items*points_per_workitem;        \n" \
"   int group_last;                                                     \n" \
"   int global_group_first;                                             \n" \
"   int global_group_last;                                              \n" \
"   if (local_id == 0) {                                                \n" \
"      /* copy local section of uk from global memory */                \n" \
"      group_last = workgroup_points;                                   \n" \
"      global_group_first = GLOBAL(1);                                  \n" \
"      global_group_last = GLOBAL(group_last);                          \n" \
"      if (global_group_last > num_points) {                            \n" \
"          global_group_last = num_points;                              \n" \
"          group_last = LOCAL(global_group_last);                       \n" \
"      }                                                                \n" \
"      for (int i=global_group_first; i<=global_group_last; ++i)        \n" \
"          local_uk[LOCAL(i)]=uk[i];                                    \n" \
"      /* copy boundary values if needed */                             \n" \
"      if (group_last >= 1) {                                           \n" \
"          local_uk[LOCAL(global_group_first-1)] =                      \n" \
"              uk[global_group_first-1];                                \n" \
"          local_uk[LOCAL(global_group_last+1)] =                       \n" \
"              uk[global_group_last+1];                                 \n" \
"      }                                                                \n" \
"   }                                                                   \n" \
"   barrier(CLK_LOCAL_MEM_FENCE);                                       \n" \
"   /* give each work item a contiguous range of points */              \n" \
"   int item_first = 1 + local_id*points_per_workitem;                  \n" \
"   int item_last = item_first + points_per_workitem - 1;               \n" \
"   int global_first = GLOBAL(item_first);                              \n" \
"   int global_last = GLOBAL(item_last);                                \n" \
"   if (global_last > num_points) {                                     \n" \
"        global_last = num_points;                                      \n" \
"        item_last = LOCAL(global_last);                                \n" \
"   }                                                                   \n" \
"   /* compute new values */                                            \n" \
"   for (int i = item_first; i <= item_last; ++i)                       \n" \
"       local_ukp1[i]=local_uk[i] +                                     \n" \
"           (dt/(dx*dx))*(local_uk[i+1]-2*local_uk[i]+local_uk[i-1]);   \n" \
"   /* check for convergence */                                         \n" \
"   float accum = 0.0f;                                                 \n" \
"   for (int i = item_first; i <= item_last; ++i) {                     \n" \
"       float diff = fabs(local_uk[i] - local_ukp1[i]);                 \n" \
"       accum = (diff > accum) ? diff : accum;                          \n" \
"   }                                                                   \n" \
"   local_maxdiff[local_id] = accum;                                    \n" \
"   barrier(CLK_LOCAL_MEM_FENCE);                                       \n" \
"   if (local_id == 0) {                                                \n" \
"      /* copy local section of ukp1 to global memory */                \n" \
"      for (int i=global_group_first; i<=global_group_last; ++i)        \n" \
"          ukp1[i]=local_ukp1[LOCAL(i)];                                \n" \
"      /* combine convergence results */                                \n" \
"      float maxdiff = 0.0f;                                            \n" \
"      for(int i = 0; i < local_work_items; ++i)                        \n" \
"          if (local_maxdiff[i] > maxdiff) maxdiff = local_maxdiff[i];  \n" \
"      partial_maxdiff[group_id] = maxdiff;                             \n" \
"   }                                                                   \n" \
"}                                                                      \n" \
"\n";


int main(int argc, char *argv[]) {

    int nx;
    int maxsteps;
    float threshold;
    cl_int num_points;
    cl_int work_groups;
    cl_int points_per_workitem;
    cl_int workgroup_size_factor;
    FILE* outfile;
    cl_device_type device_type;

    cl_float *uk; 
    cl_float dx, dt;
    cl_float maxdiff;
    int step;

    double start_time, end_time;
    double DBG_time;

    cl_float *partial_maxdiff_host;
    cl_int rval;

    size_t workgroup_size_quantum, max_workgroup_size;
    size_t domain_size[1];    /* global domain size */
    size_t workgroup_size[1];

    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
    cl_program program_obj;
    cl_kernel kernel;

    cl_mem partial_maxdiff;
    cl_mem uk_buffers[2];
    int uk_index, ukp1_index, temp_index;

    parse_arguments(argc, argv, &nx, &maxsteps, &threshold, 
            &points_per_workitem, &workgroup_size_factor,
            &outfile, &device_type);

    start_time = get_time();
    DBG_time = start_time;

    /* initialize host variables */
    /* uk has nx interior points plus fixed ends */
    /* (no need to keep ukp1 in host) */
    uk = malloc(sizeof(*uk) * (nx+2));
    if (uk == NULL) {
        fprintf(stderr, "Unable to allocate host memory\n");
        return EXIT_FAILURE;
    }
    dx = 1.0f/nx;
    dt = 0.5f*dx*dx;
    maxdiff = threshold;
    initialize_values(uk, nx);

    /* create context, command queue, and program object */
    const char* kernel_sources[] = { kernel_source_macros, kernel_source };
    initialize(device_type,
            &device_id, &context, &commands, &program_obj, 2, kernel_sources);
 
    /* create compute kernel from program object */
    kernel = clCreateKernel(program_obj, "do_step", &rval);
    if ((rval != CL_SUCCESS) || (kernel == NULL))
        error_exit("Unable to create compute kernel", rval);

    /* get info about work group sizes */
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

    printf("\nDBG time for initialization %g\n", get_time() - DBG_time);
    DBG_time = get_time();

   /* set number of work groups, number of points (for use by kernel) */
    int points_per_workgroup = workgroup_size[0]*points_per_workitem;
    work_groups = (nx+points_per_workgroup-1)/points_per_workgroup;
    if (work_groups < 1) work_groups = 1;
    num_points = nx;

    /* compute sizes of __local variables, check against max */
    int size_local_maxdiff = sizeof(cl_float)*workgroup_size[0];
    int size_local_uk = sizeof(cl_float)*(points_per_workgroup+2);
    int size_local_ukp1 = size_local_uk;
    cl_ulong local_mem_size;
    rval = clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, 
            sizeof(local_mem_size), &local_mem_size, NULL);
    if (rval != CL_SUCCESS) {
            describe_error("Unable to get size of __local:", rval, stderr);
    }
    else {
        int size_needed = size_local_maxdiff+size_local_uk+size_local_ukp1;
        if (local_mem_size < size_needed) {
            describe_error("Unable to allocate space for __local:", 
                    CL_SUCCESS, stderr);
            fprintf(stderr, "  %d bytes available, %d needed\n",
                    (int)local_mem_size, size_needed);
            exit(EXIT_FAILURE);
        }
    } 

    /* allocate host memory for partial maxdiff results */
    partial_maxdiff_host = malloc(sizeof(*partial_maxdiff_host)*work_groups);
    if (partial_maxdiff_host == NULL) {
        fprintf(stderr, "Unable to allocate host memory\n");
        return EXIT_FAILURE;
    }

    /* allocate device memory for uk (old and new), partial maxdiff results */
    uk_buffers[0] = clCreateBuffer(context, CL_MEM_READ_WRITE,
            sizeof(cl_float) * (nx+2), NULL, NULL);
    uk_buffers[1] = clCreateBuffer(context, CL_MEM_READ_WRITE,
            sizeof(cl_float) * (nx+2), NULL, NULL);
    partial_maxdiff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            sizeof(cl_float) * work_groups, NULL, NULL);
    if ((uk_buffers[0] == NULL) || (uk_buffers[1] == NULL) || 
            (partial_maxdiff == NULL))
    {
        describe_error("Unable to allocate device memory", rval, stderr);
        fprintf(stderr, "Total space needed is %d\n",
            (int)(sizeof(cl_float) * ((nx+2)*2 + work_groups) +
            2*sizeof(cl_int) * domain_size[0]));
        exit(EXIT_FAILURE);
    }

    printf("DBG time for initialization %g\n", get_time() - DBG_time);
    DBG_time = get_time();

    /* copy uk, ukp1 data to device */
    uk_index = 0;
    ukp1_index = 1;
    rval = clEnqueueWriteBuffer(commands, uk_buffers[uk_index], CL_TRUE, 0, 
            sizeof(cl_float) * (nx+2), uk, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not copy uk to device memory", rval);
    rval = clEnqueueWriteBuffer(commands, uk_buffers[ukp1_index], CL_TRUE, 0, 
            sizeof(cl_float) * (nx+2), uk, 0, NULL, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not copy ukp1 to device memory", rval);

    printf("DBG time for copy to device %g\n", get_time() - DBG_time);
    DBG_time = get_time();

    /* set up to execute kernel over entire range of indices */
    domain_size[0] = work_groups * workgroup_size[0];

    /* set arguments to compute kernel */
    rval = 0;
    rval |= clSetKernelArg(kernel, 0, sizeof(num_points), &num_points);
    rval |= clSetKernelArg(kernel, 1, sizeof(points_per_workitem), 
            &points_per_workitem);
    rval |= clSetKernelArg(kernel, 2, sizeof(dx), &dx);
    rval |= clSetKernelArg(kernel, 3, sizeof(dt), &dt);
    /* fill in args 4 and 5 (uk, ukp1) later -- will change on every step */
    rval |= clSetKernelArg(kernel, 6, size_local_uk, NULL);
    rval |= clSetKernelArg(kernel, 7, size_local_ukp1, NULL);
    rval |= clSetKernelArg(kernel, 8, size_local_maxdiff, NULL);
    rval |= clSetKernelArg(kernel, 9, sizeof(cl_mem), &partial_maxdiff);
    if (rval != CL_SUCCESS)
        error_exit("Unable to set kernel arguments", rval);

    for (step = 0; (step < maxsteps) && (maxdiff >= threshold); ++step) {

        /* set remaining arguments to compute kernel */
        rval = 0;
        rval |= clSetKernelArg(kernel, 4, sizeof(cl_mem), 
                &uk_buffers[uk_index]);
        rval |= clSetKernelArg(kernel, 5, sizeof(cl_mem), 
                &uk_buffers[ukp1_index]);
        if (rval != CL_SUCCESS)
            error_exit("Unable to set kernel arguments", rval);

        /* run kernel to compute new value and maximum difference */
        rval = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, 
                domain_size, workgroup_size, 0, NULL, NULL);
        if (rval != CL_SUCCESS)
            error_exit("Could not execute kernel", rval); 

        /* read and combine partial maxdiffs */
        rval = clEnqueueReadBuffer(commands, partial_maxdiff, CL_TRUE, 0, 
                sizeof(cl_float) * work_groups, partial_maxdiff_host, 0, 
                NULL, NULL );  
        if (rval != CL_SUCCESS)
            error_exit("Could not read diffs from device", rval);
        maxdiff = 0.0;

        for (int i=0; i < (int)work_groups; ++i) 
            if (partial_maxdiff_host[i] > maxdiff) 
                maxdiff = partial_maxdiff_host[i];

        /* "copy" ukp1 to uk by swapping pointers */
        temp_index = uk_index; uk_index = ukp1_index; ukp1_index = temp_index;
    }

    printf("DBG time for calculation %g\n", get_time() - DBG_time);
    DBG_time = get_time();

    /* copy uk data from device */
    rval = clEnqueueReadBuffer(commands, uk_buffers[uk_index], CL_TRUE, 0, 
            sizeof(cl_float) * (nx+2), uk, 0, NULL, NULL);  
    if (rval != CL_SUCCESS)
        error_exit("Could not copy uk from device memory", rval);

    printf("DBG time for copy from device %g\n", get_time() - DBG_time);
    DBG_time = get_time();

    end_time = get_time();

    if (outfile != NULL) {
        print_values(outfile, uk, nx);
        fclose(outfile);
    }

    printf("OpenCL program with localmem using device:\n");
    output_device_info(stdout, device_id, CL_FALSE);
    printf("%d points per work item, %d work groups of size %d, %d work items\n", 
            points_per_workitem,
            work_groups, (int)workgroup_size[0],
            work_groups * (int)workgroup_size[0]);
    printf("nx = %d, maxsteps = %d, threshold = %g\n", nx, maxsteps, threshold);
    if (maxdiff < threshold) {
        printf("converged in %d iterations\n", step);
    }
    else {
        printf("failed to converge in %d iterations, maxdiff = %g\n", 
                step, maxdiff);
    }
    printf("execution time = %g\n", end_time - start_time);

    /* clean up and end */
    free(uk);
    free(partial_maxdiff_host);
    clReleaseMemObject(uk_buffers[0]);
    clReleaseMemObject(uk_buffers[1]);
    clReleaseMemObject(partial_maxdiff);
    clReleaseKernel(kernel);
    finalize(context, commands, program_obj);
    return EXIT_SUCCESS;
}

