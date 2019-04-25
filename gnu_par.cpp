// Here is a work around for STL libstdc++ parallel mode
//     https://gcc.gnu.org/onlinedocs/libstdc++/manual/parallel_mode.html
// which doesn't work yet (g++-7, g++-8, clang++-6 as of 2019-04-25),
//     https://stackoverflow.com/questions/51031060/are-c17-parallel-algorithms-implemented-already

// It works for g++-7, g++-8, and clang++-6.

// HOWEVER IT MUST BE COMPILED WITH OPENMP SUPPORT
// g++-7   -std=c++17 -fopenmp gnu_par.cpp -o gnu_par  && ./gnu_par
// g++-8   -std=c++17 -fopenmp gnu_par.cpp -o gnu_par  && ./gnu_par
// clang++ -std=c++17 -fopenmp gnu_par.cpp -o gnu_par  && ./gnu_par

/*
  Algorithm                      Header     Parallel algorithm                         Parallel header
  std::accumulate                numeric     __gnu_parallel::accumulate                parallel/numeric
  std::adjacent_difference       numeric     __gnu_parallel::adjacent_difference       parallel/numeric
  std::inner_product             numeric     __gnu_parallel::inner_product             parallel/numeric
  std::partial_sum               numeric     __gnu_parallel::partial_sum               parallel/numeric
  std::adjacent_find             algorithm   __gnu_parallel::adjacent_find             parallel/algorithm
  std::count                     algorithm   __gnu_parallel::count                     parallel/algorithm
  std::count_if                  algorithm   __gnu_parallel::count_if                  parallel/algorithm
  std::equal                     algorithm   __gnu_parallel::equal                     parallel/algorithm
  std::find                      algorithm   __gnu_parallel::find                      parallel/algorithm
  std::find_if                   algorithm   __gnu_parallel::find_if                   parallel/algorithm
  std::find_first_of             algorithm   __gnu_parallel::find_first_of             parallel/algorithm
  std::for_each                  algorithm   __gnu_parallel::for_each                  parallel/algorithm
  std::generate                  algorithm   __gnu_parallel::generate                  parallel/algorithm
  std::generate_n                algorithm   __gnu_parallel::generate_n                parallel/algorithm
  std::lexicographical_compare   algorithm   __gnu_parallel::lexicographical_compare   parallel/algorithm
  std::mismatch                  algorithm   __gnu_parallel::mismatch                  parallel/algorithm
  std::search                    algorithm   __gnu_parallel::search                    parallel/algorithm
  std::search_n                  algorithm   __gnu_parallel::search_n                  parallel/algorithm
  std::transform                 algorithm   __gnu_parallel::transform                 parallel/algorithm
  std::replace                   algorithm   __gnu_parallel::replace                   parallel/algorithm
  std::replace_if                algorithm   __gnu_parallel::replace_if                parallel/algorithm
  std::max_element               algorithm   __gnu_parallel::max_element               parallel/algorithm
  std::merge                     algorithm   __gnu_parallel::merge                     parallel/algorithm
  std::min_element               algorithm   __gnu_parallel::min_element               parallel/algorithm
  std::nth_element               algorithm   __gnu_parallel::nth_element               parallel/algorithm
  std::partial_sort              algorithm   __gnu_parallel::partial_sort              parallel/algorithm
  std::partition                 algorithm   __gnu_parallel::partition                 parallel/algorithm
  std::random_shuffle            algorithm   __gnu_parallel::random_shuffle            parallel/algorithm
  std::set_union                 algorithm   __gnu_parallel::set_union                 parallel/algorithm
  std::set_intersection          algorithm   __gnu_parallel::set_intersection          parallel/algorithm
  std::set_symmetric_difference  algorithm   __gnu_parallel::set_symmetric_difference  parallel/algorithm
  std::set_difference            algorithm   __gnu_parallel::set_difference            parallel/algorithm
  std::sort                      algorithm   __gnu_parallel::sort                      parallel/algorithm
  std::stable_sort               algorithm   __gnu_parallel::stable_sort               parallel/algorithm
  std::unique_copy               algorithm   __gnu_parallel::unique_copy               parallel/algorithm
*/

// notice std::reduce, an out-of-order std::accumulate, is missing.
// https://en.cppreference.com/w/cpp/algorithm/reduce

#include <iostream>
#include <vector>
#include <parallel/algorithm>

int main()
{
  std::vector<int> v(100);

  for(auto& e: v)
    e = rand() % v.size();

  for(auto e: v)
    std::cout << e << " ";
  std::cout << "\n";

  // Explicitly force a call to parallel sort.
  __gnu_parallel::sort(v.begin(), v.end());

  std::cout << "\n";

  for(auto e: v)
    std::cout << e << " ";
  std::cout << "\n";

  return 0;
}
