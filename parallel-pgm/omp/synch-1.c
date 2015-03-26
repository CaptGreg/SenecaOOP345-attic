/*
 * simple synchronization example, version 1:
 *
 * unsynchronized access to a resource (here, standard output) can lead to
 * problems.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         /* has sleep() */
#include <omp.h>            /* OpenMP header file */

int main(void) {
    #pragma omp parallel 
    {
        /* run this code in all threads without synchronization */
        printf("hello, world ");
        sleep(1);
        printf("from thread %d\n", omp_get_thread_num());
    }
    return EXIT_SUCCESS;
}
