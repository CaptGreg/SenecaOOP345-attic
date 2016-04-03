// derived from http://www.slideshare.net/AntoineSavine/multithreading-c

#include <queue>
#include <mutex>
#include <condition_variable> // GB  - needed for tsqueue Pop that waits if q empty

// What is the difference between lock_guard and unique_lock?
// The difference is that you can lock and unlock a std::unique_lock. 
// Must use a std::unique_lock for condition variables.
// std::lock_guard will be locked only once on construction and unlocked on destruction.

// Always use lock_guard, unless you need the capabilities of unique_lock. 
// A condition_variable needs a unique_lock.

template <typename T>
class tsQueue {  // thread safe queue
protected:
  std::queue<T>      q;
  mutable std::mutex m;
public:
  virtual ~tsQueue() {
    std::lock_guard<std::mutex> smartLock(m);
    while( not q.empty() ) {
      q.pop();
    }
  }
  bool empty() const {
    std::lock_guard<std::mutex> smartLock(m);
    return q.empty();
  }
  void push( T& t ) {
    std::lock_guard<std::mutex> smartLock(m);
    q.push( std::move( t ) );
  }
  bool tryPop( T& t ) {
    std::lock_guard<std::mutex> smartLock(m);
    if(q.empty()) return false;
    t = std::move( q.front() );
    q.pop();
    return true;
  }
};

template <typename T>  // GB
class tsQueueWait : public tsQueue<T> {  // tsqueue Pop that waits for Push if q empty
  std::condition_variable cv;
  bool shutdown;
public:
  tsQueueWait() : shutdown(false) {}

  virtual ~tsQueueWait() {
    shutdown = true;
    cv.notify_all();
  }

  void Push( T& t ) {
    std::lock_guard<std::mutex> smartLock(tsQueue<T>::m);
    tsQueue<T>::q.push( std::move( t ) );
    cv.notify_one();
  }

  bool Pop( T& t ) {
    std::unique_lock<std::mutex> smartLock(tsQueue<T>::m);
    while(tsQueue<T>::q.empty() && not shutdown)
      cv.wait(smartLock);

    if(shutdown) return false;

    t = std::move( tsQueue<T>::q.front() );
    tsQueue<T>::q.pop();
    return true;
  }
};

// includes for test program
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <sstream>

int main()
{
// runtime error: 
// terminate called without an active exception

// Declaring 'std::vector<std::thread> tidVector(N);' was the problem.
// Works fine declared as 'std::vector<std::thread> tidVector;'

// Reason:  queues N thread reads and N thread writes
// vector<thread> tidVector(N) should be 2*N, not N

  const int N = 1000;
  try {
    // tsQueue<int> q;
    tsQueueWait<int> q;  // default code works with tsQueueWait
  
    std::vector<std::thread> tidVector;
   
    auto pushQ= [&q] (int i) {q.push(i);};
    auto popQ = [&q] (int i)
      {
        int tries = 1;
        int ii;
        while(q.tryPop(ii) == false)
          tries++;
        std::string s = "thread " + std::to_string(i) + " popped " + std::to_string(ii);
        if(tries != 1) {
          s += " after " + std::to_string(tries) + " tries";
        }
        s += "\n";
        std::cout << s;
      }; 
 
    // test out the tryPop mechanism.
    // queue the pops before the pushes.
    // Q N reads
    for(int i = 0; i < N ; i++) {
      tidVector.push_back( std::thread( popQ, i ) );
    }
    // issue N writes
    for(int i = 0; i < N ; i++) {
      tidVector.push_back( std::thread( pushQ, i ) );
    }
    for(auto& e : tidVector) e.join();

    std::cout << "\n";
  } catch(std::exception& e) {
    std::cerr << "program threw: " << e.what() << "\n";
  } catch(...) {
    std::cerr << "program threw unspecified error" << "\n";
  }

  try {
    tsQueueWait<int> q;
  
    std::vector<std::thread> tidVector;
    auto PushQ= [&q] (int t) {q.Push(t);};
    auto PopQ = [&q] (int t)
      {
        int i;
        q.Pop(i);
        std::cout << "thread " +  std::to_string(t) + " popped " + std::to_string(i) + " (after possible wait)\n";
      }; 

    // Q N reads
    for(int i = 0; i < N ; i++) {
      tidVector.push_back( std::thread( PopQ, i ) );
    }
    // issue N writes
    for(int i = 0; i < N ; i++) {
      tidVector.push_back( std::thread( PushQ, i ) );
    }
    for(auto& e : tidVector) e.join();


  } catch(std::exception& e) {
    std::cerr << "program threw: " << e.what() << "\n";
  } catch(...) {
    std::cerr << "program threw unspecified error" << "\n";
  }
  return 0;
}
