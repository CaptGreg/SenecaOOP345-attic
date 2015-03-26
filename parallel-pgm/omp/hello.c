/*
 * "hello world" program
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>            /* OpenMP header file */

int main(void) {
    printf("starting program\n");
    #pragma omp parallel 
    {
        /* run this in each thread */
        printf("hello, world, from thread %d\n", omp_get_thread_num());
    }
    printf("all done\n");
    return EXIT_SUCCESS;
}
