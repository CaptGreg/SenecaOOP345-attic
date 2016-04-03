// https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

// GB 2016-04-03
// 1. Boost code ported to C++11
// 2. add size() function
// 3. add emplace() function
// 4. test program

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename T>
class concurrent_queue
{
private:
  std::queue<T> q;
  mutable std::mutex m;  // error if not mutable
  // error: binding ‘const std::mutex’ to reference of type ‘std::lock_guard<std::mutex>::mutex_type& {aka std::mutex&}’ discards qualifiers
  std::condition_variable cv;
public:
  void push(T const& data)
  {
    // either
    m.lock();
    q.push(data);
    m.unlock();

    // or if there is chance q.push might throw
    // std::unique_lock<std::mutex> lck(m);
    // q.push(data);
    // lck.unlock();

    cv.notify_one();
  }

  void emplace(const T&& data)
  {
    m.lock();
    q.emplace(std::move(data));
    m.unlock();

    cv.notify_one();
  }

  size_t size() const  // GB
  {
    std::lock_guard<std::mutex> lck(m);
    return q.size();
  }

  bool empty() const
  { 
    std::lock_guard<std::mutex> lck(m);  // GB added lock
      return q.empty();
  }

  bool try_pop(T& popped_value)
  {
    std::lock_guard<std::mutex> lck(m);
    if(q.empty())
        return false;
      
    popped_value=q.front();
    q.pop();
    return true;
  }

  void wait_and_pop(T& popped_value)
  {
    std::unique_lock<std::mutex> lck(m);
    while(q.empty())
        cv.wait(lck);

    popped_value=q.front();
    q.pop();
  }
};

int main(int argc, char**argv) // GB test concurrent_queue
{
  concurrent_queue<double> cq;

  int NUM_PROCS = std::thread::hardware_concurrency();

  int size = 100;
  std::thread tid[size];
  for(int i = 0 ; i < size ; i++)
    tid[i] = std::thread([&cq] (int i) { cq.push(double(i)); }, i);

  for(int i = 0 ; i < size ; i++)
    tid[i].join();

  std::cout << "cq.size() = " << cq.size() << "\n";
    
  std::mutex coutMutex;
  for(int t = 0 ; t < NUM_PROCS ; t++)
    tid[t] = std::thread(
    [&cq,&coutMutex] (int t) 
      { 
        uint64_t count      = 0;
        uint64_t busy_count = 0;
        double d;
        while(true) {
          if(cq.try_pop(d)) {
            count++;
            std::this_thread::yield();  // give cpu away
          } else {
            busy_count++;
          }
          if(cq.empty()) break;
        }
        std::lock_guard<std::mutex> lck(coutMutex);
        std::cout << "thread " << t << " popped " << count << " values, busy " << busy_count << " times, remaining q size " << cq.size() << ".\n";
      }, t);

  for(int t = 0 ; t < NUM_PROCS ; t++)
    tid[t].join();

  std::cout << "cq.size() = " << cq.size() << "\n";

  return 0;
}
