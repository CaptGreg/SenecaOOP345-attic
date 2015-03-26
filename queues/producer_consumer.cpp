// GB - a simple producer consumer thread pool
//    - threads are created, read the queue, exit when done.
//    - this requires the amount of work to be understood by the producer and the consumer.

// clang++ -std=c++11 producer_consumer.cpp -o producer_consumer -lrt

//http://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/

//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

#include "threadsafe-queue.h"
#include <iostream>

void produce(Queue<int>& q) {
  for (int i = 0; i< 10000; ++i) {
    std::cout << "Pushing " << i << "\n";
    q.push(i);
  }
}

void consume(Queue<int>& q, unsigned int id) {
  for (int i = 0; i< 2000; ++i) {
    auto item = q.pop();
    std::cout << "Consumer " << id << " popped " << item << "\n";
  }
}

#include <thread>
int main()
{
  Queue<int> q;

  using namespace std::placeholders;

  // producer thread
  std::thread prod1(std::bind(produce, std::ref(q)));

  // consumer threads
  std::thread consumer1(std::bind(&consume, std::ref(q), 1));
  std::thread consumer2(std::bind(&consume, std::ref(q), 2));
  std::thread consumer3(std::bind(&consume, std::ref(q), 3));
  std::thread consumer4(std::bind(&consume, std::ref(q), 4));
  std::thread consumer5(std::bind(&consume, std::ref(q), 5));

  prod1.join();
  consumer1.join();
  consumer2.join();
  consumer3.join();
  consumer4.join();
  consumer5.join();

}
