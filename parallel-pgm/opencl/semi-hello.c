/*
 * First example of OpenCL program:
 * Show information about available devices.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#include "utility.h"

#define MAX_PLATFORMS 6
#define MAX_DEVICES 6

int main(int argc, char* argv[]) {
    cl_uint num_platforms;
    cl_platform_id platforms[MAX_PLATFORMS];
    cl_uint num_devices;
    cl_device_id devices[MAX_DEVICES];

    cl_int rval = clGetPlatformIDs(MAX_PLATFORMS, platforms, &num_platforms);
    if (rval != CL_SUCCESS) 
        error_exit("Could not get platform(s)", rval);

    int main_rval = EXIT_SUCCESS;

    for (cl_uint i = 0; i < num_platforms; ++i) {
        fprintf(stdout, "Platform %d:\n", i);
        rval = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL,
                MAX_DEVICES, devices, &num_devices);
        if (rval != CL_SUCCESS) 
            error_exit("Could not get device(s)", rval);
        for (cl_uint j = 0; j < num_devices; ++j) {
            fprintf(stdout, "Device %d:\n", j);
            rval = output_device_info(stdout, devices[j], CL_TRUE);
            if (rval != CL_SUCCESS) main_rval = EXIT_FAILURE;
        }
    }

    return main_rval;
}
