/*
 * heat diffusion example, as discussed in textbook.
 *
 * command line arguments are number of points, maximum number of
 *   iterations, convergence threshold, and optional filename
 *   to print final values (values are not printed if no filename
 *   is given).
 *
 * sequential version.
 *
 * version 2, combining loops where feasible.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
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
    double maxdiff, diff;
    int step;

    parse_arguments(argc, argv, &nx, &maxsteps, &threshold, &outfile);

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

    initialize(uk, ukp1, nx);

    for (step = 0; (step < maxsteps) && (maxdiff >= threshold); ++step) {

        /* compute new values and check for convergence */
        maxdiff = 0.0;
        for (int i = 1; i <= nx; ++i) {
            ukp1[i]=uk[i]+ (dt/(dx*dx))*(uk[i+1]-2*uk[i]+uk[i-1]);
            diff = fabs(uk[i] - ukp1[i]);
            if (diff > maxdiff) maxdiff = diff;
        }

        /* "copy" ukp1 to uk by swapping pointers */
        temp = ukp1; ukp1 = uk; uk = temp;
    }

    end_time = get_time();

    if (outfile != NULL) {
        print_values(outfile, uk, nx);
        fclose(outfile);
    }

    printf("sequential program v2:\n");
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
    for (int i = 1; i <= nx; ++i)
        uk[i] = ukp1[i] = 0.0;
}
