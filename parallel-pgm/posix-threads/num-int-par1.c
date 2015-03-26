/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * parallel version using POSIX threads.
 *
 * environment variable NUM_THREADS gives number of threads.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* copied from not-strictly-standard part of math.h */
#define M_PI		3.14159265358979323846
#include <pthread.h>

#include "timer.h" 
#include "cmdline.h"

#define NUM_STEPS 400000000 

void * thread_fcn(void * thread_arg);

/* ---- global variables (ugly but simple) ---- */

/* ("static" limits scope to this file) */
static int num_threads;
static double sum, step;
static pthread_mutex_t lock; 

/* ---- main program ---- */

int main(int argc, char *argv[]) {

    double start_time, end_time;
    double pi;

    num_threads = 
        get_integer_environment("NUM_THREADS", 1, "number of threads");

    int threadIDs[num_threads];
    pthread_t threads[num_threads];

    sum = 0.0;
    step = 1.0/(double) NUM_STEPS; 

    /* record start time */
    start_time = get_time();

    /* initialize lock */
    pthread_mutex_init(&lock, NULL);

    /* set up IDs for threads */
    for (int i = 0; i < num_threads; ++i)
        threadIDs[i] = i;

    /* start threads */
    for (int i = 0; i < num_threads; ++i) 
        pthread_create(&threads[i], NULL, thread_fcn, (void *) &threadIDs[i]);

    /* wait for all threads to complete */
    for (int i = 0; i < num_threads; ++i) 
        pthread_join(threads[i], NULL);

    /* clean up */
    pthread_mutex_destroy(&lock);

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

void * thread_fcn(void * thread_arg) {
    int myID = * (int *) thread_arg;

    double x;
    double part_sum = 0.0;

    /* do this thread's part of computation */
    for (int i=myID; i < NUM_STEPS; i += num_threads) { 
        x = (i+0.5)*step;
        part_sum += 4.0/(1.0+x*x); 
    }
    /* add partial sum to global sum */
    pthread_mutex_lock(&lock);  
    sum += part_sum;
    pthread_mutex_unlock(&lock);  

    pthread_exit((void* ) NULL);
}
