// http://naipc.uchicago.edu/2014/ref/cppreference/en/cpp/thread/packaged_task.html

// GB changed pow to ipow.  'pow' generated syntax errors.
// GB added direct and function object calls

#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <functional>
 
int ipow(int a, int b) { return (int) pow(double(a), double(b)); }

class function_object  // functor or function object class with overloaded operator()
{
public :
  int operator () (int a, int b) const { return ipow(a,b); }
};

void task_direct()
{
    std::packaged_task<int(int,int)> task(ipow);
 
    task(2, 1);
    std::cout << "task_direct:\t" << task.get_future().get() << '\n';

    task.reset();
    task(2, 2);
    std::cout << "task_direct:\t" << task.get_future().get() << '\n';
}

void task_fo()
{
    function_object fo;

    std::packaged_task<int(int,int)> task(fo);
 
    task(2, 3);
    std::cout << "task_fo:\t" << task.get_future().get() << '\n';

    task.reset();
    task(2, 4);
    std::cout << "task_fo:\t" << task.get_future().get() << '\n';
}

void task_lambda()
{
    auto f = [](int a, int b) { return ipow(a, b); };
    std::packaged_task<int(int,int)> task(f);
 
    task(2, 5);
    std::cout << "task_lambda:\t" << task.get_future().get() << '\n';

    task.reset();
    task(2, 6);
    std::cout << "task_lambda:\t" << task.get_future().get() << '\n';
}
 
void task_bind()
{
    std::packaged_task<int()> task(std::bind(ipow, 2, 7));
 
    task();
    std::cout << "task_bind:\t" << task.get_future().get() << '\n';
}
 
void task_thread()
{
    std::packaged_task<int(int,int)> task(ipow);
 
    std::thread task_td(std::move(task), 2, 8);
    task_td.join();
 
    std::cout << "task_thread:\t" << task.get_future().get() << '\n';
}
 
int main()
{
    task_direct();
    task_fo();
    task_lambda();
    task_bind();
    task_thread();
}
