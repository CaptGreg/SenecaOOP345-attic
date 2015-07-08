// http://alexagafonov.com/programming/thread-pool-implementation-in-c-11/

#include <iostream>
#include <thread>
#include <chrono>
#include <cctype> // toupper
using namespace std;

#include "ThreadPool.h"

void threadFunction (int num, char c)
{
    for (int i = 0; i < num; i++) {
	std::cerr << c;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {

	// ThreadPool pool(5);
	ThreadPool pool(std::max(1, (int)std::thread::hardware_concurrency()));

	for (int i = 0; i < 20; i++)
	{
		cout << "Enqueing: " << i << endl;
		pool.Enqueue([i](){
			cout << i << " alive" << endl;
			int sleep = (rand() % 1000000) + 1000000;
			usleep(sleep);
			std::this_thread::sleep_for(std::chrono::microseconds(sleep));
			cout << "Processed: " << i << " Sleep Time: " << sleep << " usecs" << endl;
		});
	}
	pool.ShutDown();

	ThreadPool pool2(std::max(1, (int)std::thread::hardware_concurrency()));

	// GB pushes a thread for each letter in vector sample
	// the thread 'print' prints the argument letter 25 times, napping 100 msec after each print
	std::vector<char> sample{'a', 'b', 'c', 'd', 'e', 'f', 'g'}; // 7 elements
	for (auto v: sample) {
		pool2.Enqueue( std::bind(threadFunction, 25, v) );
	}
	
	for (auto v: sample) {
		pool2.Enqueue( std::bind(

		[] (int num, char c) {
		    for (int i = 0; i < num; i++) {
			std::cerr << (char) std::toupper(c);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		    }
		}, 

		25, v) );
	}
	pool2.ShutDown();
	std::cout << "\n";

	return 0;
}
