#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

#include <boost/lockfree/queue.hpp>

#define USE_YIELD

class threadpool
{
public:
    //  constructors
    //
    //  calls threadpool(size_t concurrency) with:
    //
    //  concurrency - std::thread::hardware_concurrency()
    threadpool();
    //  calls threadpool(size_t concurrency, size_t queue_size) with:
    //
    //  concurrency - concurrency
    //  queue_size  - 128, arbitary value, should be sufficient for most
    //                use cases.
    threadpool(size_t concurrency);
    //  creates a threadpool with a specific number of threads and
    //  a maximum number of queued tasks.
    //
    //  Argument
    //    concurrency - the guaranteed number of threads used in the
    //                  threadpool, ie. maximum number of tasks worked
    //                  on concurrently.
    //    queue_size  - the maximum number of tasks that can be queued
    //                  for completion, currently running tasks do not
    //                  count towards this total.
    threadpool(size_t concurrency, size_t queue_size);

    //  destructor
    //
    //  Will complete any currently running task as normal, then
    //  signal to any other tasks that they were not able to run
    //  through a std::runtime_error exception
    ~threadpool();

    threadpool(const threadpool &)             = delete;
    threadpool(threadpool &&)                  = delete;

    threadpool & operator=(const threadpool &) = delete;
    threadpool & operator=(threadpool &&)      = delete;

    //  run
    //
    //  Runs the given function on one of the thread pool
    //  threads in First In First Out (FIFO) order
    //
    //  Argument
    //    task - function or functor to be called on the
    //           thread pool.
    //
    //  Result
    //    signals when the task has completed with either
    //    success or an exception. Also results in an
    //    exception if the thread pool is destroyed before
    //    execution has begun.
    std::future<void> run(std::function<void()> && task);

private:
    struct task_package
    {
    public:
        std::promise<void> completion_promise;
        std::function<void()> task;
    };

    //  Have to use 'task_package *' since a trivial destructor is
    //  required, 'task_package' and 'std::unique_ptr<task_package>'
    //  do not satisfy.
    boost::lockfree::queue<task_package *> tasks;
    std::vector<std::thread> threads;
    std::atomic<bool> shutdown_flag;

#ifndef USE_YIELD
    volatile bool wakeup_flag;
    std::condition_variable wakeup_signal;
    std::mutex wakeup_mutex;
#endif

    inline bool pop_task(std::unique_ptr<task_package> & out);
};

#endif
