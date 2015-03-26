/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * parallel version using OpenMP, as discussed in Appendix A.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* copied from not-strictly-standard part of math.h */
#define M_PI		3.14159265358979323846
#include <omp.h>		/* OpenMP header file */

#include "timer.h"

#define NUM_STEPS 400000000

/* main program */
int main(int argc, char *argv[]) {

    double start_time, end_time;
    double x, pi;
    double sum = 0.0;
    double step = 1.0/(double) NUM_STEPS;
    int nthreads;

    /* record start time */
    /* could use omp_get_wtime but get_time is what sequential code uses */
    start_time = get_time();

    /* do computation -- using all available threads */
    #pragma omp parallel
    {
        #pragma omp master
        {
            nthreads = omp_get_num_threads();
        }
        #pragma omp for private(x) reduction(+:sum) schedule(static)
        for (int i=0; i < NUM_STEPS; ++i) {
            x = (i+0.5)*step;
            sum += 4.0/(1.0+x*x);
        }
        #pragma omp master
        {
            pi = step * sum;
        }
    }

    /* record end time */
    end_time = get_time();

    /* print results */
    printf("parallel program results with %d threads and %d steps:\n", 
            nthreads, NUM_STEPS);
    printf("computed pi = %g  (%17.15f)\n",pi, pi);
    printf("difference between computed pi and math.h M_PI = %17.15f\n", 
            fabs(pi - M_PI));
    printf("time to compute = %g seconds\n", end_time - start_time);

    return EXIT_SUCCESS;
}
