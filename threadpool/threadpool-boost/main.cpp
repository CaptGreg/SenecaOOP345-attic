#include <iostream>
#include <chrono>
#include <queue>
#include <numeric>
#include "threadpool.hpp"

threadpool pool1;
threadpool pool2;
//  pool3 is used as a parasite, increasing the number threads it uses
//  pronounces the negative effect of spinning
threadpool pool3(4 * std::thread::hardware_concurrency());

std::atomic_flag cout_flag = ATOMIC_FLAG_INIT;

int main()
{
    auto results1 = std::queue< std::future<void> >();
    auto results2 = std::queue< std::future<void> >();

    auto lambda1 = []()
    {
        while (cout_flag.test_and_set(std::memory_order_acquire)) ;
        std::cout << "running on pool1 threadid: " << std::this_thread::get_id() << std::endl;
        cout_flag.clear(std::memory_order_release);
    };
    auto lambda2 = []()
    {
        while (cout_flag.test_and_set(std::memory_order_acquire)) ;
        std::cout << "running on pool2 threadid: " << std::this_thread::get_id() << std::endl;
        cout_flag.clear(std::memory_order_release);
    };

    auto times = std::vector<std::chrono::nanoseconds>{};
    times.reserve(10000);

    for (int j = 0; j < 10000; ++j)
    {
        cout_flag.test_and_set(std::memory_order_acquire);
        std::cout << "round " << j << std::endl;

        for (unsigned u = 0; u < 3; ++u)
        {
            results1.push(pool1.run(lambda1));
            results2.push(pool2.run(lambda2));
        }

        int i = 0;

        auto start = std::chrono::steady_clock::now();

        cout_flag.clear(std::memory_order_release);

        //  main loop
        while (i++ < 100)
        {
            auto & future1 = results1.front();
            future1.get();
            results1.pop();
            results1.push(pool1.run(lambda1));

            auto & future2 = results2.front();
            future2.get();
            results2.pop();
            results2.push(pool2.run(lambda2));
        }

        while (!results1.empty())
        {
            results1.front().get();
            results1.pop();
        }
        while (!results2.empty())
        {
            results2.front().get();
            results2.pop();
        }

        auto finish = std::chrono::steady_clock::now();

        times.push_back(finish - start);
    }

    auto average = std::accumulate(std::begin(times), std::end(times), std::chrono::nanoseconds{}) / (206 * times.size());

    std::cout << "Average time per task: " << static_cast<double>(average.count()) / 1000 << " us" << std::endl;
}
