// http://en.cppreference.com/w/cpp/algorithm/reduce 

// g++-8 -Wall -std=c++17   reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=c++1y   reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=c++1z   reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=gnu++17 reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=gnu++2a reduce.cpp -o reduce -pthread

#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
// #include <execution> // error: execution: No such file or directory

#include <parallel/parallel.h> // error: ‘std::execution’ has not been declared std::execution::par,
//  reduce.cpp:47:30: error: ‘reduce’ is not a member of ‘std’
//          double result = std::reduce(std::execution::par, v.begin(), v.end());
//                               ^~~~~~
// reduce.cpp:47:42: error: ‘std::execution’ has not been declared
//          double result = std::reduce(std::execution::par, v.begin(), v.end());

#include <algorithm> // http://en.cppreference.com/w/cpp/header/execution says <execution> part of <algorithm>
// #include <algorithm/execution> // error: algorithm/execution: No such file or directory

// error: ‘reduce’ is not a member of ‘std’
//          double result = std::reduce(std::execution::par, v.begin(), v.end());
//                               ^~~~~~
// reduce.cpp:55:30: note: suggested alternative: ‘replace’
//          double result = std::reduce(std::execution::par, v.begin(), v.end());
//                               ^~~~~~
//                               replace
// reduce.cpp:55:42: error: ‘std::execution’ has not been declared
//          double result = std::reduce(std::execution::par, v.begin(), v.end());


// $ find /usr/include  -name execution
// (nothing found)

// grep -R -r ::par'\>' /usr/include 
//    *  finds thrust, omp, mpi, ..., but no c++

// try experimental:
// ls /usr/include/c++/8/experimental
// algorithm  deque         list             optional         set              tuple          vector
// any        filesystem    map              propagate_const  source_location  type_traits
// array      forward_list  memory           random           string           unordered_map
// bits       functional    memory_resource  ratio            string_view      unordered_set
// chrono     iterator      numeric          regex            system_error     utility

#include <numeric> // GB std::accumulate
int main()
{
    std::vector<double> v(10'000'007, 0.5);
 
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        double result = std::accumulate(v.begin(), v.end(), 0.0);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::fixed << "std::accumulate result " << result
                  << " took " << ms.count() << " ms\n";
    }
 
 #if 0
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        double result = std::reduce(std::execution::par, v.begin(), v.end()); // GB 'par' someday...
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << "std::reduce result "
                  << result << " took " << ms.count() << " ms\n";
    }
 #endif

    return 0;
}
