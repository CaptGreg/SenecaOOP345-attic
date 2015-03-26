#ifndef __MUTEX_H__
#define __MUTEX_H__

// #include <pthread.h>
#include <mutex>

class Mutex
{
	private:
		// pthread_mutex_t mutex;
		std::mutex _mutex;
	public:
		// Mutex(); // not needed with c++11 std::mutex
		// ~Mutex(); // not needed with c++11 std::mutex
		void lock();
		void unlock();
		bool trylock();
};

#endif
