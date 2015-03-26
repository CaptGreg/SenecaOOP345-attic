/*
 * simple synchronization example, version 2:
 *
 * synchronize access to a resource (here, standard output) using mutex
 * (lock).
 *
 * environment variable NUM_THREADS gives number of threads.
 */
#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>
#include <mutex>

#include "cmdline.h"

void thread_fcn(int thread_id, int num_threads);

/* ---- global variables (ugly but simple) ---- */

/* (static keyword limits scope to this file) */
static std::mutex print_lock;

/* ---- main program ---- */

int main(int argc, char* argv[]) {

    int num_threads = 
        get_integer_environment("NUM_THREADS", 1, "number of threads");

    std::vector<std::thread> threads;

    std::cout << "hello from main program\n";

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(thread_fcn, i, num_threads));
    }
    print_lock.lock();
    std::cout << "threads launched\n";
    print_lock.unlock();

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "threads all done\n";

    return EXIT_SUCCESS;
}

/* ---- code to be executed by each thread ---- */

void thread_fcn(int thread_id, int num_threads) {
    print_lock.lock();
    std::cout << 
        "hello from thread " << thread_id << " of " << num_threads << 
        " (id " << std::this_thread::get_id() << ")\n";
    print_lock.unlock();
}
