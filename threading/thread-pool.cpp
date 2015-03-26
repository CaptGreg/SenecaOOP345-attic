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

#include <string>
#include <deque>
#include <vector>
#include <iostream>      // cout
#include <chrono>        // sleep
#include <sstream>       // stringstream
#include <atomic>

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
 
class ThreadPool;
  
// our worker thread objects
class Worker {
public:
    Worker(ThreadPool &s) : pool(s), tid(this_thread::get_id()) { }
    void operator()();
private:
    ThreadPool &pool;
    std::thread::id tid; // GB for debugging
};
  
// the actual thread pool
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F> void enqueue(F f);
    ~ThreadPool();
private:
    friend class Worker;
 
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
 
    // the task queue
    std::deque< std::function<void()> > tasks;
 
    // synchronization
    std::mutex              queue_mutex;
    std::condition_variable condition;
    volatile bool           stop;
    std::atomic<int>        active;     // GB active thread count.
    std::mutex              idle_mutex; // GB
    std::condition_variable idle_cv;    // GB
};

void Worker::operator()()
{
    std::function<void()> task;
    while(true) {
        {   // acquire lock
            std::unique_lock<std::mutex> lock(pool.queue_mutex);
             
            // look for a work item
            while(!pool.stop && pool.tasks.empty()) { // if there are none wait for notification
                pool.condition.wait(lock);
            }
 
            if(pool.stop) return;// exit if the pool is stopped
 
            // get the task from the queue
            task = pool.tasks.front();
            pool.tasks.pop_front();
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
// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads) :   stop(false)
{
    active = 0; // atomic data type, = overload
    cout << "ThreadPool:  launching " << threads << " worker threads\n";
    for(size_t i = 0;i<threads;++i)
        workers.push_back(std::thread(Worker(*this)));
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
     
    for( auto& w : workers ) w.join(); // join them
}

// add new work item to the pool
template<class F>
void ThreadPool::enqueue(F f)
{
    { // acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push_back(std::function<void()>(f)); // add the task
    } // release lock
     
    condition.notify_one(); // wake up one thread
}

int main(int argc,char**argv)
{
    ThreadPool pool(3); // create a thread pool of worker threads
 
    for(int i = 0;i<9;++i) // queue a bunch of "work items"
    {
        stringstream ss; ss << "main: enqueuing " << i << "\n"; cout << ss.str();
        pool.enqueue([i] () -> void // compiles fine without the '() -> void'
            {
                std::thread::id tid = this_thread::get_id();
                stringstream ss; 
                ss << ">>> hello " << i << " tid=(" << tid << ')' << '\n'; 
                cout << ss.str(); 
    
                this_thread::sleep_for (chrono::milliseconds(1000)); 
    
                ss.str("");
                ss << ">>> world " << i << " tid=(" << tid << ')' << '\n'; 
                cout << ss.str(); 
            }
        );
    }
}
