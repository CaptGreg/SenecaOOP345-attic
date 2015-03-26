/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * sequential version that tries to produce a better result than the simple
 * approach with single-precision using the algorithm described in 
 * http://en.wikipedia.org/wiki/Kahan_summation_algorithm
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* copied from not-strictly-standard part of math.h */
#define M_PI		3.14159265358979323846
#include <float.h>
#include "timer.h"

#define NUM_STEPS 400000000

/* main program */
int main(int argc, char *argv[]) {

    double start_time, end_time;
    float x, pi;
    float sum;
    float step = 1.0f/(float)NUM_STEPS; 

    /* record start time */
    start_time = get_time();

    /* do computation */
    sum = 0.0f;
    float c = 0.0f;     /* error correction */
    float sum_input;
    float y, t;
    for (int i=0; i < NUM_STEPS; ++i) {
        x = (i+0.5f)*step;
        sum_input = 4.0f/(1.0f+x*x);
        y = sum_input - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    pi = step * sum;

    /* record end time */
    end_time = get_time();

    /* print results */
    printf("sequential program (single-precision, Kahan version)\n");
    printf("results with %d steps:\n", NUM_STEPS);
    printf("computed pi = %g  (%.15f)\n", pi, pi);
    printf("difference between computed pi and math.h M_PI = %.15f\n", 
            fabs(pi - M_PI));
    printf("time to compute = %g seconds\n", end_time - start_time);

    return EXIT_SUCCESS;
}
