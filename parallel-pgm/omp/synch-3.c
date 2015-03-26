/*
 * simple synchronization example, version 3:
 *
 * synchronize access to a resource (here, standard output) using
 * OpenMP locks.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         /* has sleep() */
#include <omp.h>            /* OpenMP header file */

int main(void) {

    /* define and initialize lock */
    omp_lock_t printlock;
    omp_init_lock(&printlock);

    #pragma omp parallel 
    {
        /* acquire lock (wait if not available) */
        omp_set_lock(&printlock);
        /* only one thread at a time executes this code */
        printf("hello, world ");
        sleep(1);
        printf("from thread %d\n", omp_get_thread_num());
        /* release lock */
        omp_unset_lock(&printlock);
    }
    /* clean up */
    omp_destroy_lock(&printlock);
    return EXIT_SUCCESS;
}
