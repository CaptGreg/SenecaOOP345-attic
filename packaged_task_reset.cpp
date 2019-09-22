#include <cmath>
#include <functional>
#include <iostream>
#include <future>
#include <thread>
 
int main()
{
  #if 0
    auto function = [](int a, int b) -> int { return std::pow(a, b); };
  #else
    std::function<int(int,int)> function = [](int a, int b) -> int { return std::pow(a, b); };
  #endif

    std::packaged_task<int(int,int)> task(function);
    std::future<int> result;

    result = task.get_future();
    task(2, 9);
    std::cout << "2^9 = " << result.get() << '\n';
 
    task.reset();
    result = task.get_future();
  #if 0
    std::thread task_td(std::move(task), 2, 10);
    task_td.join();
  #else
    task( 2, 10);
  #endif
    std::cout << "2^10 = " << result.get() << '\n';
}
