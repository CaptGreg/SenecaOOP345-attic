// g++  -std=c++11 -o2 omp.cpp -o omp && time ./omp ; g++ -std=c++11 -O2 -fopenmp omp.cpp -o omp && time ./omp

// Visual Studio command line:
// CL omp.c 
// omp
// CL /openmp omp.c 
// omp

#include <omp.h>
#include <cmath> 

const int N = 50 * 1000 * 1000;

long double x[N];
long double y[N];
long double z[N];

int main(int argc, char**argv)
{
    int i;

#pragma omp parallel for
    for(i = 0 ; i  < N ; i++) {
      x[i] = sin(M_PI*i/1000.);
      y[i] = cos(M_PI*i/1000.);
      z[i] = 2.3 * x[i] + y[i] + 9.9 * sin(M_PI*i/1000.);
    }
}
