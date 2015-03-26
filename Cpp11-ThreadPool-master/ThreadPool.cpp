#include <thread>
#include <future>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <deque>
#include <functional>
#include "ThreadPool.hpp"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using namespace thread;

int main(int argc, char**argv)
{
 
  ThreadPool<void(int)> pool;

   for(int i = 0;i<9;++i) // queue a bunch of "work items"
    {
        std::stringstream ss; ss << "main: enqueuing " << i << "\n"; std::cout << ss.str();
        pool.addjob([i] () -> void // compiles fine without the '() -> void'
        // ThreadPool pool([i] () -> void // compiles fine without the '() -> void'
            {
                std::thread::id tid = this_thread::get_id();
                stringstream ss; 
                ss << ">>> hello " << i << " tid=(" << tid << ')' << '\n'; 
                std::cout << ss.str(); 
    
                this_thread::sleep_for (chrono::milliseconds(1000)); 
    
                ss.str("");
                ss << ">>> world " << i << " tid=(" << tid << ')' << '\n'; 
                std::cout << ss.str(); 
            }
        );
    }

}
