#include "Thread.h"

Thread::~Thread()
{}

void * Thread::dispatch(void * ptr)
{
	if (!ptr) return 0;
	static_cast<Thread *>(ptr)->run();
	// pthread_exit(ptr);
	return 0;
}

void Thread::start()
{
	// pthread_create(&thread, 0, Thread::dispatch, this);
	thread = thread( Thread::dispatch, this);
}

void Thread::join()
{
	// pthread_join(thread, 0);
        thread.join();
}
