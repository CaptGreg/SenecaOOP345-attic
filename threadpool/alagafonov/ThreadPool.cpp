// http://alexagafonov.com/programming/thread-pool-implementation-in-c-11/

#include "ThreadPool.h"

// Constructor.
ThreadPool::ThreadPool(int threads) :
	terminate(false),
	stopped(false)
{
    // Create number of required threads and add them to the thread pool vector.
    for(int i = 0; i < threads; i++)
    {
    	threadPool.emplace_back( thread(&ThreadPool::Invoke, this) );
    }
}

void ThreadPool::Enqueue(function<void()> f)
{
	// Scope based locking.
	{
		// Put unique lock on task mutex.
		unique_lock<mutex> lock(tasksMutex);

		// Push task into queue.
		tasks.push(f);
	}

	// Wake up one thread.
	condition.notify_one();
}

void ThreadPool::Invoke() {

	function<void()> task;
	while(true)
	{
		// Scope based locking.
		{
			// Put unique lock on task mutex.
			unique_lock<mutex> lock(tasksMutex);

			// Wait until queue is not empty or termination signal is sent.
			condition.wait(lock, [this]{ return !tasks.empty() || terminate; });

			// If termination signal received and queue is empty then exit else continue clearing the queue.
			if (terminate && tasks.empty())
			{
				return;
			}

			// Get next task in the queue.
			task = tasks.front();

			// Remove it from the queue.
			tasks.pop();
		}

		// Execute the task.
		task();
	}
}

void ThreadPool::ShutDown()
{
	// Scope based locking.
	{
		// Put unique lock on task mutex.
		unique_lock<mutex> lock(tasksMutex);

		// Set termination flag to true.
		terminate = true;
	}

	// Wake up all threads.
	condition.notify_all();

	// Join all threads.
	for(thread &thread : threadPool)
	{
		thread.join();
	}

	// Empty workers vector.
	threadPool.empty();

	// Indicate that the pool has been shut down.
	stopped = true;
}

// Destructor.
ThreadPool::~ThreadPool()
{
	if (!stopped)
	{
		ShutDown();
	}
}
