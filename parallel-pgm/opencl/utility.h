/*
 * Utility routines for use in OpenCL programs.
 */
#ifndef OPENCL_UTILITY_H_
#define OPENCL_UTILITY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>
#include "filesize.h"

/* 
 * Print error message:
 * msg is message to print, or NULL
 * rval is OpenCL return code, printed if other than CL_SUCCESS
 *   (text description for some values, otherwise numeric value)
 */
void describe_error(const char * msg, cl_int rval, FILE * outfile) 
{
    fprintf(outfile, "Error:  ");
    if (msg != NULL) fprintf(outfile, "%s  ", msg);
    if (rval != CL_SUCCESS) {
        fprintf(outfile, "(error %d)", (int)rval);
        /* partial list of possible errors */
        switch (rval) {
            case CL_INVALID_VALUE:
                fprintf(outfile, " (CL_INVALID_VALUE)");
                break;
            case CL_INVALID_DEVICE:
                fprintf(outfile, " (CL_INVALID_DEVICE)");
                break;
            case CL_INVALID_CONTEXT:
                fprintf(outfile, " (CL_INVALID_CONTEXT)");
                break;
            case CL_INVALID_QUEUE_PROPERTIES:
                fprintf(outfile, " (CL_INVALID_QUEUE_PROPERTIES)");
                break;
            case CL_OUT_OF_RESOURCES:
                fprintf(outfile, " (CL_OUT_OF_RESOURCES)");
                break;
            case CL_INVALID_BUILD_OPTIONS:
                fprintf(outfile, " (CL_INVALID_BUILD_OPTIONS)"); 
                break;
            case CL_BUILD_PROGRAM_FAILURE:
                fprintf(outfile, " (CL_BUILD_PROGRAM_FAILURE)"); 
                break;
            case CL_INVALID_PROGRAM:
                fprintf(outfile, " (CL_INVALID_PROGRAM)"); 
                break;
            case CL_INVALID_PROGRAM_EXECUTABLE:
                fprintf(outfile, " (CL_INVALID_PROGRAM_EXECUTABLE)"); 
                break;
            case CL_INVALID_KERNEL:
                fprintf(outfile, " (CL_INVALID_KERNEL)"); 
                break;
            case CL_INVALID_KERNEL_NAME:
                fprintf(outfile, " (CL_INVALID_KERNEL_NAME)"); 
                break;
            case CL_INVALID_KERNEL_DEFINITION:
                fprintf(outfile, " (CL_INVALID_KERNEL_DEFINITION)"); 
                break;
            case CL_INVALID_KERNEL_ARGS:
                fprintf(outfile, " (CL_INVALID_KERNEL_ARGS)"); 
                break;
            case CL_INVALID_ARG_VALUE:
                fprintf(outfile, " (CL_INVALID_ARG_VALUE)");
                break;
            case CL_INVALID_ARG_SIZE:
                fprintf(outfile, " (CL_INVALID_ARG_SIZE)");
                break;
            case CL_INVALID_WORK_DIMENSION:
                fprintf(outfile, " (CL_INVALID_WORK_DIMENSION)"); 
                break;
            case CL_INVALID_GLOBAL_OFFSET:
                fprintf(outfile, " (CL_INVALID_GLOBAL_OFFSET)"); 
                break;
            case CL_INVALID_WORK_GROUP_SIZE:
                fprintf(outfile, " (CL_INVALID_WORK_GROUP_SIZE)"); 
                break;
            case CL_INVALID_WORK_ITEM_SIZE:
                fprintf(outfile, " (CL_INVALID_WORK_ITEM_SIZE)"); 
                break;
            case CL_INVALID_IMAGE_SIZE:
                fprintf(outfile, " (CL_INVALID_IMAGE_SIZE)"); 
                break;
            case CL_INVALID_EVENT_WAIT_LIST:
                fprintf(outfile, " (CL_INVALID_EVENT_WAIT_LIST)"); 
                break;
            case CL_MEM_OBJECT_ALLOCATION_FAILURE:
                fprintf(outfile, " (CL_MEM_OBJECT_ALLOCATION_FAILURE)"); 
                break;
            case CL_OUT_OF_HOST_MEMORY:
                fprintf(outfile, " (CL_OUT_OF_HOST_MEMORY)"); 
                break;
            default:
                break;
        }
    }
    fprintf(outfile, "\n");
}

