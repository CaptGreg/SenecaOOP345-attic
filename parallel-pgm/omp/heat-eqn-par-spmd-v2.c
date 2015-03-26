/*
 * heat diffusion example, as discussed in textbook.
 *
 * command line arguments are number of points, maximum number of
 *   iterations, convergence threshold, and optional filename
 *   to print final values (values are not printed if no filename
 *   is given).
 *
 * parallel version with OpenMP, more complex SPMD-like approach.
 *
 * version 2, combining loops where feasible.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

#include "timer.h"
#include "cmdline.h"

#define LEFTVAL 1.0
#define RIGHTVAL 10.0

#include "heat-eqn-functions.h"

void initialize_end_points(double uk[], double ukp1[], int nx);
void initialize_local_section(double uk[], double ukp1[], 
        int loop_start, int loop_limit);

int main(int argc, char *argv[]) {

    int nx;
    int maxsteps;
    double threshold;
    FILE* outfile; /* NULL if output values are not to be printed */

    double *uk; 
    double *ukp1;
    double *temp; 
    double dx, dt;
    double start_time, end_time;
    double maxdiff;
    int step;

    int nthreads;

    parse_arguments(argc, argv, &nx, &maxsteps, &threshold, &outfile);

    /* could use omp_get_wtime but get_time is what sequential code uses */
    start_time = get_time();

    #pragma omp parallel 
    {
        #pragma omp single 
        {
            nthreads = omp_get_num_threads();
        }
    }
    if ((nx % nthreads) != 0) {
        fprintf(stderr, "Number of threads must evenly divide %d\n", nx);
        exit(EXIT_FAILURE);
    }

    /* uk, ukp1 have nx interior points plus fixed ends */
    uk = malloc(sizeof(*uk) * (nx+2)); 
    ukp1 = malloc(sizeof(*ukp1) * (nx+2));
    if ((uk == NULL) || (ukp1 == NULL)) {
        fprintf(stderr, "Unable to allocate memory\n");
        return EXIT_FAILURE;
    }
    dx = 1.0/nx;
    dt = 0.5*dx*dx;
    maxdiff = threshold;

    /* initialize(uk, ukp1, nx); */
    initialize_end_points(uk, ukp1, nx);
    /* remainder of initialize in parallel section */

    #pragma omp parallel
    {
        int step_local; 
        double maxdiff_local;
        double diff;

        int my_id = omp_get_thread_num();
        int loop_start = 1 + my_id * (nx/nthreads);
        int loop_limit = loop_start + (nx/nthreads);
        maxdiff = threshold;

        initialize_local_section(uk, ukp1, loop_start, loop_limit);
        #pragma omp barrier

        for (step_local = 0; (step_local < maxsteps) && (maxdiff >= threshold);
                ++step_local) 
        {
            #pragma omp barrier
            #pragma omp single
            {
                maxdiff = 0.0;
            }
 
            /* compute new values and check for convergence */
            maxdiff_local = 0.0;
            /* for (int i = 1; i < nx-1; ++i) */
            for (int i = loop_start; i < loop_limit; ++i) {
                ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
                diff = fabs(uk[i] - ukp1[i]);
                if (diff > maxdiff_local) maxdiff_local = diff;
            }
            #pragma omp critical
            {
                if (maxdiff_local > maxdiff) maxdiff = maxdiff_local;
            }

            /* "copy" ukp1 to uk by swapping pointers */
            #pragma omp barrier
            #pragma omp single
            {
                temp = ukp1; ukp1 = uk; uk = temp;
            }
        }
        /* save count of steps in shared variable */
        #pragma omp single
        {
            step = step_local;
        }
    }

    end_time = get_time();

    if (outfile != NULL) {
        print_values(outfile, uk, nx);
        fclose(outfile);
    }

    printf("OpenMP program, SPMD-style v2 (%d threads):\n", nthreads);
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
    free(ukp1);
    return EXIT_SUCCESS;
}

void initialize_end_points(double uk[], double ukp1[], int nx) {
    uk[0] = ukp1[0] = LEFTVAL; 
    uk[nx+1] = ukp1[nx+1] = RIGHTVAL;
}

void initialize_local_section(double uk[], double ukp1[], 
        int loop_start, int loop_limit) 
{
    for (int i = loop_start; i < loop_limit; ++i) {
        uk[i] = ukp1[i] = 0.0;
    }
}

