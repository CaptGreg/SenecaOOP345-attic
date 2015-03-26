#ifndef __CONSUMER_H__
#define __CONSUMER_H__

#include "Thread.h"
#include "Pool.h"
#include <string>

using namespace std;

class Consumer : public Thread
{
	private:
		static Mutex mtx;
		Pool<string> * pool;
		string id;
		size_t num_items;
	protected:
		virtual void run();
	public:
		Consumer(Pool<string> * pool, string id)
			: pool(pool), id(id), num_items(0) {}
		void consume(size_t);
};

#endif
