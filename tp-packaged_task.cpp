// http://stackoverflow.com/questions/31072279/implementing-a-simple-generic-thread-pool-in-c11 and code:
// https://github.com/horazont/scc/blob/7a93301ae04635924d1ddda8f5ea3ee7651d7b73/libffengine-core/src/common/utils.cpp


// suggested improvement:
// Take F&& instead of packaged_task. 
// Cast to std::decay<F>::type. 
// Replace void* with unique_ptr<void,void(*)(void*)>, 
// remove dtor and m_call_delete. 
// Replace m_call_exec type with void(*)(abstract_packaged_task*). 
// Result: simpler code, faster code, works on any callable. 
// Class becomes movable and not copyable. – Yakk Jun 26 '15 at 13:54

/**********************************************************************
File name: utils.cpp
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
#include <iostream>
#include "tp-packaged_task.h"

// #include "ffengine/io/log.hpp"  // GB - try without the logging code


namespace engine {

// static io::Logger &nw_logger = io::logging().get_logger("common.utils.NotifiableWorker");


NotifiableWorker::NotifiableWorker():
    m_notified(false),
    m_terminate(false)
{

}

NotifiableWorker::~NotifiableWorker()
{
    tear_down();
}

void NotifiableWorker::worker_thread()
{
    std::unique_lock<std::mutex> state_lock(m_state_mutex);
    // nw_logger.logf(io::LOG_DEBUG, "%p started", this);
    while (!m_terminate)
    {
        while (!m_notified) {
            m_wakeup.wait(state_lock);
        }
        m_notified = false;
        state_lock.unlock();
        // nw_logger.logf(io::LOG_DEBUG, "%p woke up", this);

        const bool call_again = worker_impl();

        state_lock.lock();
        m_notified = m_notified || call_again;
    }
    // nw_logger.logf(io::LOG_DEBUG, "%p stopped", this);
}

void NotifiableWorker::start()
{
    if (m_worker_thread.joinable()) {
        return;
    }
    m_worker_thread = std::thread(std::bind(&NotifiableWorker::worker_thread,
                                            this));
}

void NotifiableWorker::tear_down()
{
    if (m_worker_thread.joinable()) {
        // nw_logger.logf(io::LOG_DEBUG, "stopping %p", this);
        {
            std::lock_guard<std::mutex> guard(m_state_mutex);
            m_terminate = true;
        }
        m_worker_thread.join();
    }
}

void NotifiableWorker::notify()
{
    // nw_logger.logf(io::LOG_DEBUG, "notifying %p", this);
    {
        std::lock_guard<std::mutex> guard(m_state_mutex);
        m_notified = true;
    }
    m_wakeup.notify_all();
}


ThreadPool::abstract_packaged_task::abstract_packaged_task(
        abstract_packaged_task &&other):
    m_task(std::move(other.m_task)),
    m_call_exec(std::move(other.m_call_exec)),
    m_call_delete(std::move(other.m_call_delete))
{

}

ThreadPool::abstract_packaged_task::~abstract_packaged_task()
{
    if (m_call_delete != nullptr) {
        m_call_delete(this);
    }
}

void ThreadPool::abstract_packaged_task::operator ()()
{
    m_call_exec(this);
}


ThreadPool::ThreadPool():
    m_terminated(false)
{
    initialize_workers(std::thread::hardware_concurrency());
}

ThreadPool::ThreadPool(unsigned int workers):
    m_terminated(false)
{
    initialize_workers(workers);
}

ThreadPool::~ThreadPool()
{
    stop_all();
}

void ThreadPool::initialize_workers(unsigned workers)
{
    m_workers.reserve(workers);
    for (unsigned iNumber = 0; iNumber < workers; iNumber++)
    {
        m_workers.emplace_back(std::bind(&ThreadPool::worker_impl,
                                         this, iNumber));
    }
}

void ThreadPool::stop_all()
{
  std::cout <<"ThreadPool::stop_all()\n";
    m_terminated = true;
    m_queue_wakeup.notify_all();
    for (auto &worker: m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    m_workers.clear();
}

void ThreadPool::worker_impl(unsigned thread)
{
    std::unique_lock<std::mutex> lock(m_queue_mutex, std::defer_lock);
    std::cout << std::string("worker ") + std::to_string(thread) + " started.\n";
    while (!m_terminated) {
        lock.lock();
        while (m_task_queue.empty()) {
            m_queue_wakeup.wait(lock);
            if (m_terminated) {
                return;
            }
        }
        abstract_packaged_task task(std::move(m_task_queue.front()));
        m_task_queue.pop_front();
        lock.unlock();

        task();

        {
          std::lock_guard<std::mutex> lock(m_queue_mutex); // GB
          if (m_task_queue.empty()) {  // GB
            m_terminated = true;
            break;
          }
        }
    }
    std::cout << std::string("worker ") + std::to_string(thread) + " finished.\n";
}

} // namespace engine 

#define TEST
#ifdef TEST
#include <iostream>
#include <chrono>
#include <mutex>
#include <future>
#include <string>
#include <sstream>
using namespace std;

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t msecs() {
    typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    return duration_get.count();
  }
  uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    return duration_get.count();
  }
  uint64_t nsecs() {
    typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    return duration_get.count();
  }
};

int main()
{
  cout << "This machine has " << thread::hardware_concurrency()  << " cores.\n";
  engine :: ThreadPool tp( thread::hardware_concurrency() );
  // engine :: ThreadPool tp( );
  mutex coutLock;
  Timer t;
  t.Start();
  t.Stop();
  cout << t.usecs() << ": -PROGRAM START-\n";

  for(int j = 0; j < 17; j++) { // submit 17 jobs
    auto job = [j, &coutLock, &t] ()-> void {
      t.Stop();
      {
        lock_guard<mutex> smartLock(coutLock); // lock the screen
        cout << t.usecs() << ": job "
             << j << " start.\n";
      }
      int sleep = (rand() % 1000000) + 750000;
      this_thread::sleep_for(chrono::microseconds(sleep));
      t.Stop();
      {
        lock_guard<mutex> smartLock(coutLock); // lock the screen
        cout << t.usecs() << ": job "
             << j << " finished: sleep time was " << sleep * 1e-6 << " secs.\n";
      }
    };
    std::packaged_task<void()> tsk ( job );
    tp.submit_task( std::move( tsk ) );
  }

  unsigned workers;
  while( (workers = tp.workers()) > 0 ) {
      t.Stop();
      cout << to_string(t.usecs()) + ": main " + to_string(workers) + " active\n";
      this_thread::sleep_for(chrono::milliseconds(200));
      t.Stop();
      if( tp.taskQueueEmpty() ) {
        cout << to_string(t.usecs()) + ": task queue empty.\n";
        break;
      }
  }
  t.Stop();
  cout << t.usecs() << ": -PROGRAM FINISHED-\n";
}
#endif

#if 0
// http://www.cplusplus.com/reference/future/packaged_task/

// packaged_task example
#include <iostream>     // std::cout
#include <future>       // std::packaged_task, std::future
#include <chrono>       // std::chrono::seconds
#include <thread>       // std::thread, std::this_thread::sleep_for

// count down taking a second for each value:
int countdown (int from, int to) {
  for (int i=from; i!=to; --i) {
    std::cout << i << '\n';
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::cout << "Lift off!\n";
  return from-to;
}

int main ()
{
  std::packaged_task<int(int,int)> tsk (countdown);   // set up packaged_task
  std::future<int> ret = tsk.get_future();            // get future

  std::thread th (std::move(tsk),10,0);   // spawn thread to count down from 10 to 0

  // ...

  int value = ret.get();                  // wait for the task to finish and get result

  std::cout << "The countdown lasted for " << value << " seconds.\n";

  th.join();

  return 0;
}
#endif
