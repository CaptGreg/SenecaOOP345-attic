/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * parallel version using OpenMP with approach taken in MPI program ("SPMD")
 * and local variables for partial sums.  
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
    double pi;
    double sum = 0.0;
    double step = 1.0/(double) NUM_STEPS; 
    int nthreads;

    /* record start time */
    /* could use omp_get_wtime but get_time is what sequential code uses */
    start_time = get_time();

    /* do computation -- using all available threads */
    #pragma omp parallel 
    {
        /* local variables -- or declare in main and make private */
        double x; 
        int myid = omp_get_thread_num();
        double partsum = 0.0;

        /* get number of threads */
        #pragma omp master 
        {
            nthreads = omp_get_num_threads();
        }
        #pragma omp barrier

        for (int i=myid; i < NUM_STEPS; i += nthreads) {
            x = (i+0.5)*step;
            partsum += 4.0/(1.0+x*x);
        }

        /* sum partial results */
        #pragma omp critical
        {
            sum += partsum;
        }
        #pragma omp barrier

        /* compute final result */
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
