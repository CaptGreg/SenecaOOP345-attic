// http://en.cppreference.com/w/cpp/thread/packaged_task

// GB modified to figure out when run as a thread
// GB packaged_task's are not run as thread.  They can be 
// GB passed to thread/join just like any other function.

#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <functional>
 
std::thread::id tidMain = std::this_thread::get_id();   // GB

// unique function to avoid disambiguating the std::pow overload set
int f(int x, int y) 
{ 
  if(tidMain == std::this_thread::get_id()) 
  std::cout << "f: NOT threaded\n"; else std::cout << "f: Threaded\n";
  return std::pow(x,y); 
}
 
void task_lambda()
{
    std::cout << "\ntask_lambda\n";
    std::packaged_task<int(int,int)> task([](int a, int b) {
        if(tidMain == std::this_thread::get_id()) 
        std::cout << "lambda: NOT threaded\n"; else std::cout << "lambda: Threaded\n";
        return std::pow(a, b); 
    });
    std::future<int> fut = task.get_future();
 
    task(2, 9);
 
    std::cout << "task_lambda:\t" << fut.get() << '\n';
}
 
void task_bind()
{
    std::cout << "\ntask_bind\n";
    std::packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> fut = task.get_future();
 
    task();
 
    std::cout << "task_bind:\t" << fut.get() << '\n';
}
 
void task_thread()
{
    std::cout << "\ntask_thread\n";
    std::packaged_task<int(int,int)> task(f);
    std::future<int> fut = task.get_future();
 
    std::thread task_td(std::move(task), 2, 10);
    task_td.join();
 
    std::cout << "task_thread:\t" << fut.get() << '\n';
}
 
int main()
{
    std::cout << "\nmain start\n";
    task_lambda();
    task_bind();
    task_thread();
    std::cout << "\nmain stop\n";
}