/* 
 * Print error message and exit. 
 * Uses 
 */
void error_exit(const char * error_message, cl_int rval)
{
    describe_error(error_message, rval, stderr);
    exit(EXIT_FAILURE);
}

/* 
 * Print information about OpenCL device to output stream(s).
 * If more_info == CL_TRUE, output is multi-line and includes various
 *   interesting-sounding things.
 * Returns EXIT_SUCCESS or EXIT_FAILURE.
 */
/* main and helper function declarations */
int output_device_info(FILE* out_stream, 
        cl_device_id device_id, cl_bool more_info);
cl_int output_device_info_uint(cl_device_id device_id, cl_device_info which, 
        FILE* out_stream, const char* fmt_string);
cl_int output_device_info_size_t(cl_device_id device_id, cl_device_info which, 
        FILE* out_stream, const char* fmt_string);
cl_int output_device_info_char(cl_device_id device_id, cl_device_info which,
        FILE* out_stream, const char* fmt_string);
/* main and helper function definitions */
int output_device_info(FILE* out_stream, 
        cl_device_id device_id, cl_bool more_info) 
{
    int return_value = EXIT_SUCCESS;
    cl_int err;
    cl_device_type device_type;

    /* basic info */
    err = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, 
            sizeof(device_type), &device_type, NULL);
    if (err == CL_SUCCESS) {
        if(device_type  == CL_DEVICE_TYPE_GPU)
            fprintf(out_stream, "GPU");
        else if (device_type == CL_DEVICE_TYPE_CPU)
            fprintf(out_stream, "CPU");
        else 
            fprintf(out_stream, "Unknown device type");
    } else {
        return_value = EXIT_FAILURE;
    }

    err = output_device_info_char(device_id, CL_DEVICE_NAME, 
            out_stream, " %s");
    if (err != CL_SUCCESS) return_value = EXIT_FAILURE;

    err = output_device_info_char(device_id, CL_DEVICE_VENDOR, 
            out_stream, " (%s)");
    if (err != CL_SUCCESS) return_value = EXIT_FAILURE;

    err = output_device_info_uint(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, 
            out_stream, ", %d compute units\n");
    if (err != CL_SUCCESS) return_value = EXIT_FAILURE;

    /* more info */
    if (more_info == CL_TRUE) {
      output_device_info_uint(device_id, CL_DEVICE_ADDRESS_BITS, 
              out_stream, "  %d address bits\n");
      output_device_info_size_t(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE,
              out_stream, "  max workgroup size %d\n");
      output_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_INT,
              out_stream, "  native vector width (int) %d\n");
      output_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG,
              out_stream, "  native vector width (long) %d\n");
      output_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT,
              out_stream, "  native vector width (float) %d\n");
      output_device_info_uint(device_id, CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE,
              out_stream, "  native vector width (double) %d\n");
      output_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
              out_stream, "  preferred vector width (int) %d\n");
      output_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
              out_stream, "  preferred vector width (long) %d\n");
      output_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
              out_stream, "  preferred vector width (float) %d\n");
      output_device_info_uint(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
              out_stream, "  preferred vector width (double) %d\n");
    }

    return return_value;
}
cl_int output_device_info_uint(cl_device_id device_id, cl_device_info which, 
        FILE* out_stream, const char* fmt_string)
{
    cl_uint info;
    cl_int err = clGetDeviceInfo(device_id, which, sizeof(info), &info, NULL);
    if (err == CL_SUCCESS) {
        fprintf(out_stream, fmt_string, info);
    }
    return err;
}
cl_int output_device_info_size_t(cl_device_id device_id, cl_device_info which, 
        FILE* out_stream, const char* fmt_string)
{
    size_t info;
    cl_int err = clGetDeviceInfo(device_id, which, sizeof(info), &info, NULL);
    if (err == CL_SUCCESS) {
        fprintf(out_stream, fmt_string, info);
    }
    return err;
}
cl_int output_device_info_char(cl_device_id device_id, cl_device_info which,
        FILE* out_stream, const char* fmt_string)
{
    cl_char info[1024] = {0};
    cl_int err = clGetDeviceInfo(device_id, which, sizeof(info), &info, NULL);
    if (err == CL_SUCCESS) {
        fprintf(out_stream, fmt_string, info);
    }
    return err;
}

