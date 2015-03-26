/**
 * This file contains a basic thread pool, built using C++11's standard threading facilities.
 * It allows simple parallel execution of some predefined function with variable arguments.
 *
 * This class implements the basic idea by Andy Prowl which originated at StackOverflow:
 * http://stackoverflow.com/a/15257055/2148672
 *
 * ThreadPool.hpp
 *
 *      Author: Marc Claesen
 */

 /*
Copyright (C) 2013 Marc Claesen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*************************************************************************************************/

#include <thread>
#include <future>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <deque>
#include <functional>

/*************************************************************************************************/

namespace thread{

/*************************************************************************************************/

extern const unsigned NUM_HARDWARE_THREADS{std::thread::hardware_concurrency()};

template <typename T>
class ThreadPool;

template<typename Ret, typename... Args>
class ThreadPool<Ret(Args...)>{
public:
	typedef typename std::deque<std::future<Ret>>::iterator iterator;
	typedef typename std::deque<std::future<Ret>>::const_iterator const_iterator;
	typedef std::function<Ret(Args...)> Fun;

private:
	typedef std::packaged_task<Ret()> job;

	// the job queue
	std::queue<job> jobs;
	std::mutex jobs_mutex;
	std::condition_variable jobs_cv;

	const Fun fun;

	std::deque<std::future<Ret>> futures;
	bool stop_{false}; // When set, this flag tells the threads that they should exit

	// maximum amount of jobs in queue or 0 if unbounded
	const unsigned maxjobs=0; // const to avoid the need for an extra mutex

	// used to signal the threads that add jobs
	std::condition_variable maxjobs_cv;

public:
	/**
	 * The actual execution of jobs is done by workers.
	 */
	class Worker{
	private:
		typedef ThreadPool<Ret(Args...)>::job job;
		ThreadPool<Ret(Args...)> &mgr;
		std::thread t;

		static void thread_func_static(Worker *t){
			std::unique_lock<std::mutex> l(t->mgr.jobs_mutex, std::defer_lock);
			while (true)
			{
				l.lock();

				// Wait until the queue won't be empty or stop is signaled
				t->mgr.jobs_cv.wait(l, [t](){
					return (t->mgr.stop_ || !t->mgr.jobs.empty());
				});

				// Stop was signaled, let's exit the thread
				if (t->mgr.stop_) { return; }

				// Pop one task from the queue...
				job j = std::move(t->mgr.jobs.front());
				t->mgr.jobs.pop();

				// notify potential job adding
				if(t->mgr.maxjobs > 0 && t->mgr.jobs.size() < t->mgr.maxjobs) t->mgr.maxjobs_cv.notify_one();
				l.unlock();

				// Execute the task!
				j();
			}
		};

	public:
		Worker(ThreadPool<Ret(Args...)> &mgr):mgr(mgr){
			t = std::thread(thread_func_static,this);
		};

		friend class ThreadPool<Ret(Args...)>;
	};

private:
	friend class Worker;

	// thread pool
	std::vector<Worker> threads;

public:
	/**
	 * Creates a ThreadPool to execute fun.
	 * 
	 * The default number of threads is std::thread::hardware_concurrency().
	 * Using this constructor allows an infinite job queue.
	 */
	ThreadPool(Fun&& fun):ThreadPool{std::move(fun),std::thread::hardware_concurrency(),0}{};
	/**
	 * Creates a ThreadPool to execute fun with specified number of threads.
	 * 
	 * When maxjobs=0 (default), an infinite job queue is permitted.
	 * When maxjobs>0, ThreadPool::addjob() will block until the queue
	 * is small enough.
	 */
	ThreadPool(Fun&& fun, unsigned numthreads, unsigned maxjobs=0)
	:fun{std::move(fun)},
	 stop_{false},
	 maxjobs{maxjobs}
	{
		// create worker threads
		if(!numthreads) numthreads=1;
		threads.reserve(numthreads);
		for(unsigned i=0; i<numthreads; ++i){
			threads.emplace_back(*this);
		}
	};

	ThreadPool(const ThreadPool<Ret(Args...)> &t) = delete;
	ThreadPool<Ret(Args...)>& operator=(const ThreadPool<Ret(Args...)> &t) = delete;
	ThreadPool<Ret(Args...)>& operator=(ThreadPool<Ret(Args...)>&& t) = default;
	~ThreadPool(){
		join();
	}

	/**
	 * Adds a new job to this ThreadPool's job queue.
	 * 
	 * This may block when the job queue reached its maxmimum length.
	 */
	void addjob(Args... arguments){
		std::unique_lock<std::mutex> lck(jobs_mutex);

		// if there is a max job queue and we reached it, wait for jobs to disappear
		if(maxjobs > 0 && jobs.size() >= maxjobs) maxjobs_cv.wait(lck);
		job newjob(std::bind(fun,arguments...));

		// add the job
		futures.push_back(std::move(newjob.get_future()));
		jobs.push(std::move(newjob));

		// notify a worker thread
		jobs_cv.notify_one();
	}

	/**
	 * Joins the threads managed by this ThreadPool.
	 */ 
	void join(){
		stop();
		for (auto &td: threads){ td.t.join(); }
	}

	/**
	 * Notifies all worker threads to stop.
	 */ 
	void stop(){
		stop_=true;
		jobs_cv.notify_all();
	}

	/**
	 * Wait for all workers to finish.
	 */
	void wait(){
	    for (auto& f : futures) { f.wait(); }
	}

	/**
	 * Erases the list of futures maintained by this ThreadPool.
	 */
	void clear_futures(){ futures.clear(); }

	/**
	 * Returns the number of threads managed by this ThreadPool.
	 */
	unsigned num_threads() const{ return threads.size(); }

	/**
	 * Iterate over the futures of all posted jobs.
	 * Iteration occurs in the order the jobs were added.
	 */
	iterator begin(){ return futures.begin(); }
	iterator end(){ return futures.end(); }
	/**
	 * Iterate over the futures of all posted jobs.
	 * Iteration occurs in the order the jobs were added.
	 */
	const_iterator cbegin() const{ return futures.cbegin(); }
	const_iterator cend() const{ return futures.cend(); }
};

/*************************************************************************************************/

}  // thread namespace

/*************************************************************************************************/
