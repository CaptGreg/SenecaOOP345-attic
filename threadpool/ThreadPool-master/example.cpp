// g++ --std=c++11 example.cpp -o example -pthread

#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.hpp"

int main(void){
    ThreadPool<PRIORITY> pool(2);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 11; i++) {
        results.push_back(
            pool.enqueue(i, [i] {
                std::cout << "hello " << i << std::endl;
//                std::this_thread::sleep_for(std::chrono::seconds(1));
                for(int x = 0; x < 0xFFFFFFF; x++);

                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(size_t i = 0; i < results.size(); i++)
        std::cout << results[i].get() << ' ';

    std::cout << std::endl;
    return 0;
}