/* 
 * Do standard initialization.
 * Prints error message(s) and exits program on error.
 */
void initialize(
    cl_device_type device_type,
    cl_device_id * device_id_ptr,
    cl_context * context_ptr,
    cl_command_queue * commands_ptr,
    cl_program * program_obj_ptr,
    cl_uint num_source_strings,
    const char* kernel_source[]
    )
{
    cl_int rval;

    cl_platform_id platform;
    rval = clGetPlatformIDs(1, &platform, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not get platform", rval);
    rval = clGetDeviceIDs(platform, device_type, 1, device_id_ptr, NULL);
    if (rval != CL_SUCCESS)
        error_exit("Could not get device ID", rval);
    /*
    printf("Compute device:\n");
    output_device_info(stdout, *device_id_ptr, CL_FALSE);
    */
  
    /* create a compute context */
    *context_ptr = clCreateContext(0, 1, device_id_ptr, NULL, NULL, &rval);
    if ((rval != CL_SUCCESS) || (*context_ptr == NULL))
        error_exit("Could not create compute context", rval);

    /* create a command queue */
    *commands_ptr = clCreateCommandQueue(*context_ptr, *device_id_ptr, 
            0, &rval);
    if ((rval != CL_SUCCESS) || (*commands_ptr == NULL))
        error_exit("Could not create command queue", rval);

    /* create compute program object from source buffer */
    *program_obj_ptr = clCreateProgramWithSource(*context_ptr, 
            num_source_strings, (const char**) kernel_source, NULL, &rval);
    if ((rval != CL_SUCCESS) || (*program_obj_ptr == NULL))
        error_exit("Could not create program object from source", rval);

    /* build program object */
    rval = clBuildProgram(*program_obj_ptr, 0, NULL, NULL, NULL, NULL);
    if (rval != CL_SUCCESS) {
        size_t len;
        char buffer[4096];

        describe_error("Could not build kernel executable", rval, stderr);
        clGetProgramBuildInfo(*program_obj_ptr, *device_id_ptr, 
                CL_PROGRAM_BUILD_LOG, 
                sizeof(buffer), buffer, &len);
        fprintf(stderr, "%s\n", buffer);
        exit(EXIT_FAILURE);
    }
}

/*
 * Do standard cleanup.
 */
void finalize(
    cl_context context,
    cl_command_queue commands,
    cl_program program_obj)
{
    clReleaseProgram(program_obj);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
}

/*
 * Load function(s) from file.
 * Returns C-style string (pointer to malloc'd space) or NULL.
 * Caller must free allocated string.
 */
char* source_from_file(const char* filename) {
    long file_size = filesize(filename);
    if (file_size < 0) return NULL;
    FILE* in = fopen(filename, "r");
    if (in == NULL) return NULL;
    char* data = malloc(file_size+1);
    size_t data_pos = 0;
    int input;
    while ((input = fgetc(in)) != EOF) {
        data[data_pos++] = input;
    }
    data[data_pos] = '\0';
    fclose(in);
    return data;
}

#endif /* #ifndef OPENCL_UTILITY_H_ */
