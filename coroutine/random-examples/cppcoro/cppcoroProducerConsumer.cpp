// https://www.modernescpp.com/index.php/c-20-coroutine-abstraction-with-cppcoro

// cppcororProducerConsumer.cpp

#include <cppcoro/single_consumer_event.hpp>
#include <cppcoro/sync_wait.hpp>
#include <cppcoro/task.hpp>
#include <future>
#include <iostream>

auto start = std::chrono::high_resolution_clock::now();

void at (std::string s) 
{
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = now - start;    // (4)
  std::cout << std::to_string(elapsed.count()) + " : " + s + "\n";
}

cppcoro::single_consumer_event event;  

cppcoro::task<> consumer() 
{
    
    auto start = std::chrono::high_resolution_clock::now();
    
    at(std::string("consumer thread waiting"));
    co_await event;  // suspended until some thread calls event.set()
    at(std::string("consumer thread proceding"));
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Consumer thread waited " << elapsed.count() << " seconds." << std::endl;
  
    co_return;
}

void producer() 
{

    using namespace std::chrono_literals;
    at(std::string("producer thread working"));
    std::this_thread::sleep_for(2s);
    at(std::string("producer thread finished - signalling consumer"));
    
    event.set();  // resumes the consumer  
    
}

int main() 
{
    std::cout << std::endl;
    
    auto con = std::async([]{ cppcoro::sync_wait(consumer()); });  // (1)
    auto prod = std::async(producer);                              // (2)
    con.get(), prod.get();
    
    std::cout << std::endl;
    
    return EXIT_SUCCESS;
}
