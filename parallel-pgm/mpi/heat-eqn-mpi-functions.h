#include <stdio.h>
#include "cmdline.h"

int global_index(int local_index, int num_points, int myID) {
    return local_index + myID*num_points;
}

void parse_arguments(int argc, char *argv[], int myID,
        int *nx, int *maxsteps, double *threshold, bool *print_flag,
        FILE **outfile)
{
    char *usage_msg = 
    "usage is %s points max_iterations convergence_threshold [outfilename]\n";

    *nx = get_integer_arg_extended(argc, argv, 1, 1,
            "points", usage_msg, (myID == 0),
            (void (*)(void))MPI_Finalize);
    *maxsteps = get_integer_arg_extended(argc, argv, 2, 1,
            "max_iterations", usage_msg, (myID == 0),
            (void (*)(void))MPI_Finalize);
    *threshold = get_floating_arg_extended(argc, argv, 3, 0.0,
            "convergence_threshold", usage_msg, (myID == 0),
            (void (*)(void))MPI_Finalize);
    *print_flag = false;
    if (argc > 4) {
        FILE *out = NULL;
        int out_okay = 0;
        if (myID == 0) {
            out = fopen(argv[4], "w");
            if (out == NULL) {
                fprintf(stderr, "Cannot open outfile %s\n", argv[4]);
                fprintf(stderr, usage_msg, argv[0]);
            }
            else {
                out_okay = 1;
            }
        }
        /* ugly(?) hack since only process 0 should try the fopen? */
        MPI_Bcast(&out_okay, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (out_okay == 0) {
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }
        *print_flag = true;
        *outfile = out;
    } 
    else {
        *outfile = NULL;
    }
}

void print_value(FILE *outfile, int index, double value) {
	fprintf(outfile, "uk[%010d] = %14.10f\n", index, value);
}
void print_values(FILE *outfile, double uk[], int num_points, int num_procs, 
        int myID) 
{
    MPI_Status status; 
    if (myID == 0) {

        /* left endpoint */
        print_value(outfile, global_index(0, num_points, 0), 
            LEFTVAL);

        /* loop over all processes */
        for (int p = 0; p < num_procs; ++p) {
            if (p != 0) {
                /* get data from next process */
                MPI_Recv(&uk[1], num_points, MPI_DOUBLE, p, 0, MPI_COMM_WORLD, &status);
            }
            /* print */
            for (int i = 1; i <= num_points; ++i) {
                int global_i = global_index(i, num_points, p);
                print_value(outfile, global_i, uk[i]);
            }
        }

        /* right endpoint */
        print_value(outfile, global_index(num_points+1, num_points, num_procs-1), 
            RIGHTVAL);
    }
    else {
        /* send data to process 0 */
        MPI_Send(&uk[1], num_points, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
}

