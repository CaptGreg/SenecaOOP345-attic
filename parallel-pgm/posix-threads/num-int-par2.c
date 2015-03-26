/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * parallel version using POSIX threads, "improved" version with no
 * global variables.
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

/* ---- data structure for thread arguments ---- */

typedef struct thread_args {
    int myID;
    int num_threads;
    double step;
    double *sum_p;
    pthread_mutex_t *lock_p; 
} thread_args_t;

/* ---- main program ---- */

int main(int argc, char *argv[]) {

    double start_time, end_time;
    double pi;
    double sum = 0.0;
    double step = 1.0/(double) NUM_STEPS; 

    int num_threads =
        get_integer_environment("NUM_THREADS", 1, "number of threads");

    thread_args_t thread_args[num_threads];
    pthread_t threads[num_threads];

    pthread_mutex_t lock; 

    /* record start time */
    start_time = get_time();

    /* initialize lock */
    pthread_mutex_init(&lock, NULL);

    /* set up parameters for threads */
    for (int i = 0; i < num_threads; ++i) {
        thread_args_t * args_p = &thread_args[i];
        args_p->myID = i;
        args_p->num_threads = num_threads;
        args_p->step = step;
        args_p->sum_p = &sum;
        args_p->lock_p = &lock;
    }

    /* start threads */
    for (int i = 0; i < num_threads; ++i) 
        pthread_create(&threads[i], NULL, thread_fcn, (void *) &thread_args[i]);

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
    thread_args_t * args_p = (thread_args_t *) thread_arg;

    double x;
    double part_sum = 0.0;

    /* do this thread's part of computation */
    for (int i=args_p->myID; i < NUM_STEPS; i += args_p->num_threads) { 
        x = (i+0.5)*args_p->step;
        part_sum += 4.0/(1.0+x*x); 
    }
    /* add partial sum to global sum */
    pthread_mutex_lock(args_p->lock_p);
    *(args_p->sum_p) += part_sum;
    pthread_mutex_unlock(args_p->lock_p);

    pthread_exit((void* ) NULL);
}
