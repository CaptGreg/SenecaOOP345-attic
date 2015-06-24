// C++11 thread pool 'ThreadPool' + 'Worker' classes:
// http://progsch.net/wordpress/?p=81
// boost::... main
// http://progsch.net/wordpress/?p=71 

// GB NOTES:-
// The ThreadPool destructor shuts down the threadpool even if there is work in the queue
// GB fixed!
//  - use an atomic variable to 'count' active threads.
//  - set a condition_variable when the active thread count drops to zero
//  - if no work to do, all threads finished, ~ThreadPool() active, let ~ThreadPool() 
//    signal the threads to exit and join the threads.

#include <queue>         // GB queue works fine, was deque
#include <vector>        // used to hold tid's so we can join threads
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <iostream>      // cout
#include <chrono>        // sleep
#include <sstream>       // stringstream
#include <cstdlib>       // random number generator, 'random()'
#include <ctime>         // initialize random number generator with 'time(0L)', the current time

using namespace std;
 
class ThreadPool;
  
// our worker thread objects
class Worker {
    ThreadPool &pool;
    std::thread::id tid; // GB for debugging
public:
    Worker(ThreadPool &s) : pool(s), tid(this_thread::get_id()) { }
    void operator()();
};
  
// the actual thread pool
class ThreadPool {
    friend class Worker;
 
    // need to keep track of threads so we can join them
    std::vector< std::thread > threadVector;
 
    // the task queue
    std::queue< std::function<void()> > taskQueue;
 
    // synchronization
    std::mutex              queue_mutex;
    std::condition_variable condition;
    volatile bool           stop;
    std::atomic<int>        active;     // GB active thread count.
    std::mutex              idle_mutex; // GB
    std::condition_variable idle_cv;    // GB
public:
    ThreadPool(size_t);
    template<class F> void enqueue(F f);
    ~ThreadPool();
};

void Worker::operator()()
{
    std::function<void()> task;
    while(true) {
        {   // acquire lock
            std::unique_lock<std::mutex> lock(pool.queue_mutex);
             
            // look for a work item
            while(!pool.stop && pool.taskQueue.empty()) // if there are none wait for notification
                pool.condition.wait(lock);
 
            if(pool.stop) return;// exit if the pool is stopped
 
            // get the next task from the task queue
            task = pool.taskQueue.front();
            pool.taskQueue.pop();
        }   // release lock

        // GB active count and cv for notifying ThreadPool dtor to shut it down.

        pool.active++; // atomic data type, ++ overload
        task(); // execute the task
        pool.active--; // atomic data type, -- overload

        if(!pool.active.load()) // ThreadPool destructor should be waiting for the last thread to finish
        { // acquire lock
            std::unique_lock<std::mutex> lck(pool.idle_mutex);
            pool.idle_cv.notify_one();
        } // release lock
    }
}
// the constructor just launches some amount of threadVector
ThreadPool::ThreadPool(size_t threads) :   stop(false), active(0)
{
    active = 0; // atomic data type, = overload
    cout << "ThreadPool:  launching " << threads << " worker threads\n";
    for(size_t t = 0; t<threads; t++)
        threadVector.push_back(std::thread(Worker(*this)));
}
   
// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    // GB wait for active threads to finish
    { // acquire lock
        std::unique_lock<std::mutex> lck(idle_mutex);
        while (active.load()) idle_cv.wait(lck);  // notified if a thread drops the active count to zero.
    } // release lock

    // stop all threads
    stop = true;
    condition.notify_all();
     
    for( auto& w : threadVector ) w.join(); // join them
}

// add new work item to the pool
template<class F>
void ThreadPool::enqueue(F f)
{
    { // acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);
        taskQueue.push(std::function<void()>(f)); // add the task (queue)
    } // release lock
     
    condition.notify_one(); // wake up one thread
}

int main(int argc,char**argv)
{
    srandom(time(0L)); // comment this out to generate the same repeatible random number sequence

    const int THREADS_IN_POOL = 3;
    ThreadPool threadPool(THREADS_IN_POOL); // create a thread pool of worker threads
 
    for(int wi = 0; wi<11; wi++) // queue some "work items"
    {
        stringstream ss; 
        ss << "main: enqueuing " << wi << "\n"; cout << ss.str();
        auto workItem = [wi] () -> void // compiles fine without the '() -> void'
            {
                // std::thread::id tid = this_thread::get_id();
                stringstream ss; 
                ss << ">>> hello " << wi 
                   // << " tid=(" << tid << ')' 
                   << '\n'; 
                cout << ss.str(); 
    
                int nap =  random() % 5000; 
                this_thread::sleep_for (chrono::milliseconds(nap));
    
                // ss.str("");
                ss.clear();
                ss << ">>> world " << wi 
                   // << " tid=(" << tid << ')' 
                   << " " << nap << " milliseconds"
                   << '\n'; 
                cout << ss.str(); 
            };
        threadPool.enqueue(workItem);
    }
}
