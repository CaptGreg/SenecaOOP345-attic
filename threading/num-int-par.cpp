/*
 * numerical integration example, as discussed in textbook:  
 *
 * compute pi by approximating the area under the curve f(x) = 4 / (1 + x*x)
 * between 0 and 1.
 *
 * parallel version using C++11 threads.
 *
 * environment variable NUM_THREADS gives number of threads.
 */
#include <iostream>
#include <cmath>
// #define M_PI		3.14159265358979323846 //  copied from not-strictly-standard part of math.h

#include <thread>
#include <vector>
#include <mutex>

#include "timer.h"

using namespace std;

void Print_us(Timer& timer, const char* msg) { std::cout << msg << " " <<  timer.microsecs() << " us.\n"; }

#define NUM_STEPS 400000000 

void thread_fcn(int my_id, int num_threads, double step, double *sum_p, std::mutex *lock_p);

/* ---- main program ---- */

int main(int argc, char *argv[]) {

    double pi;
    double sum = 0.0;
    double step = 1.0/(double) NUM_STEPS; 

    int num_threads = std::thread::hardware_concurrency();

    std::mutex lock;

    Timer timer;
    timer.Start();

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i)  {
        // threads.push_back(std::thread(thread_fcn, i, num_threads, step, &sum, &lock));
        threads.emplace_back(thread_fcn, i, num_threads, step, &sum, &lock);
    }
    for (auto& t : threads) t.join();

    pi = step * sum;

    timer.Stop();

    cout << "parallel program results with " << num_threads << " threads:\n";
    cout << "difference between computed pi " << pi << " and math.h M_PI " << M_PI << " = " << fabs(pi - M_PI) << "\n";
    Print_us(timer , "time to compute = " );

    return EXIT_SUCCESS;
}

/* ---- code to be executed by each thread ---- */
void thread_fcn(int my_id, int num_threads, double step, double *sum_p, std::mutex *lock_p)
{
    double x;
    double part_sum = 0.0;

    /* do this thread's part of computation */
    for (int i=my_id; i < NUM_STEPS; i += num_threads) { 
        x = (i+0.5)*step;
        part_sum += 4.0/(1.0+x*x); 
    }
    /* add partial sum to global sum */
    lock_p->lock();
    *sum_p += part_sum;
    lock_p->unlock();
}
