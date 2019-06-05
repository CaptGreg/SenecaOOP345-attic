// motivated from example at http://en.cppreference.com/w/cpp/algorithm/reduce 

// However, std::reduce does not exist
// g++-8 -Wall -std=c++17   -fopenmp -Ofast reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=c++1y   -fopenmp -Ofast reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=c++1z   -fopenmp -Ofast reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=gnu++17 -fopenmp -Ofast reduce.cpp -o reduce -pthread
// g++-8 -Wall -std=gnu++2a -fopenmp -Ofast reduce.cpp -o reduce -pthread

#include <chrono>
#include <future>
#include <iostream>
#include <numeric>           // std::accumulate
#include <omp.h>
#include <parallel/numeric>  // std::__parallel::accumulate
#include <thread>
#include <vector>

using namespace std;

// want 'execution' header file
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


// 2019-06-05: ls /usr/include/c++/*/parallel/
// /usr/include/c++/7.4.0/parallel/:
// algobase.h            base.h                  equally_split.h   for_each_selectors.h  multiseq_selection.h  omp_loop.h         partition.h       search.h          types.h
// algo.h                basic_iterator.h        features.h        iterator.h            multiway_merge.h      omp_loop_static.h  queue.h           set_operations.h  unique_copy.h
// algorithm             checkers.h              find.h            list_partition.h      multiway_mergesort.h  parallel.h         quicksort.h       settings.h        workstealing.h
// algorithmfwd.h        compatibility.h         find_selectors.h  losertree.h           numeric               par_loop.h         random_number.h   sort.h
// balanced_quicksort.h  compiletime_settings.h  for_each.h        merge.h               numericfwd.h          partial_sum.h      random_shuffle.h  tags.h
// 
// /usr/include/c++/7/parallel/:
// algobase.h            base.h                  equally_split.h   for_each_selectors.h  multiseq_selection.h  omp_loop.h         partition.h       search.h          types.h
// algo.h                basic_iterator.h        features.h        iterator.h            multiway_merge.h      omp_loop_static.h  queue.h           set_operations.h  unique_copy.h
// algorithm             checkers.h              find.h            list_partition.h      multiway_mergesort.h  parallel.h         quicksort.h       settings.h        workstealing.h
// algorithmfwd.h        compatibility.h         find_selectors.h  losertree.h           numeric               par_loop.h         random_number.h   sort.h
// balanced_quicksort.h  compiletime_settings.h  for_each.h        merge.h               numericfwd.h          partial_sum.h      random_shuffle.h  tags.h
// 
// /usr/include/c++/8/parallel/:
// algobase.h            base.h                  equally_split.h   for_each_selectors.h  multiseq_selection.h  omp_loop.h         partition.h       search.h          types.h
// algo.h                basic_iterator.h        features.h        iterator.h            multiway_merge.h      omp_loop_static.h  queue.h           set_operations.h  unique_copy.h
// algorithm             checkers.h              find.h            list_partition.h      multiway_mergesort.h  parallel.h         quicksort.h       settings.h        workstealing.h
// algorithmfwd.h        compatibility.h         find_selectors.h  losertree.h           numeric               par_loop.h         random_number.h   sort.h
// balanced_quicksort.h  compiletime_settings.h  for_each.h        merge.h               numericfwd.h          partial_sum.h      random_shuffle.h  tags.h

// ls /usr/include/c++/*/experimental
// /usr/include/c++/7.4.0/experimental:
// algorithm  bits    filesystem    iterator  memory           optional         ratio  source_location  system_error  unordered_map  vector
// any        chrono  forward_list  list      memory_resource  propagate_const  regex  string           tuple         unordered_set
// array      deque   functional    map       numeric          random           set    string_view      type_traits   utility
// 
// /usr/include/c++/7/experimental:
// algorithm  bits    filesystem    iterator  memory           optional         ratio  source_location  system_error  unordered_map  vector
// any        chrono  forward_list  list      memory_resource  propagate_const  regex  string           tuple         unordered_set
// array      deque   functional    map       numeric          random           set    string_view      type_traits   utility
// 
// /usr/include/c++/8/experimental:
// algorithm  bits    filesystem    iterator  memory           optional         ratio  source_location  system_error  unordered_map  vector
// any        chrono  forward_list  list      memory_resource  propagate_const  regex  string           tuple         unordered_set
// array      deque   functional    map       numeric          random           set    string_view      type_traits   utility

