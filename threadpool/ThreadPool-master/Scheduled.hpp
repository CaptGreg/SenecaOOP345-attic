#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <stack>
#include <queue>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>
#include <future>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include <chrono>
#include <condition_variable>

#include <iostream>

typedef std::function<void(void)> task_t;

struct scheduled_task{
    std::chrono::steady_clock::time_point time;
    task_t task;

    scheduled_task(std::chrono::steady_clock::time_point p, task_t&& f) : time(p), task(f) {}

    bool operator< (const scheduled_task& other) const{
        return this->time > other.time;
    }

    void operator()(void){
        task();
    }
};

class ThreadPool {
private:
    std::vector<std::thread> mWorkers;
    std::priority_queue<scheduled_task> mTasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> isActive;
public:
    ThreadPool (size_t numThreads = std::thread::hardware_concurrency() ) : isActive(true){
        for(size_t i = 0 ; i < numThreads; i++){
            mWorkers.emplace_back(std::thread(
            [this] {
                while(true){
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    while( (this->isActive.load() && this->mTasks.empty()) || (!mTasks.empty() && mTasks.top().time > std::chrono::steady_clock::now()) ){
                        if( this->mTasks.empty() ){
                            this->condition.wait(lock);
                        }
                        else{
                            this->condition.wait_until(lock, mTasks.top().time);
                        }
                    }
                    if( ! this->isActive.load() && this->mTasks.empty())
                        return;
                    auto lNextTask = std::move (this->mTasks.top());
                    this->mTasks.pop();
                    lock.unlock();
                    lNextTask();
                }
            }
            ));
        }
    }

    ThreadPool(ThreadPool& to_copy) = delete; //Probably wouldn't and shouldn't copy this.
    ThreadPool& operator =(ThreadPool& to_copy) = delete;

    template<class F, class... Args>
    auto submit_at(std::chrono::steady_clock::time_point priority, F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        typedef typename std::result_of<F(Args...)>::type return_type;
        // Don't allow enqueueing after stopping the pool
        if ( ! isActive.load() )
            throw std::runtime_error("enqueue on stopped ThreadPool");

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...) );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mTasks.push(scheduled_task(priority, [task](void){ (*task)();}));
        }
        condition.notify_one();
        return res;
    }

    template<class F, class... Args>
    auto submit_after(std::chrono::seconds dura, F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        return submit_at(std::chrono::steady_clock::now() + dura, std::forward<F>(f), args...); //Forward?
    }

    int pending(void){
        std::unique_lock<std::mutex> lock(queue_mutex);
        return this->mTasks.size();
    }

    ~ThreadPool(void){
        this->isActive = false;
        condition.notify_all();
        for(std::thread& t : mWorkers)
            t.join();
    }
};
#endif
