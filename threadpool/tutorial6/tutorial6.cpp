#include "Pool.h"
#include "Producer.h"
#include "Consumer.h"
#include <string>

using namespace std;

int main(void)
{
	Pool<string> pool;
	Producer prod(&pool, "Producer 1");
	Consumer cons0(&pool, "Consumer 0");
	Consumer cons1(&pool, "Consumer 1");

	cons0.consume(5);
	cons1.consume(5);
	prod.produce(10);	

	prod.join();
	cons0.join();
	cons1.join();
	
	return 0;
}