int main()
{
    vector<double> v(10 * 1000 * 1000, 0.5);
 
    {
        auto t1 = chrono::high_resolution_clock::now();
        double result = 0.;
        result = accumulate(v.begin(), v.end(), result);
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << fixed << "accumulate result " << result
                  << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using __parallel::accumulate(v.begin(), v.end(), 0.) on a " << thread::hardware_concurrency() << " core machine:\n";
        auto t1 = chrono::high_resolution_clock::now();
        double result = 0.;
        result = __parallel::accumulate(v.begin(), v.end(), result);
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "__parallel::accumulate result "
                  << result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using indexed for-loop.\n";
        auto t1 = chrono::high_resolution_clock::now();
        double result = 0.;
        for(unsigned i = 0; i < v.size(); i++)
          result += v[i];
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "for-loop result "
                  << result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using iterator for-loop.\n";
        auto t1 = chrono::high_resolution_clock::now();
        double result = 0.;
        for(auto it = v.begin(); it != v.end(); ++it)
          result += *it;
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "for-loop result "
                  << result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using OMP PARALLEL FOR for-loop on a " << thread::hardware_concurrency() << " core machine:\n";
        auto t1 = chrono::high_resolution_clock::now();
        double result = 0.;
        #pragma omp parallel for reduction(+:result)
        // #pragma omp simd reduction(+:result) // simd omp is twice as slow 
        for(size_t i = 0; i < v.size(); i++)
          result += v[i];
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "OMP PARALLEL FOR for-loop result "
                  << result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using range-based-for-loop.\n";
        auto t1 = chrono::high_resolution_clock::now();
        double result = 0.;
        for(double e :  v)
          result += e;
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "range-based-for-loop result "
                  << result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using DIY threaded indexed for-loop on a " << thread::hardware_concurrency() << " core machine:\n";
        auto t1 = chrono::high_resolution_clock::now();
        size_t N = thread::hardware_concurrency();
        size_t chunk = (v.size() + N-1) / N;
        vector<future<double>> futs;
        size_t beg = 0;
        for(size_t i = 0; i < N; i++) {
          size_t end = beg + chunk;
          if(i == N) 
            end = v.size();
          futs.emplace_back( 
            async(
                      launch::async,
                      [&v] (size_t beg, size_t end) 
                        { double r = 0.; for(auto i=beg; i!=end; i++) r += v[i]; return r; }, 
                      beg, 
                      end
                    )
             );
          beg = end;
        }

        double result = 0.;
        for(auto& e : futs)
          result += e.get();
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "DIY threaded indexed for-loop result "
                  << result << " took " << ms.count() << " ms\n\n";
    }

    {
        cout << "Same calculation using DIY threaded iterator for-loop on a " << thread::hardware_concurrency() << " core machine:\n";    
        auto t1 = chrono::high_resolution_clock::now();
        size_t N = thread::hardware_concurrency();
        size_t chunk = (v.size() + N-1) / N;
        vector<future<double>> futs;
        auto beg = v.begin();
        for(size_t i = 0; i < N; i++) {
          auto end = beg + chunk;
          if(i == N) 
            end = v.end();
          futs.emplace_back( 
            async(
                      launch::async,
                      [] (decltype(v.begin()) beg, decltype(v.end()) end) 
                        { double r = 0.; for(auto it=beg; it!=end; ++it) r += *it; return r; }, 
                      beg, 
                      end
                    )
            );
          beg = end;
        }

        double result = 0.;
        for(auto& e : futs)
          result += e.get();
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << " DIY threaded iterator for-loop result "
                  << result << " took " << ms.count() << " ms\n\n";
    }
 
 #if 0 // OMP doesn't support range-based for-loops
    {
        // OMP spec (even 5.0) states all for-loops must be canonical.  That is 'for(init; test; increment)'.

        cout << "Same calculation using OMP PARALLEL FOR range-based-for-loop concurrency on " << thread::hardware_concurrency() << " core machine:\n";
        auto t1 = chrono::high_resolution_clock::now();
        double result = 0.;
        #pragma omp parallel for
        for(double e :  v)
          result += e;
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "__parallel::accumulate result "
                  << result << " took " << ms.count() << " ms\n\n";
    }
#endif

 #if 0
    {
        // reduce is the out-of-order version of accumulate, not today, maybe someday...

        auto t1 = chrono::high_resolution_clock::now();
        double result = reduce(execution::par, v.begin(), v.end(), 0.); // GB 'par' someday...
        auto t2 = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> ms = t2 - t1;
        cout << "reduce result "
                  << result << " took " << ms.count() << " ms\n\n";
    }
 #endif

    return 0;
}
