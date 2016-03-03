// file threadsafe-queue.h
//http://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

#pragma once

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

  T pop() 
  {
    std::unique_lock<std::mutex> mlock(m);
    while (q.empty()) {
      c.wait(mlock);
    }
    auto val = q.front();
    q.pop();
    return val;
  }

  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(m);
    while (q.empty()) {
      c.wait(mlock);
    }
    item = q.front();
    q.pop();
  }

  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(m);
    q.push(item);
    mlock.unlock();
    c.notify_one();
  }
  Queue()=default;
  Queue(const Queue&) = delete;            // disable copying
  Queue& operator=(const Queue&) = delete; // disable assignment
};
