// GB - A trivial, not very interesting, producer consumer thread pool
//    - It is a 'one shot' thread pool.
//    - Threads are created, read the queue, exit when done.
//    - This requires the total amount of work to be understood by the producer and the consumer.
//    - GB pass workUnit size to the consumer and totalWork size to the producer.
//    - GB convert consumer threads to array of threads from fixed calls.
//    - The salient feature is uses 'threadsafe-queue.h'.
//    - The queue uses mutex to lock access and condition variables to wait if the queue is empty.


// clang++ -std=c++11 producer_consumer.cpp -o producer_consumer -lrt

// http://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
// https://github.com/juanchopanza/cppblog/blob/master/Concurrency/Queue/producer_consumer1.cpp

//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

// #pragma once
// file threadsafe-queue.h
// http://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//
// GB - 2016 tidied up, converted to use move semantics

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue
{
  std::queue<T>           q;
  std::mutex              m;
  std::condition_variable c;
 public:

  T&& pop() 
  {
    std::unique_lock<std::mutex> mlock(m);
    while (q.empty()) {
      c.wait(mlock);
    }
    auto val = std::move(q.front());
    q.pop();
    return std::move(val);
  }

  void pop(T&& item)
  {
    std::unique_lock<std::mutex> mlock(m);
    while (q.empty()) {
      c.wait(mlock);
    }
    item = std::move(q.front());
    q.pop();
  }

  void push(const T&& item)
  {
    std::unique_lock<std::mutex> mlock(m);
    q.push(std::move(item));
    mlock.unlock();
    c.notify_one();
  }
  Queue()                          = default;
  virtual ~Queue() {}
  Queue(const Queue&)              = delete; // disable copy constructor
  Queue(Queue&&)                   = delete; // disable move constructor
  Queue&  operator= (const Queue&) = delete; // disable copy assignment
  Queue&& operator= (Queue&&)      = delete; // disable move assignment
}; // class Queue

#include <thread>
#include <mutex>   // for screenLock
#include <iostream>
// #include "threadsafe-queue.h"

std::mutex screenLock;

void produce(Queue<unsigned>& q, unsigned totalWork) {
  for (unsigned i = 0; i< totalWork; i++) {
    {std::unique_lock<std::mutex> myTurn(screenLock);
      std::cout << "Pushing " << i << "\n";
    }
    q.push(std::move(i));
  }
}

void consume(Queue<unsigned>& q, unsigned id, unsigned workUnits) {
  for (unsigned i = 0; i< workUnits; i++) {
    auto item = q.pop();
    {std::unique_lock<std::mutex> myTurn(screenLock);
      std::cout << "Consumer " << id << " popped " << item << "\n";
    }
  }
}

int main()
{
  Queue<unsigned> q;

  unsigned TOTALWORK  = 15; // 10000;
  unsigned NUMTHREADS = 5;
  unsigned WORKUNITS  = TOTALWORK / NUMTHREADS;
  
  // consumer threads
  std::unique_ptr<std::thread[]> consumers(new std::thread[NUMTHREADS]);
  for(unsigned t = 0; t < NUMTHREADS; t++)
    consumers[t] = std::thread( std::bind(&consume, std::ref(q), t+1, WORKUNITS) );

  // producer thread
  std::thread producer( std::bind(produce, std::ref(q), TOTALWORK) );
  producer.join();

  for(unsigned t = 0; t < NUMTHREADS; t++)
    consumers[t].join();
}
