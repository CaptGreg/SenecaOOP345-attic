#include "Mutex.h"
#include <mutex>

// Mutex::Mutex() // not needed with c++11 std::mutex
// {
	// pthread_mutex_init(&mutex, 0);
// }

// Mutex::~Mutex() // not needed with c++11 std::mutex
// {
	// pthread_mutex_destroy(&mutex);
// }

void Mutex::lock()
{
	// pthread_mutex_lock(&mutex);
	_mutex.lock();
}

void Mutex::unlock()
{
	// pthread_mutex_unlock(&mutex);
	_mutex.unlock();
}

bool Mutex::trylock()
{
	// return (pthread_mutex_trylock(&mutex) == 0);
	return _mutex.try_lock();
}
