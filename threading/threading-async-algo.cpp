// Time some algorithms running a reduction: calc sum of an array 

// clang++ -Wall -std=c++11 -O3  -fopenmp threading-async-algo.cpp -o threading-async-algo  -pthread && ./threading-async-algo 500000

#include <thread>
#include <future>
#include <iostream>
#include <sstream>

#include <omp.h>

#include <valarray> // '#include <valarray>' before vector
#include <vector>
#include <algorithm>

using namespace std;

int threadsSpawned = 0; // psum counts threads

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Reset() { start = std::chrono::high_resolution_clock::now(); }
  uint64_t Print(string msg) {
    stop  = std::chrono::high_resolution_clock::now();
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t tWait = duration_get.count();
    std::cout << msg << tWait << " usecs\n";
    return tWait;
  }
};

template <typename ITERATOR> // invocation error: no matching function for call to 'async'
int parallel_sum(ITERATOR base, ITERATOR beg, ITERATOR end, thread::id ptid)
{
    thread::id tid = this_thread::get_id();

    auto len = distance(beg, end);

    if(len < 1000) return accumulate(beg, end, 0);

    ITERATOR mid = beg + len/2;

    future<int> f = async (launch::async, 
      [mid,end] () ->int{int s=0; for(auto i=mid;i<end;i++) s+=*i; return s;} 
    ); // works, but defeats the purpose of using a recursive call to break up the size of the problem

    int sum = parallel_sum(base, beg, mid, tid);
    threadsSpawned++;

    return sum + f.get();
}

// template <typename ITERATOR>
#define ITERATOR vector<int>::const_iterator   // works fine if template type defined as a macro
int PARALLEL_SUM(ITERATOR base, ITERATOR beg, ITERATOR end, thread::id ptid) // ptid = parent thread id
{
    thread::id tid = this_thread::get_id();

    auto len = distance(beg, end);

    if(len < 1000) return accumulate(beg, end, 0);

    ITERATOR mid = beg + len/2;

    future<int> f = async (launch::async, PARALLEL_SUM, base, mid, end, tid); // launch thread to do half
    threadsSpawned++;

    int sum = PARALLEL_SUM(base, beg, mid, tid); // recurse to do the other half

    return sum + f.get();
}

template <typename IT>
int psum(IT beg, IT end)
{
    auto cores = thread::hardware_concurrency();
    auto len = distance(beg, end);
    int workUnit = len / cores;

    future<int>* fTable = new std::future<int>[cores];

    for(int core = 0; core < cores; core++) {
        vector<int>::iterator  b = beg + core * workUnit;
        vector<int>::iterator  e = b + workUnit;

        if(core == (cores-1))  e = end; // 'cores' might not divide 'len' evenly

        auto f =[] (vector<int>::iterator b, vector<int>::iterator e)
               -> int { return std::accumulate(b, e, (int) 0); };

        fTable[core] = std::async(std::launch::async, f, b, e);
    }
    
    int sum = 0;
    for(int core = 0; core < cores; core++) {
       sum += fTable[core].get();
    }
    delete [] fTable;
    return sum;
}

 
int main(int argc, char ** argv)
{
    Timer timer;
    stringstream ss;
    thread::id tid = this_thread::get_id();
    auto cores = thread::hardware_concurrency();
    int sum;

    cout << cores << " hardware cores, main tid=" << tid << "\n";

    size_t size = 10 * 1000 * 1000;
    if(argc > 1) size = atoi(argv[1]);
    int value = -1;
    vector<int> v(size, value);

    ss << "main: v.size() = " << v.size() << "\n";
    cout << ss.str(); ss.str("");
    cout << "\n";

    timer.Reset();
    cout << "accumulate: reference case " << std::accumulate(v.begin(), v.end(), (int) 0) << "\n";
    timer.Print("accumulate: reference case ");
    cout << "\n";

    timer.Reset();
    sum = 0; for(auto i = v.begin(); i != v.end(); i++) sum += *i;
    cout << "iterator: reference case " << sum << "\n";
    timer.Print("iterator: reference case ");
    cout << "\n";

    timer.Reset();
    sum = 0; for(int i = size-1; i >=0; i--) sum += v[i];
    cout << "for loop count down: reference case " << sum << "\n";
    timer.Print("for loop count down: reference case ");
    timer.Reset();
    sum = 0; for(int i = 0; i < size; i++) sum += v[i];
    cout << "for loop count up: reference case " << sum << "\n";
    timer.Print("for loop count up: reference case ");

    cout << "\n";

    timer.Reset();
    sum = 0;
    #pragma omp parallel for 
    for(int i = size-1; i >=0; i--) sum += v[i];
    cout << "omp parallel for count down: reference case " << sum << "\n";
    timer.Print("omp parallel for count down: reference case ");
    timer.Reset();
    sum = 0;
    #pragma omp parallel for 
    for(int i = 0; i < size; i++) sum += v[i];
    cout << "omp parallel for count up: reference case " << sum << "\n";
    timer.Print("omp parallel for count up: reference case ");

    #define P(X) cout << #X "()=" << X() << "\n";
    P(omp_get_num_procs)
    P(omp_get_max_threads)
    cout << "\n";

    
    timer.Reset();
    sum = 0;
    #pragma omp parallel for reduction (+:sum)
    for(int i = size-1; i >=0; i--) sum += v[i];
    cout << "omp parallel for reduction (+:sum) count down: reference case " << sum << "\n";
    timer.Print("omp parallel for reduction (+:sum) count down: reference case ");
    timer.Reset();
    sum = 0;
    #pragma omp parallel for reduction (+:sum)
    for(int i = 0; i < size; i++) sum += v[i];
    cout << "omp parallel for reduction (+:sum) count up: reference case " << sum << "\n";
    timer.Print("omp parallel for reduction (+:sum) count up: reference case ");

    cout << "\n";

    timer.Reset();
    sum = 0; for(auto element : v) sum += element;
    cout << "range-based for: reference case " << sum << "\n";
    timer.Print("range-based for: reference case ");
    cout << "\n";

    valarray<int> va(value,size); // NOTE: ctor parameter order is backwards from other STL containers
    timer.Reset();
    ss<<"main: valarray<int>.sum() is "<< va.sum() <<"\n";
    cout << ss.str(); ss.str("");
    timer.Print("valarray<int>.sum() ");
    cout << "\n";

    timer.Reset();
    ss << "main: psum, 1 job per core= " << psum(v.begin(), v.end()) << "\n";
    cout << ss.str(); ss.str("");
    timer.Print("psum, 1 job per core ");
    cout << "\n";

    ::threadsSpawned  = 0;
    timer.Reset();
    ss<<"main: parallel_sum(template) is "<< parallel_sum<vector<int>::const_iterator>(v.cbegin(),v.cbegin(),v.cend(),tid) <<"\n";
    cout << ss.str(); ss.str("");
    timer.Print("parallel_sum(template) ");
    cout << ::threadsSpawned << " threads spawned\n";
    cout << "\n";

    ::threadsSpawned  = 0;
    timer.Reset();
    ss<<"main: PARALLEL_SUM(macro+recursion) is "<< PARALLEL_SUM(v.cbegin(),v.cbegin(),v.cend(),tid) <<"\n";   // works with const iterators
    cout << ss.str(); ss.str("");
    timer.Print("PARALLEL_SUM(macro+recursion) ");
    cout << ::threadsSpawned << " threads spawned\n";
    cout << "\n";
}
