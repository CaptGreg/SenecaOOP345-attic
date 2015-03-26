#include <stdio.h>
#include "cmdline.h"

void parse_arguments(int argc, char *argv[], 
        int *nx, int *maxsteps, float *threshold, 
        int *points_per_workitem, int *workgroup_size_factor,
        FILE **outfile, cl_device_type *device_type)
{
    char *usage_msg = 
    "usage is"
    "\n %s points max_iterations convergence_threshold"
    "\n  points_per_workitem workgroup_size_factor [\"CPU\"]"
    "\n  [outfilename]"
    "\n";

    *nx = get_integer_arg(argc, argv, 1, 1, 
            "points", usage_msg);
    *maxsteps = get_integer_arg(argc, argv, 2, 1, 
            "max_iterations", usage_msg);
    *threshold = get_floating_arg(argc, argv, 3, 0.0, 
            "convergence_threshold", usage_msg);
    *points_per_workitem = get_integer_arg(argc, argv, 4, 1, 
            "points_per_workitem", usage_msg);
    *workgroup_size_factor = get_integer_arg(argc, argv, 5, 1, 
            "workgroup_size_factor", usage_msg);
    *outfile = NULL;
    *device_type = CL_DEVICE_TYPE_GPU;
    int required_parms = 5;
    if (argc > required_parms+1) {
        if (strcmp(argv[required_parms+1], "CPU") == 0) {
            *device_type = CL_DEVICE_TYPE_CPU;
        }
        else {
            FILE* out = fopen(argv[required_parms+1], "w");
            if (out == NULL) {
                fprintf(stderr, "Cannot open outfile %s\n", 
                        argv[required_parms+1]);
                fprintf(stderr, usage_msg, argv[0]);
                exit(EXIT_FAILURE);
            }
            *outfile = out;
        }
    }
    if (argc > required_parms+2) {
        if (strcmp(argv[required_parms+2], "CPU") == 0) {
            *device_type = CL_DEVICE_TYPE_CPU;
        }
        else {
            FILE* out = fopen(argv[required_parms+2], "w");
            if (out == NULL) {
                fprintf(stderr, "Cannot open outfile %s\n", 
                        argv[required_parms+2]);
                fprintf(stderr, usage_msg, argv[0]);
                exit(EXIT_FAILURE);
            }
            *outfile = out;
        }
    }
    /* FIXME?  check for duplicate optional parameters? */
}

void initialize_values(float uk[], int nx) {
    uk[0] = LEFTVAL; uk[nx+1] = RIGHTVAL;
    for (int i = 1; i <= nx; ++i)
        uk[i] = 0.0f;
}
