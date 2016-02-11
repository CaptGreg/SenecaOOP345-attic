// producer-consumer 

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

typedef int sample;

sample job = 0;
const int JOB_COUNT = 5;
bool should_produce () { return job < JOB_COUNT; }
const sample produce() { std::cout << "produce " << job << "\n"; return job++; }
void process(sample s) { std::cout << "process " << s << "\n"; }
bool is_last(sample s) { return job+1 >= JOB_COUNT; }

std::mutex              fifo_mutex;
std::queue<sample>      data_fifo;
std::condition_variable data_rdy;

void producer_thread () 
{
  while ( should_produce() ) {
    const sample s = produce();
    std::lock_guard <std::mutex > lk (fifo_mutex);
    data_fifo.push (s);
    data_rdy.notify_all ();
  }
}

void consumer_thread () 
{
  while (true) {
    std::unique_lock <std::mutex > lk ( fifo_mutex);
    data_rdy.wait ( lk , []{return !  data_fifo.empty (); });
    sample s = data_fifo.front ();
    data_fifo.pop();
    lk.unlock () ;
    process(s);
    if ( is_last (s)) break;
  }
}

int main(int argc, char**argv)
{
  std::thread pt( producer_thread );
  std::thread ct( consumer_thread );

  pt.join();
  ct.join();
}
