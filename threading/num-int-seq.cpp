/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * sequential version.
 */
#include <cstdio>
#include <cstdlib>
#include <cmath>
/* copied from not-strictly-standard part of math.h */
#define M_PI		3.14159265358979323846

#include "timer.h"

#define NUM_STEPS 400000000

/* main program */
int main(int argc, char *argv[]) {

    double start_time, end_time;
    double x, pi;
    double sum = 0.0;
    double step = 1.0/(double) NUM_STEPS; 

    /* record start time */
    start_time = get_time();

    /* do computation */
    for (int i=0; i < NUM_STEPS; ++i) {
        x = (i+0.5)*step;
        sum += 4.0/(1.0+x*x);
    }
    pi = step * sum;

    /* record end time */
    end_time = get_time();

    /* print results */
    printf("sequential program results with %d steps:\n", 
            NUM_STEPS);
    printf("computed pi = %g  (%17.15f)\n",pi, pi);
    printf("difference between computed pi and math.h M_PI = %17.15f\n", 
            fabs(pi - M_PI));
    printf("time to compute = %g seconds\n", end_time - start_time);

    return EXIT_SUCCESS;
}
