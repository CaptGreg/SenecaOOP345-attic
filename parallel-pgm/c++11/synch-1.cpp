/*
 * simple synchronization example, version 1:
 *
 * unsynchronized access to a resource (here, standard output) can lead to
 * problems.
 *
 * environment variable NUM_THREADS gives number of threads.
 */
#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>

#include "cmdline.h"

void thread_fcn(int thread_id, int num_threads);

/* ---- main program ---- */

int main(int argc, char* argv[]) {

    int num_threads = 
        get_integer_environment("NUM_THREADS", 1, "number of threads");

    std::vector<std::thread> threads;

    std::cout << "hello from main program\n";

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(thread_fcn, i, num_threads));
    }
    std::cout << "threads launched\n";

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "threads all done\n";

    return EXIT_SUCCESS;
}

/* ---- code to be executed by each thread ---- */

void thread_fcn(int thread_id, int num_threads) {
    std::cout << 
        "hello from thread " << thread_id << " of " << num_threads << 
        " (id " << std::this_thread::get_id() << ")\n";
}
