// http://stackoverflow.com/questions/31072279/implementing-a-simple-generic-thread-pool-in-c11 and code:
// https://github.com/horazont/scc/blob/7a93301ae04635924d1ddda8f5ea3ee7651d7b73/libffengine-core/ffengine/common/utils.hpp

/**********************************************************************
File name: utils.hpp
This file is part of: SCC (working title)

LICENSE

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.

FEEDBACK & QUESTIONS

For feedback and questions about SCC please e-mail one of the authors named in
the AUTHORS file.
**********************************************************************/
#ifndef SCC_ENGINE_COMMON_UTILS_HPP
#define SCC_ENGINE_COMMON_UTILS_HPP

#include <atomic>
#include <cassert>
#include <cerrno>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <stdexcept>
#include <system_error>
#include <thread>
#include <vector>

namespace engine {

class NotifiableWorker
{
public:
    NotifiableWorker();
    virtual ~NotifiableWorker();

private:
    std::mutex m_state_mutex;
    bool m_notified;
    bool m_terminate;
    std::condition_variable m_wakeup;

    std::thread m_worker_thread;

private:
    void worker_thread();

protected:
    void start();
    void tear_down();

    /**
     * Actual implementation of the work to do.
     *
     * @return \c true, if it needs to be called again immediately, independent
     * of notifications, \c false otherwise.
     */
    virtual bool worker_impl() = 0;

public:
    void notify();

};


class ThreadPool
{
public:
    ThreadPool();
    explicit ThreadPool(unsigned int workers);
    ~ThreadPool();

private:
    struct abstract_packaged_task
    {
        template <typename R>
        abstract_packaged_task(std::packaged_task<R> &&task):
            m_task((void*)(new std::packaged_task<R>(std::move(task)))),
            m_call_exec([](abstract_packaged_task *instance)mutable{
                (*(std::packaged_task<R>*)instance->m_task)();
            }),
            m_call_delete([](abstract_packaged_task *instance)mutable{
                delete (std::packaged_task<R>*)(instance->m_task);
            })
        {

        }

        abstract_packaged_task(abstract_packaged_task &&other);

        ~abstract_packaged_task();

        void operator()();

        void *m_task;
        std::function<void(abstract_packaged_task*)> m_call_exec;
        std::function<void(abstract_packaged_task*)> m_call_delete;
    };

    std::atomic<bool>                  m_terminated;

    std::mutex                         m_queue_mutex;
    std::condition_variable            m_queue_wakeup;
    std::deque<abstract_packaged_task> m_task_queue;

    std::vector<std::thread>           m_workers;

protected:
    void initialize_workers(unsigned int workers);
    void stop_all();
    void worker_impl(unsigned tNumber);

public:
    template <typename R>
    std::future<R> submit_task(std::packaged_task<R()> &&task)
    {
        assert(m_workers.size() > 0);
        std::future<R> result = task.get_future();
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_task_queue.emplace_back(std::move(task));
        }
        m_queue_wakeup.notify_one();
        return result;
    }

    inline unsigned int workers() const
    {
        return m_workers.size();
    }

    bool taskQueueEmpty() // GB
    {  
      std::lock_guard<std::mutex> lock(m_queue_mutex);
      return m_task_queue.empty(); 
    }

};


/**
 * Raise the last OS error (as detected by querying errno) as
 * std::system_error.
 */
static inline void raise_last_os_error()
{
    const int err = errno;
    if (err != 0) {
        throw std::system_error(err, std::system_category());
    }
}

}

#endif // SCC_ENGINE_COMMON_UTILS_HPP

