// derived from http://www.slideshare.net/AntoineSavine/multithreading-c

#include <queue>
#include <mutex>

// includes for test program
#include <vector>
#include <thread>
#include <iostream>
#include <sstream>

template <typename T>
class tsQueue {  // thread safe queue
  std::queue<T> q;
  mutable std::mutex m;
public:
  ~tsQueue() {
    std::lock_guard<std::mutex> smartLock(m);
    while( ! q.empty() )
      q.pop();
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

int main()
{
// runtime error: 
// terminate called without an active exception
// add try catch

// No, try-catch did not help but gdb did:
// terminiate called by thread destructor

// Declaring 'std::vector<std::thread> t(N);' was the problem.
// Works fine declared as 'std::vector<std::thread> t;'

  try {
    tsQueue<int> q;
  
    const int N = 1000;
    std::vector<std::thread> t;
   
    auto pushQ= [&q] (int i) {q.push(i);};
    auto popQ = [&q] (int i)
      {
        int tries = 1;
        int ii;
        while(q.tryPop(ii) == false)
          tries++;
        std::ostringstream ss;
        if(tries == 1)
          ss << "thread " << i << " popped " << ii << std::endl;
        else
          ss << "thread " << i << " popped " << ii << " after " << tries << " tries" << std::endl;
        std::cout << ss.str();
      }; 
 
    // test out the tryPop mechanism.
    // queue the pops before the pushes.
    for(int i = 0; i < N ; i++) {
      t.push_back( std::thread( popQ, i ) );
    }
    for(int i = 0; i < N ; i++) {
      t.push_back( std::thread( pushQ, i ) );
    }
    for(auto& e : t) e.join();


    std::cout << std::endl;

  } catch(std::exception& e) {
    std::cerr << "program threw: " << e.what() << std::endl;
  } catch(...) {
    std::cerr << "program threw unspecified error" << std::endl;
  }
}
