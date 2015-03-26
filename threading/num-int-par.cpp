/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * parallel version using C++11 threads.
 *
 * environment variable NUM_THREADS gives number of threads.
 */
#include <cstdio>
#include <cstdlib>
#include <cmath>
/* copied from not-strictly-standard part of math.h */
#define M_PI		3.14159265358979323846
#include <unistd.h>

#include <thread>
#include <vector>
#include <mutex>

#include "timer.h"          /* has get_time() */
#include "cmdline.h"

#define NUM_STEPS 400000000 

void thread_fcn(int my_id, int num_threads, double step, 
        double *sum_p, std::mutex *lock_p);

/* ---- main program ---- */

int main(int argc, char *argv[]) {

    double start_time, end_time;
    double pi;
    double sum = 0.0;
    double step = 1.0/(double) NUM_STEPS; 

    int num_threads = 
        get_integer_environment("NUM_THREADS", 1, "number of threads");

    std::mutex lock;

    /* record start time */
    start_time = get_time();

    /* create and start threads */
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(
                    thread_fcn, i, num_threads, step, &sum, &lock));
    }
    /* wait for them to complete */
    for (auto& t : threads) {
        t.join();
    }

    /* finish computation */
    pi = step * sum;

    /* record end time */
    end_time = get_time();

    /* print results */
    printf("parallel program results with %d threads:\n", num_threads);
    printf("computed pi = %g  (%17.15f)\n",pi, pi);
    printf("difference between computed pi and math.h M_PI = %17.15f\n", 
            fabs(pi - M_PI));
    printf("time to compute = %g seconds\n", end_time - start_time);

    return EXIT_SUCCESS;
}

/* ---- code to be executed by each thread ---- */

void thread_fcn(int my_id, int num_threads, double step, 
        double *sum_p, std::mutex *lock_p)
{
    double x;
    double part_sum = 0.0;

    /* do this thread's part of computation */
    for (int i=my_id; i < NUM_STEPS; i += num_threads) { 
        x = (i+0.5)*step;
        part_sum += 4.0/(1.0+x*x); 
    }
    /* add partial sum to global sum */
    lock_p->lock();
    *sum_p += part_sum;
    lock_p->unlock();
}
