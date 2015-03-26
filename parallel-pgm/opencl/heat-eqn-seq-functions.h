#include <stdio.h>
#include "cmdline.h"

void parse_arguments(int argc, char *argv[], 
        int *nx, int *maxsteps, float *threshold, FILE **outfile) 
{
    char *usage_msg = 
    "usage is %s points max_iterations convergence_threshold [outfilename]\n";

    *nx = get_integer_arg(argc, argv, 1, 1, "points", usage_msg);
    *maxsteps = get_integer_arg(argc, argv, 2, 1, "max_iterations", usage_msg);
    *threshold = get_floating_arg(argc, argv, 3, 0.0, "convergence_threshold",
            usage_msg);
    int required_parms=3;
    if (argc > required_parms+1) {
        FILE* out = fopen(argv[required_parms+1], "w");
        if (out == NULL) {
            fprintf(stderr, "Cannot open outfile %s\n", argv[required_parms+1]);
            fprintf(stderr, usage_msg, argv[0]);
            exit(EXIT_FAILURE);
        }
        *outfile = out;
    } 
    else {
        *outfile = NULL;
    }
}
