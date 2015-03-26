#ifndef __PRODUCER_H__
#define __PRODUCER_H__

#include "Thread.h"
#include "Pool.h"
#include <string>

using namespace std;

class Producer : public Thread
{
	private:
		Pool<string> * pool;
		string id;
		size_t num_items;
	protected:
		virtual void run();
	public:
		Producer(Pool<string> * pool, string id)
			: pool(pool), id(id), num_items(0) {}
		void produce(size_t);
};

#endif
