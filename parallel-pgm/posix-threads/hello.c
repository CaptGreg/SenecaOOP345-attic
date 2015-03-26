/*
 * "hello world" program:
 *
 * environment variable NUM_THREADS gives number of threads.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "cmdline.h"

void * thread_fcn(void * thread_arg);

/* ---- main program ---- */

int main(int argc, char* argv[]) {

    int * threadIDs;
    pthread_t * threads;

    int num_threads = 
        get_integer_environment("NUM_THREADS", 1, "number of threads");

    /* 
     * Set up IDs for threads (need a separate variable for each since they're
     *   shared among threads).
     */
    threadIDs = malloc(num_threads * sizeof(*threadIDs));
    for (int i = 0; i < num_threads; ++i)
        threadIDs[i] = i;

    /* Start num_threads new threads, each with different ID. */
    threads = malloc(num_threads * sizeof(*threads));
    for (int i = 0; i < num_threads; ++i) 
        pthread_create(&threads[i], NULL, thread_fcn, (void *) &threadIDs[i]);

    printf("all threads launched\n");

    /* Wait for all threads to complete. */
    for (int i = 0; i < num_threads; ++i) 
        pthread_join(threads[i], NULL);

    printf("all threads complete\n");

    /* Clean up and exit. */
    free(threadIDs);
    free(threads);
    return EXIT_SUCCESS;
}

/* ---- code to be executed by each thread ---- */

void * thread_fcn(void * thread_arg) {
    int myID = * (int *) thread_arg;

    fprintf(stdout, "hello, world, from thread %d\n", myID);

    pthread_exit((void* ) NULL);
}
