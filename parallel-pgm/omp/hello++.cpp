/*
 * "hello world" program, C++ version
 */
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <omp.h>            /* OpenMP header file */

int main(void) {
    std::cout << "starting program\n";
    #pragma omp parallel 
    {
        /* run this in each thread */
        std::ostringstream msg;
        msg << "hello, world, from thread " 
            << omp_get_thread_num() 
            << std::endl;
        std::cout << msg.str();
    }
    std::cout << "all done\n";
    return EXIT_SUCCESS;
}
