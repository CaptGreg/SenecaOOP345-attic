/*
 * "hello world" program with C++11 threads:
 *
 * environment variable NUM_THREADS gives number of threads.
 */
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>

#include "cmdline.h"

void thread_fcn(int myID);

/* ---- main program ---- */

int main(int argc, char* argv[]) {

    int num_threads = 
        get_integer_environment("NUM_THREADS", 1, "number of threads");

    std::vector<std::thread> threads;

    printf("hello from main program\n");

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(thread_fcn, i));
    }
    printf("threads launched\n");

    for (auto& t : threads) {
        t.join();
    }

    printf("threads all done\n");

    return EXIT_SUCCESS;
}

/* ---- code to be executed by each thread ---- */

void thread_fcn(int myID) {
    printf("hello from thread %d\n", myID);
}
