// g++ --std=c++11 example_2.cpp -o example_2 -pthread

#include <iostream>
#include <vector>
#include <chrono>

#include "Scheduled.hpp"

int func(const int i){
    std::cout << "hello " << i << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for(int x = 0; x < 0xFFFFFFF; x++);
    std::cout << "world " << i << std::endl;
    return i*i;
}


int main(void){
    ThreadPool pool(2);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 10; i++) {
//      results.push_back( pool.submit_at(std::chrono::steady_clock::now() + std::chrono::seconds(5), func, i) );
        results.push_back( pool.submit_after(std::chrono::seconds(5), func, i) );
    }

    for(size_t i = 0; i < results.size(); i++)
        results[i].get();
//        std::cout << results[i].get() << ' ';

    std::cout << std::endl;
    return 0;
}
