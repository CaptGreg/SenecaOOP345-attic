#ifndef __POOL_H__
#define __POOL_H__

#include "Mutex.h"
#include <queue>
#include <unistd.h>

template <class T> class Pool
{
	private:
		std::queue<T> data;
		Mutex mtx;
		size_t max_size;
		size_t spin_time;
	public:
		Pool(size_t max_size = 10, size_t spin_time = 50)
			: max_size(max_size), spin_time(spin_time)
			{}
		~Pool()
			{
				mtx.lock();
				while (data.size()) data.pop();
				mtx.unlock();
			}
		size_t size() const
			{
				mtx.lock();
				size_t result = data.size();
				mtx.unlock();
				return result;
			}
		void push(T item)
			{
				mtx.lock();
				while (data.size() >= max_size)
				{
					mtx.unlock();
					usleep(spin_time);
					mtx.lock();
				}
				data.push(item);
				mtx.unlock();
			}
		T pop()
			{
				mtx.lock();
				while (data.size() <= 0)
				{
					mtx.unlock();
					usleep(spin_time);
					mtx.lock();
				}
				T item = data.front(); data.pop();
				mtx.unlock();
				return item;
			}
};

#endif
