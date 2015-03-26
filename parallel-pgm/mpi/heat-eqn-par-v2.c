/*
 * heat diffusion example, as discussed in textbook.
 *
 * command line arguments are number of points, maximum number of
 *   iterations, convergence threshold, and optional filename
 *   to print final values (values are not printed if no filename
 *   is given).
 *
 * parallel version with MPI and improved output.
 *
 * version 2, combining loops where feasible.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

#include "timer.h"
#include "cmdline.h"

#define LEFTVAL 1.0
#define RIGHTVAL 10.0

#include "heat-eqn-mpi-functions.h"

void initialize(double uk[], double ukp1[], int num_points, int num_procs, 
        int myID);

int main(int argc, char *argv[]) {

    int nx;
    int maxsteps;
    double threshold;
    bool print_flag;
    FILE *outfile;

    double *uk;
    double *ukp1;
    double *temp;
    double dx, dt;
    double start_time, end_time;
    double maxdiff, maxdiff_local, diff;
    int step;

    int num_procs, myID, left_nbr, right_nbr;
    int num_points;
    MPI_Status status; 

    /* MPI initialization */
    MPI_Init(&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);

    parse_arguments(argc, argv, myID, &nx, &maxsteps, &threshold, &print_flag,
            &outfile);

    MPI_Barrier(MPI_COMM_WORLD);
    /* could use MPI_Wtime but get_time is what sequential code uses */
    start_time = get_time();

    dx = 1.0/nx;
    dt = 0.5*dx*dx;
    maxdiff = threshold;

    if ((nx % num_procs) != 0) {
        if (myID == 0) {
            fprintf(stderr, "Number of processes must evenly divide %d\n", nx);
        }
        MPI_Finalize(); exit(EXIT_FAILURE);
    }

    left_nbr = myID - 1; /* ID of left "neighbor" process */
    right_nbr = myID + 1; /* ID of right "neighbor" process */
    num_points = (nx / num_procs);
    /* uk, ukp1 include a "ghost cell" at each end */
    uk = malloc(sizeof(*uk) * (num_points+2));
    ukp1 = malloc(sizeof(*ukp1) * (num_points+2));
    if ((uk == NULL) || (ukp1 == NULL)) {
        fprintf(stderr, "Unable to allocate memory\n");
        /* MPI_Abort here since we could fail in only one process */
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    initialize(uk, ukp1, num_points, num_procs, myID);

    for (step = 0; (step < maxsteps) && (maxdiff >= threshold); ++step) {

        /* exchange boundary information */
        if (myID != 0) 
            MPI_Send(&uk[1], 1, MPI_DOUBLE, left_nbr, 0, MPI_COMM_WORLD);
        if (myID != num_procs-1)
            MPI_Send(&uk[num_points], 1, MPI_DOUBLE, right_nbr, 0, MPI_COMM_WORLD);
        if (myID != 0)
            MPI_Recv(&uk[0], 1, MPI_DOUBLE, left_nbr, 0, MPI_COMM_WORLD, &status);
        if (myID != num_procs-1)
            MPI_Recv(&uk[num_points+1],1, MPI_DOUBLE, right_nbr, 0, MPI_COMM_WORLD,
                    &status);

        /* compute new values and check for convergence */
        maxdiff_local = 0.0;
        for (int i = 1; i <= num_points; ++i) {
            ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
            diff = fabs(uk[i] - ukp1[i]);
            if (diff > maxdiff_local) maxdiff_local = diff;
        }
        MPI_Allreduce(&maxdiff_local, &maxdiff, 1, MPI_DOUBLE, MPI_MAX,
                MPI_COMM_WORLD);

        /* "copy" ukp1 to uk by swapping pointers */
        temp = ukp1; ukp1 = uk; uk = temp;
    }

    MPI_Barrier(MPI_COMM_WORLD); /* sloppy -- to get more meaningful timing */
    end_time = get_time();

    if (print_flag) {
        print_values(outfile, uk, num_points, num_procs, myID);
        if (myID == 0) {
            fclose(outfile);
        }
    }

    if (myID == 0) {
        printf("MPI program v2 (%d processes):\n", num_procs);
        printf("nx = %d, maxsteps = %d, threshold = %g\n", 
                nx, maxsteps, threshold);
        if (maxdiff < threshold) {
            printf("converged in %d iterations\n", step);
        }
        else {
            printf("failed to converge in %d iterations, maxdiff = %g\n", 
                    step, maxdiff);
        }
        printf("execution time = %g\n", end_time - start_time);
    }
 
    /* clean up and end */
    free(uk);
    free(ukp1);
    MPI_Finalize();
    return EXIT_SUCCESS;
}

void initialize(double uk[], double ukp1[], int num_points, 
        int num_procs, int myID) 
{
    /* uk, ukp1 include "ghost cell" at each end */
    for (int i = 1; i <= num_points; ++i)
        uk[i] = ukp1[i] = 0.0;
    /* left endpoint */
    if (myID == 0)
        uk[0] = ukp1[0] = LEFTVAL;
    /* right endpoint */
    if (myID == num_procs-1)
        uk[num_points+1] = ukp1[num_points+1] = RIGHTVAL;
} 


