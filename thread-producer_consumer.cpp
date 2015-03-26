// GB - A trivial, not very interesting, producer consumer thread pool
//    - It is a 'one shot' thread pool.
//    - Threads are created, read the queue, exit when done.
//    - This requires the total amount of work to be understood by the producer and the consumer.
//    - GB pass workUnit size to the consumer and totalWork size to the producer.
//    - GB convert consumer threads to array of threads from fixed calls.
//    - The salient feature is uses 'threadsafe-queue.h'.
//    - The queue uses mutex to lock access and condition variables to wait if the queue is empty.


// clang++ -std=c++11 producer_consumer.cpp -o producer_consumer -lrt

//http://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/

//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

#include <thread>
#include <mutex>   // for screenLock
#include <iostream>
#include "threadsafe-queue.h"

std::mutex screenLock;

void produce(Queue<int>& q, unsigned totalWork) {
  for (int i = 0; i< totalWork; ++i) {
    {std::unique_lock<std::mutex> myTurn(screenLock);
      std::cout << "Pushing " << i << "\n";
    }
    q.push(i);
  }
}

void consume(Queue<int>& q, unsigned id, unsigned workUnits) {
  for (int i = 0; i< workUnits; ++i) {
    auto item = q.pop();
    {std::unique_lock<std::mutex> myTurn(screenLock);
      std::cout << "Consumer " << id << " popped " << item << "\n";
    }
  }
}

int main()
{
  Queue<int> q;

  using namespace std::placeholders;

  unsigned totalWork  = 15; // 10000;
  unsigned numThreads = 5;
  unsigned workUnits  = totalWork / numThreads;
  
  // consumer threads
  // std::thread consumer1( std::bind(&consume, std::ref(q), 1, workUnits) );
  // std::thread consumer2( std::bind(&consume, std::ref(q), 2, workUnits) );
  // std::thread consumer3( std::bind(&consume, std::ref(q), 3, workUnits) );
  // std::thread consumer4( std::bind(&consume, std::ref(q), 4, workUnits) );
  // std::thread consumer5( std::bind(&consume, std::ref(q), 5, workUnits) );

  std::unique_ptr<std::thread[]> consumers(new std::thread[numThreads]);
  for(int t = 0; t < numThreads; t++)
    consumers[t] = std::thread( std::bind(&consume, std::ref(q), t+1, workUnits) );

  // producer thread
  std::thread prod1( std::bind(produce, std::ref(q), totalWork) );

  prod1.join();

  for(int t = 0; t < numThreads; t++)
    consumers[t].join();

  // consumer1.join();
  // consumer2.join();
  // consumer3.join();
  // consumer4.join();
  // consumer5.join();

}
