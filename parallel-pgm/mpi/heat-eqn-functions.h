#include <stdio.h>
#include "cmdline.h"

void parse_arguments(int argc, char *argv[], 
        int *nx, int *maxsteps, double *threshold, FILE **outfile) 
{
    char *usage_msg = 
    "usage is %s points max_iterations convergence_threshold [outfilename]\n";

    *nx = get_integer_arg(argc, argv, 1, 1, "points", usage_msg);
    *maxsteps = get_integer_arg(argc, argv, 2, 1, "max_iterations", usage_msg);
    *threshold = get_floating_arg(argc, argv, 3, 0.0, "convergence_threshold",
            usage_msg);
    if (argc > 4) {
        FILE* out = fopen(argv[4], "w");
        if (out == NULL) {
            fprintf(stderr, "Cannot open outfile %s\n", argv[4]);
            fprintf(stderr, usage_msg, argv[0]);
            exit(EXIT_FAILURE);
        }
        *outfile = out;
    } 
    else {
        *outfile = NULL;
    }
}

void print_values(FILE *outfile, double uk[], int nx) {
    for (int i = 0; i < nx+2; ++i) {
        fprintf(outfile, "uk[%010d] = %14.10f\n", i, uk[i]);
    }
}
