/*
 * heat diffusion example, as discussed in textbook.
 *
 * command line arguments are number of points, maximum number of
 *   iterations, convergence threshold, and optional filename
 *   to print final values (values are not printed if no filename
 *   is given).
 *
 * parallel version with OpenMP, simple approach.
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

void initialize(double uk[], double ukp1[], int nx);

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

    #pragma omp parallel 
    {
        #pragma omp single 
        {
            nthreads = omp_get_num_threads();
        }
    }

    initialize(uk, ukp1, nx);

    for (step = 0; (step < maxsteps) && (maxdiff >= threshold); ++step) {

        maxdiff = 0.0;
        #pragma omp parallel 
        {
	    /* compute new values and check for convergence */
            double maxdiff_local = 0.0;
            double diff;
            #pragma omp for schedule(static)
            for (int i = 1; i <= nx; ++i) {
                ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
                diff = fabs(uk[i] - ukp1[i]);
                if (diff > maxdiff_local) maxdiff_local = diff;
            }
            #pragma omp critical
            {
                if (maxdiff_local > maxdiff) maxdiff = maxdiff_local;
            }
        }

        /* "copy" ukp1 to uk by swapping pointers */
        temp = ukp1; ukp1 = uk; uk = temp;
    }

    end_time = get_time();

    if (outfile != NULL) {
        print_values(outfile, uk, nx);
        fclose(outfile);
    }

    printf("OpenMP program v2 (%d threads):\n", nthreads);
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


void initialize(double uk[], double ukp1[], int nx) {
    uk[0] = ukp1[0] = LEFTVAL; 
    uk[nx+1] = ukp1[nx+1] = RIGHTVAL;
    #pragma omp parallel for schedule(static)
    for (int i = 1; i <= nx; ++i)
        uk[i] = ukp1[i] = 0.0;
}

