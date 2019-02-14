/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * sequential version.
 */
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

#include "timer.h"

#define M_PI		3.14159265358979323846 // copied from not-strictly-standard part of math.h

#define NUM_STEPS 400000000

/* main program */
int main(int argc, char *argv[]) {

    Timer timer;

    double x, pi;
    double sum = 0.0;
    double step = 1.0/(double) NUM_STEPS; 

    timer.Start();

    /* do computation */
    for (int i=0; i < NUM_STEPS; ++i) {
        x = (i+0.5)*step;
        sum += 4.0/(1.0+x*x);
    }
    pi = step * sum;

    timer.Stop();

    /* print results */
    cout << "sequential program results with " << NUM_STEPS << " steps:\n";
    cout << "computed pi = " << pi << "\n";
    cout << "difference between computed pi and math.h M_PI = " << fabs(pi - M_PI) << "\n";
    cout << "time to compute = " << timer.millisecs() << " milliseconds.\n";

    return EXIT_SUCCESS;
}
