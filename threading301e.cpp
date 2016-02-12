// http://naipc.uchicago.edu/2015/ref/cppreference/en/cpp/thread/packaged_task.html
//
// The class template std::packaged_task wraps any callable target 
// (function, lambda expression, bind expression, or another function object) 
// so that it can be invoked asynchronously. Its return value or exception 
// thrown is stored in a shared state which can be accessed through 
// std::future objects.
//
// Just like std::function, std::packaged_task is a polymorphic, allocator-aware 
// container: the stored callable target may be allocated on heap or with a provided 
// allocator. 
//
#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <functional>   // bind
 
void task_lambda()
{
    std::packaged_task<int(int,int)> task([](int a, int b) {
        return pow(a, b); 
    });
    std::future<int> result = task.get_future();
 
    task(2, 9);
 
    std::cout << "task_lambda:\t" << result.get() << '\n';
}
 
void task_bind()
{
    std::packaged_task<int()> task(std::bind(pow, 2, 11));
    std::future<int> result = task.get_future();
 
    task();
 
    std::cout << "task_bind:\t" << result.get() << '\n';
}
 
void task_thread()
{
    std::packaged_task<int(int,int)> task(pow);
    std::future<int> result = task.get_future();
 
    std::thread task_td(std::move(task), 2, 10);
    task_td.join();
 
    std::cout << "task_thread:\t" << result.get() << '\n';
}
 
int main()
{
    task_lambda();
    task_bind();
    task_thread();
}
