// g++  -std=c++11 -o2 omp2.cpp -o omp2 && time ./omp2 ; g++ -std=c++11 -O2 -fopenmp omp2.cpp -o omp2 && time ./omp2

// Visual Studio command line:
// CL omp2.c 
// omp2
// CL /openmp omp2.c 
// omp2

#include <omp.h>
#include <cmath> 

#include <iostream>
#include <chrono>           // c++11 time functions
using namespace std;

const int N = 50 * 1000 * 1000;

long double x[N];
long double y[N];
long double z[N];

int main(int argc, char**argv)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> stop;
    typedef std::chrono::duration<int,std::milli> millisecs_t ;

    int i;
    cout << "This is a " << 8*sizeof(void*) << "-bit operating system\n";
    cout << "sizeof(long double) = " << sizeof(long double) << " bytes\n";
    cout << "sizeof(x + y + z)   = " << sizeof(x) + sizeof(y) + sizeof(z) << " bytes, "
         << (sizeof(x) + sizeof(y) + sizeof(z)) / 1e9 << " GB\n";

    start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
    for(i = 0 ; i  < N ; i++) {
      x[i] = sin(M_PI*i/1000.);
      y[i] = cos(M_PI*i/1000.);
      z[i] = 2.3 * x[i] + y[i] + 9.9 * sin(M_PI*i/1000.);
    }

    stop = std::chrono::high_resolution_clock::now();

    millisecs_t duration( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    long tWait = duration.count();
    cout << "time = " << tWait * 1e-3 << " seconds\n";
}
