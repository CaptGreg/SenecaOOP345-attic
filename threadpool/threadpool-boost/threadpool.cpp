#include "threadpool.hpp"

#include <algorithm>
#include <exception>
#include <utility>

template<typename T>
constexpr T zero(T)
{
    return 0;
}

threadpool::threadpool() :
    threadpool(std::thread::hardware_concurrency())
{ };

threadpool::threadpool(size_t concurrency) :
    threadpool(concurrency, 128)
{ };

threadpool::threadpool(size_t concurrency, size_t queue_size) :
    tasks(queue_size),
    shutdown_flag(false),
    threads()
#ifndef USE_YIELD
    ,wakeup_flag(false),
    wakeup_signal(),
    wakeup_mutex()
#endif
{
    // This is more efficient than creating the 'threads' vector with
    // size constructor and populating with std::generate since
    // std::thread objects will be constructed only to be replaced
    threads.reserve(concurrency);

    for (auto a = zero(concurrency); a < concurrency; ++a)
    {
        // emplace_back so thread is constructed in place
        threads.emplace_back([this]()
            {
                // checks whether parent threadpool is being destroyed,
                // if it is, stop running.
                while (!shutdown_flag.load())
                {
                    auto current_task_package = std::unique_ptr<task_package>{nullptr};

                    // use pop_task so we only ever have one reference to the
                    // task_package
                    if (pop_task(current_task_package))
                    {
                        try
                        {
                            current_task_package->task();
                            current_task_package->completion_promise.set_value();
                        }
                        catch (...)
                        {
                            // try and tell the owner that something bad has happened...
                            try
                            {
                                // ...but this can also throw, so stay protected
                                current_task_package->completion_promise.set_exception(std::current_exception());
                            }
                            catch (...) { }
                        }
                    }
                    else
                    {
                        // rather than spinning, give up thread time to other things
#ifdef USE_YIELD
                        std::this_thread::yield();
#else
                        auto lock = std::unique_lock<std::mutex>(wakeup_mutex);

                        wakeup_flag = false;

                        wakeup_signal.wait(lock, [this](){ return wakeup_flag; });
#endif
                    }
                }
            });

    }
};

threadpool::~threadpool()
{
    // signal that threads should not perform any new work
    shutdown_flag.store(true);

#ifndef USE_YIELD
    {
        std::lock_guard<std::mutex> lock(wakeup_mutex);
        wakeup_flag = true;
        wakeup_signal.notify_all();
    }
#endif

    // wait for work to complete then destroy thread
    for (auto && thread : threads)
    {
        thread.join();
    }

    auto current_task_package = std::unique_ptr<task_package>{nullptr};

    // signal to each uncomplete task that it will not complete due to
    // threadpool destruction
    while (pop_task(current_task_package))
    {
        try
        {
            auto except = std::runtime_error("Could not perform task before threadpool destruction");
            current_task_package->completion_promise.set_exception(std::make_exception_ptr(except));
        }
        catch (...) { }
    }
};

std::future<void> threadpool::run(std::function<void()> && task)
{
    auto promise = std::promise<void>{};
    auto future = promise.get_future();

    // ensures no memory leak if push throws (it shouldn't but to be safe)
    auto package = std::make_unique<task_package>();

    package->completion_promise = std::move(promise);
    package->task = std::forward<std::function<void()> >(task);

    tasks.push(package.get());

    // no longer in danger, can revoke ownership so
    // tasks is not left with dangling reference
    package.release();

#ifndef USE_YIELD
    {
        std::lock_guard<std::mutex> lock(wakeup_mutex);
        wakeup_flag = true;
        wakeup_signal.notify_all();
    }
#endif

    return future;
};

inline bool threadpool::pop_task(std::unique_ptr<task_package> & out)
{
    task_package * temp_ptr = nullptr;

    if (tasks.pop(temp_ptr))
    {
        out = std::unique_ptr<task_package>(temp_ptr);
        return true;
    }
    return false;
}
