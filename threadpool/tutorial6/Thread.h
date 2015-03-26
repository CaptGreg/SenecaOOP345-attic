#ifndef __THREAD_H__
#define __THREAD_H__

// #include <pthread.h>
#include <thread>

class Thread
{
	private:
		//pthread_t thread;
		std::thread thread;
		static void * dispatch(void *);
	protected:
		virtual void run() = 0;
	public:
		virtual ~Thread();
		void start();
		void join();
};

#endif
