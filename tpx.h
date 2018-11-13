#pragma once

#include <condition_variable>
#include <functional>             // std::function
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  std::vector<std::thread>            threads;
  std::queue< std::function<void()> > jobQueue;
  std::mutex                          mtx;
  std::condition_variable             cv;
  bool                                quit     = false; // C++14
  bool                                stopped  = false; // C++14
  void                                Run();  // private member function
public:
  ThreadPool(int numberOfThreads = std::thread::hardware_concurrency());
  ~ThreadPool();
  void AddJob(std::function<void()> job);
  void ShutDown();
};
