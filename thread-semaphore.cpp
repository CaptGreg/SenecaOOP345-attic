// https://www.snip2code.com/Snippet/38266/How-to-make-a-semaphore-using-c--11--Got
// https://gist.github.com/sguzman/9594227

#include <mutex>
#include <condition_variable>
using namespace std;

class semaphore
{
public:

  semaphore(int count_ = 0) : count{count_}
  {}

  void notify()
  {
    unique_lock<mutex> lck(mtx);
    ++count;
    cv.notify_one();
  }

  void wait()
  {
    unique_lock<mutex> lck(mtx);
    while(count == 0)
    {
      cv.wait(lck);
    }

    --count;
  }

private:

  mutex mtx;
  condition_variable cv;
  int count;
};

// From the comments in the github link:

// Note that if you test this, you'll see that this implementation is a "weak" semaphore, and some threads will starve others.
// @TwistedScorpio
// TwistedScorpio commented on Jun 15
// 
// Could you explain how will this starve other threads.
// @AlexMeuer
// AlexMeuer commented 15 days ago • edited
// 
// @TwistedScorpio
// Assume Thread_A acquires the semaphore, and Thread_B and Thread_C are waiting for it.
// When Thread_A releases the semaphore, either B or C will acquire it.
// So now B has the semaphore, we would assume that C will acquire it next.
// However, A has started waiting for the semaphore again. And herein lies the problem.
// A could acquire the semaphore again and C will still be waiting. This can become very serious if C is kept waiting for a long time and can be disastrous with many threads.
// 
// You can solve this by ensuring that each thread acquires the semaphore in the order they started waiting for it. The ticket algorithm is one example of how to accomplish this.

// The ticket algorithm is one example of how to accomplish this.
// GB -code follows-


// http://alexmeuer.github.io/2016/07/12/ticket-algorithm.html
// C++ Ticket Algorithm

// Jul 12, 2016

// This is a basic implementation of the ticket algorithm for multithreading in C++.
// I mean it when I say basic, I programmed this in a single lab for Software Engineering for Games. It had to satisfy a strict layout and rubric so it’s not the nicest looking code but it shouldn’t prove difficult to read or understand (it also may not be 100% perfect, but I got full marks for it).
// The ticket algorithm aims to solve the problem of starvation: when one thread has to wait for ages before it is let acquire the mutex, while another thread (or multiple other threads) might be acquiring it over and over in the mean time. This can be detrimental to your program, you never want to starve out a thread. This algorithm works like a butcher shop or Argos: customers take a ticket and wait for their number to be called. This ensures that everyone gets served in the order they arrive (thus preventing any customer from being ignored in the corner).

/////////////////////////////////////////////////////////////////////////////////
// GB this code runs the threads throught the CS in the same order every time. //
// * tested the CS integrity with a try_lock mutex call.  The CS works.        //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>
 
using namespace std;
 
const int  n = 10;
atomic_int _number;
atomic_int _next;
atomic_int _turn[n];

// GB - don't know about numThreads, not atomic, incremented by func as that thread begins execution.
int        numThreads;

// mutex      coutMutex;   // GB use lock-free I/O method via string
 
mutex      proof;   // GB Test the CS integrity with a try_lock mutex call.

ostringstream data;
 
void func() {
    int i = numThreads++;  //Intentional post-increment
 
    // coutMutex.lock();
    // cout << "Thread " << i << " reporting in." << endl;
    // coutMutex.unlock();
    cout << string("Thread ") + to_string(i) + " reporting in.\n";
 
    this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 500));
 
    while (true) {
        //Take a ticket
        _turn[i] = _number.fetch_add(1);
 
        //Using a mutex for output so that threads don't uppercut each other on the console.
        // coutMutex.lock();
        // cout << "t" << i << "\tturn: " << _turn[i] << endl;
        // coutMutex.unlock();
        cout << string("t") + to_string(i) + "\tturn: " + to_string(_turn[i]) + "\n";
 
        //Slow down the program so that we can read the console.
        this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 500));
 
        uint64_t spins = 0;
        while (_turn[i] != _next) {
            spins++;
            continue;
        }
 
        // GB Test the CS integrity with a try_lock mutex call.
        if(proof.try_lock()) {
          proof.unlock();
        } else {
          cout << "try_lock failed - critical section guard code failed - mutex 'proof' locked by other thread\n";
          exit(1);
        }

        // coutMutex.lock();
        // cout << "t" << i << "\t+CS]\tspins=" << spins << endl;
        // coutMutex.unlock();
        cout << string("t") + to_string(i) + "\t+CS\t" + to_string(spins) + "\n";
 
        //critical section
        data << " t" << i;
 
        //exit protocol
        _next++;
 
        // coutMutex.lock();
        // cout << data.str() << endl;
        // cout << "t" << i << "\tnext = " << _next << endl;
        // coutMutex.unlock();
        cout << data.str() + "\n" + "t" + to_string(i) + "\t-CS next = " + to_string(_next) + "\n";
    }
}
 
 
vector<thread>tidTable(5);

int main(int argc, char* argv[])
{
    srand(time(NULL));
 
    data = ostringstream();
 
    numThreads = 0;
    _number = 1;
    _next = 1;
    for (int i = 0; i < n; i++) {
        _turn[i] = 0;
    }
 
    for(auto &e : tidTable)
      e = thread(func);
    //thread t1 = thread(func);
    //thread t2 = thread(func);
    //thread t3 = thread(func);  //Add as many threads as you like
    //thread t4 = thread(func);
    //thread t5 = thread(func);
 
    // GB function 'func' never returns.  It loops for ever.
    for(auto &e : tidTable)
      e . join();
    // t1.join();
    // t2.join();
    // t3.join();
    // t4.join();
    // t5.join();
 
    return 0;
}

