// http://dhruba.name/2012/08/14/opencl-cookbook-listing-all-devices-and-their-critical-attributes/

// GB April 13, 2016 compile line:
// gcc -I/opt/AMDAPP/include ocllistdev.c -o ocllistdev -L/opt/AMDAPP/lib/x86_64  -lOpenCL
// added global/local/constant mem size prints
// for all cl parameters, run 'clinfo'

// Running on a 2012 vintage AMD1100T 6 core CPU + AMD 6970 GPU
// 1. Device: Cayman
//  1.1 Hardware version: OpenCL 1.2 AMD-APP (1800.11)
//  1.2 Software version: 1800.11 (VM)
//  1.3 OpenCL C version: OpenCL C 1.2
//  1.4 Parallel compute units: 24
//  1.5 global memory size: 1649410048 BYTES
//  1.6 local memory size: 32768 BYTES
//  1.7 max constant buffer size: 65536 BYTES
// 2. Device: AMD Phenom(tm) II X6 1100T Processor
//  2.1 Hardware version: OpenCL 1.2 AMD-APP (1800.11)
//  2.2 Software version: 1800.11 (sse2)
//  2.3 OpenCL C version: OpenCL C 1.2
//  2.4 Parallel compute units: 6
//  2.5 global memory size: 8284336128 BYTES
//  2.6 local memory size: 32768 BYTES
//  2.7 max constant buffer size: 65536 BYTES

#include <stdio.h>                                                                                                                                               
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
int main() {
 
    int i, j;
    char* value;
    size_t valueSize;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_uint deviceCount;
    cl_device_id* devices;
    cl_uint maxComputeUnits;
    cl_ulong globalMemSize;
    cl_ulong localMemSize;
    cl_ulong maxConstantBufferSize;
 
    // get all platforms
    clGetPlatformIDs(0, NULL, &platformCount);
    platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
    clGetPlatformIDs(platformCount, platforms, NULL);
 
    for (i = 0; i < platformCount; i++) {
 
        // get all devices
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
        devices = (cl_device_id*) malloc(sizeof(cl_device_id) * deviceCount);
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);
 
        // for each device print critical attributes
        for (j = 0; j < deviceCount; j++) {
 
            // print device name
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, valueSize, value, NULL);
            printf("%d. Device: %s\n", j+1, value);
            free(value);
 
            // print hardware device version
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value, NULL);
            printf(" %d.%d Hardware version: %s\n", j+1, 1, value);
            free(value);
 
            // print software driver version
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value, NULL);
            printf(" %d.%d Software version: %s\n", j+1, 2, value);
            free(value);
 
            // print c version supported by compiler for device
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &valueSize);
            value = (char*) malloc(valueSize);
            clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize, value, NULL);
            printf(" %d.%d OpenCL C version: %s\n", j+1, 3, value);
            free(value);
 
            // print parallel compute units
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
                    sizeof(maxComputeUnits), &maxComputeUnits, NULL);
            printf(" %d.%d Parallel compute units: %d\n", j+1, 4, maxComputeUnits);

            // print global memory size
            clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE,
                    sizeof(globalMemSize), &globalMemSize, NULL);
            printf(" %d.%d global memory size: %ld BYTES\n", j+1, 5, globalMemSize);

            // print local memory size
            clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE,
                    sizeof(localMemSize), &localMemSize, NULL);
            printf(" %d.%d local memory size: %ld BYTES\n", j+1, 6, localMemSize);

            // print constant buffer size
            clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
                    sizeof(maxConstantBufferSize), &maxConstantBufferSize, NULL);
            printf(" %d.%d max constant buffer size: %ld BYTES\n", j+1, 7, maxConstantBufferSize);
        }
 
        free(devices);
 
    }
    free(platforms);
}
