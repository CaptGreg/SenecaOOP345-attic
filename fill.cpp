// Q. What is the fastest way to fill a vector or array with a constant value?
// A. Use anything but a for(int i=...) loop with -Ofast
//    Use std::fill with -O0

// clang++ -O0 -Wall -std=c++11  fill.cpp -o fill && ./fill
// clang++ -O1 -Wall -std=c++11  fill.cpp -o fill && ./fill
// clang++ -O2 -Wall -std=c++11  fill.cpp -o fill && ./fill
// clang++ -O3 -Wall -std=c++11  fill.cpp -o fill && ./fill

#if 0
Timings with typedef int64_t DATA_TYPE;

clang++ -O0 -Wall -std=c++11  fill.cpp -o fill && ./fill
    for (int i=0; i<size; i++) vec[i] = -1; 616733 usecs
    -100000000

    for (DATA_TYPE& x : vec) x = -2; 1834182 usecs
    -200000000

    std::fill(vec.begin(), vec.end(), -3); 325723 usecs
    -300000000

    for(int i=0; i<size; i++) array[i] = -4; 521863 usecs
    -400000000

    for (DATA_TYPE& x : array) x = -5; 392098 usecs
    -500000000

    std::fill(array, array+size, -6); 330221 usecs
    -600000000

clang++ -Ofast -Wall -std=c++11  fill.cpp -o fill && ./fill
    for (int i=0; i<size; i++) vec[i] = -1; 166548 usecs
    -100000000

    for (DATA_TYPE& x : vec) x = -2; 164027 usecs
    -200000000

    std::fill(vec.begin(), vec.end(), -3); 168817 usecs
    -300000000

    for(int i=0; i<size; i++) array[i] = -4; 283050 usecs
    -400000000

    for (DATA_TYPE& x : array) x = -5; 170926 usecs
    -500000000

    std::fill(array, array+size, -6); 173210 usecs
    -600000000
#endif


#include <iostream>
#include <vector>
#include <numeric>      // std::accumulate
#include <string>
// using namespace std;

#include "timer.h"

// typedef double DATA_TYPE;
// typedef int DATA_TYPE;
typedef int64_t DATA_TYPE;

const int SIZE = 100 * 1000 * 1000;   // 100 million
std::vector<DATA_TYPE> vec  (SIZE);
DATA_TYPE              array[SIZE];

const size_t size =  sizeof(array) / sizeof(array[0]);

int main(int argc,char**argv)
{
  Timer timer;

  timer.Start();
  for (int i=0; i<SIZE; i++) vec[i] = -1; 
  timer.Stop(); 
  timer.Print_us("for (int i=0; i<size; i++) vec[i] = -1; ");
  std::cout << std::accumulate(vec.begin(), vec.end(), (DATA_TYPE) 0) << "\n\n";

  timer.Start();
  for (DATA_TYPE& x : vec) x = -2; 
  timer.Stop(); 
  timer.Print_us("for (DATA_TYPE& x : vec) x = -2; ");
  std::cout << std::accumulate(vec.begin(), vec.end(), (DATA_TYPE) 0) << "\n\n";

  timer.Start();
  std::fill(vec.begin(), vec.end(), -3);
  timer.Stop(); 
  timer.Print_us("std::fill(vec.begin(), vec.end(), -3); ");
  std::cout << std::accumulate(vec.begin(), vec.end(), (DATA_TYPE) 0) << "\n\n";

  timer.Start();
  for (int i=0; i<size; i++) array[i] = -4; 
  timer.Stop(); 
  timer.Print_us("for(int i=0; i<size; i++) array[i] = -4; ");
  std::cout << std::accumulate(array, array+size, (DATA_TYPE) 0) << "\n\n";

  timer.Start();
  for (DATA_TYPE& x : array) x = -5; 
  timer.Stop(); 
  timer.Print_us("for (DATA_TYPE& x : array) x = -5; ");
  std::cout << std::accumulate(array, array+size, (DATA_TYPE) 0) << "\n\n";

  timer.Start();
  std::fill(array, array+size, -6);
  timer.Stop(); 
  timer.Print_us("std::fill(array, array+size, -6); ");
  std::cout << std::accumulate(array, array+size, (DATA_TYPE) 0) << "\n\n";
}
