// https://modernescpp.com/index.php/c-20-thread-synchronization-with-coroutines

// conditionVariables.cpp
// g++-10 -fcoroutines -std=c++2a   conditionVariables.cpp -o conditionVariables -lcppcoro -pthread && ./conditionVariables

// sync using conddition_variables, not coroutines


#include <condition_variable>
#include <iostream>
#include <thread>

std::mutex mutex_;
std::condition_variable condVar; 

bool dataReady{false};

void waitingForWork(){
    std::cout << "Waiting " << std::endl;
    std::unique_lock<std::mutex> lck(mutex_);
    condVar.wait(lck, []{ return dataReady; });   // (4)
    std::cout << "Running " << std::endl;
}

void setDataReady(){
    {
        std::lock_guard<std::mutex> lck(mutex_);
        dataReady = true;
    }
    std::cout << "Data prepared" << std::endl;
    condVar.notify_one();                        // (3)
}

int main(){
    
  std::cout << std::endl;

  std::thread t1(waitingForWork);               // (1)
  std::thread t2(setDataReady);                 // (2)

  t1.join();
  t2.join();
  
  std::cout << std::endl;
  
}
