// https://gist.github.com/iikuy/8115191

// GB hacked

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::mutex              mx;
std::condition_variable cv;
std::queue<int>         q;

bool finished = false;

void producer(int n) 
{
	for(int i=0; i<n; ++i) {
		{ 
      std::lock_guard<std::mutex> lk(mx); // acquire lock
			q.push(i);
			std::cout << "pushing " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
		} // release lock
		cv.notify_all(); // GB release consumer
	}
  { 
    std::lock_guard<std::mutex> lk(mx); // acquire lock
		finished = true;
	} // release lock
	cv.notify_all();
}

void consumer() 
{
	do {
		std::unique_lock<std::mutex> lk(mx);
		cv.wait(lk, []{ return finished || !q.empty(); });
		std::cout << "consumer passed cv wait: q size=" <<  q.size() << "\n";
		while (!q.empty()) {
			std::cout << "consuming " << q.front() << std::endl;
      q.pop();
    }
  } while (not finished);
}

int main1() 
{
  std::thread p(producer, 10);
  std::thread c(consumer);
  p.join();
  c.join();
	std::cout << "main1 finished!" << std::endl;
  return 0;
}

int main2() 
{
// GB hack
// http://www.cplusplus.com/reference/condition_variable/condition_variable/wait/

  int count = 0;
  int cargo = 0;
  bool halt = false;
  auto shipment_available =[cargo] { return cargo != 0;};

  auto consume =[&] (int n) {
    while(true) {
      std::unique_lock<std::mutex> lck(mx);
      // cv.wait(lck,[&] {shipment_available() and not halt}););
      while(not shipment_available() and not halt)
        cv.wait(lck);
      if(halt) break;
      // consume:
      // std::cout << cargo << '\n';
      cargo=0;
      count++;
    }
  };

  auto produce = [&] {
    while (true) {
      while (not halt and shipment_available()) 
        std::this_thread::yield();
      if(halt) 
        break;
      std::unique_lock<std::mutex> lck(mx);
      cargo++;
      cv.notify_one();
    }
  };

  std::thread consumer_thread (consume,10);
  std::thread producer_thread (produce);

  int ms = 1000;
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  halt = true;
  cv.notify_one();
  producer_thread.join();
  consumer_thread.join();

	std::cout << "main2 finished!, count=" << count << std::endl;
  return 0;
}
int main() 
{
  main1();
  main2();
  return 0;
}
