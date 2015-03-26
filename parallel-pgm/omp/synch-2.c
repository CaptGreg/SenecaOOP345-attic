/*
 * simple synchronization example, version 2:
 *
 * synchronize access to a resource (here, standard output) using
 * OpenMP construct for "critical section".
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         /* has sleep() */
#include <omp.h>            /* OpenMP header file */

int main(void) {
    #pragma omp parallel 
    {
        #pragma omp critical 
        {
            /* run this code in only one thread at a time */
            printf("hello, world ");
            sleep(1);
            printf("from thread %d\n", omp_get_thread_num());
        }
    }
    return EXIT_SUCCESS;
}
