// https://msdn.microsoft.com/en-us/library/hh873179.aspx

// parallel-map-reduce.cpp
// compile with: /EHsc

// GB try compiling with g++-8  -std=c++17
// GB   g++-8 -Wall -std=c++17 map-reduce.cpp -o map-reduce -pthread
// GB needs a 'reduce' function

#include <algorithm>         
#include <numeric>           // acccumulate + http://en.cppreference.com/w/cpp/algorithm/reduce
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
// #include <windows.h>

// using namespace concurrency;
using namespace std;

class MapFunc 
{ 
public:
    unordered_map<wstring, size_t> operator()(vector<wstring>& elements) const 
    { 
        unordered_map<wstring, size_t> m;
        for_each(begin(elements), end(elements), [&m](const wstring& elem)
        { 
            m[elem]++;
        });
        return m; 
    }
}; 

struct ReduceFunc : binary_function<unordered_map<wstring, size_t>, 
                    unordered_map<wstring, size_t>, unordered_map<wstring, size_t>>
{
    unordered_map<wstring, size_t> operator() (
        const unordered_map<wstring, size_t>& x, 
        const unordered_map<wstring, size_t>& y) const
    {
        unordered_map<wstring, size_t> ret(x);
        for_each(begin(y), end(y), [&ret](const pair<wstring, size_t>& pr) {
            auto key = pr.first;
            auto val = pr.second;
            ret[key] += val;
        });
        return ret; 
    }
}; 


#if 0
Compiling the Code

To compile the code, copy it and then paste it in a Visual Studio project, or 
paste it in a file that is named parallel-map-reduce.cpp and then run the 
following command in a Visual Studio Command Prompt window.

cl.exe /EHsc parallel-map-reduce.cpp
Robust Programming

In this example, you can use the concurrent_unordered_map class—which is defined 
in concurrent_unordered_map.h—to perform the map and reduce in one operation.

C++
#endif

void app1()
{ 
  // File 1 
  vector<wstring> v1;
  v1.push_back(L"word1"); //1 
  v1.push_back(L"word1"); //2 
  v1.push_back(L"word2"); 
  v1.push_back(L"word3"); 
  v1.push_back(L"word4"); 

  // File 2 
  vector<wstring> v2; 
  v2.push_back(L"word5"); 
  v2.push_back(L"word6"); 
  v2.push_back(L"word7"); 
  v2.push_back(L"word8"); 
  v2.push_back(L"word1"); //3 

  vector<vector<wstring>> v;
  v.push_back(v1);
  v.push_back(v2);

  vector<unordered_map<wstring, size_t>> map(v.size()); 

  // The Map operation
  // parallel_transform // GB not defined
  transform             // GB try transform
    (begin(v), end(v), begin(map), MapFunc());   // MapFunc function object

  // The Reduce operation 
  unordered_map<wstring, size_t> result = 
  // parallel_reduce // GB not defined
  // std::reduce // GB not defined
  std::accumulate // GB reduce is an out-of-order accumulate, substitute accumulate
    ( begin(map), end(map), unordered_map<wstring, size_t>(), ReduceFunc());

  wcout << L"\"word1\" occurs " << result.at(L"word1") << L" times. " << endl;

  /* Output:
     "word1" occurs 3 times.
  */
} 

#include <atomic>
void app2()
{
    // File 1 
    vector<wstring> v1;
    v1.push_back(L"word1"); //1 
    v1.push_back(L"word1"); //2 
    v1.push_back(L"word2"); 
    v1.push_back(L"word3"); 
    v1.push_back(L"word4"); 

    // File 2 
    vector<wstring> v2; 
    v2.push_back(L"word5"); 
    v2.push_back(L"word6"); 
    v2.push_back(L"word7"); 
    v2.push_back(L"word8"); 
    v2.push_back(L"word1"); //3 

    vector<vector<wstring>> v;
    v.push_back(v1);
    v.push_back(v2);

    // concurrent_unordered_map // GB not defined
    unordered_map <wstring, size_t> result; // no issue in-order
    // unordered_map <wstring, atomic_size_t> result;  // avoid race condition if threaded

    // parallel_for_each // GB not defined
    for_each(begin(v), end(v), [&result](const vector<wstring>& words) {
        for_each(begin(words), end(words), 
          [&result](const wstring& word) {
            // InterlockedIncrement(&result[word]);
            // error: ‘InterlockedIncrement’ was not declared in this scope
            // ‘InterlockedIncrement’ appears to be a C# atomic increment
            // no issue in-order
            // race condition possible with out-of-order threads
            result[word]++;
          }
        );
    });
                
    wcout << L"\"word1\" occurs " << result.at(L"word1") << L" times. " << endl;

    /* Output:
       "word1" occurs 3 times.
    */
}

int main()
{
  app1();
  app2();
  return 0;
}
