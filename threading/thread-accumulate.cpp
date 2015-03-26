// http://qtandopencv.blogspot.ca/2013/10/stdasync-stdpackagetask-and-stdpromise.html

#include <algorithm>
#include <future>
#include <iostream>
#include <iterator>
#include <vector>
#include <future>

void test(std::future<int>& input) {
  std::cout << input.get() << std::endl;
}

inline int accumulate(std::vector<int> const &data)
{
  return std::accumulate(std::begin(data), std::end(data), 0);
}

struct Accmulate
{
    int operator()(std::vector<int> const &data)
    {
        return std::accumulate(std::begin(data), std::end(data), 0);
    }
};

inline int max(int a, int b)
{
     return std::max(a, b);
}


int main()
{
  std::cout << "//////////////////////////// std::async\n";
  //////////////////////////// std::async
  std::vector<int> data(100);
  // std::iota(std::begin(data), std::end(data));

  //called by function
  std::future<int> result = std::async(accumulate, std::cref(data));  
  std::cout<<result.get()<<std::endl; //#1

  //called by functor
  Accmulate acc;
  std::future<int> result2 = std::async(&Accmulate::operator(), 
                                              &acc, std::cref(data));
  std::cout<<result2.get()<<std::endl; //#2


  std::cout << "//////////////////////////// std::promise\n";
  std::promise<int> pm;

  std::future<int> result3 = pm.get_future();

  std::thread th1 (test, std::ref(result3));

  pm.set_value (10);
                            
  th1.join();

  std::cout << "//////////////////////////// std::packaged_task\n";
    std::vector<std::packaged_task<int(int, int)>> tasks;
    std::vector<std::future<int>> futures;
    for(size_t i = 0; i != 4; ++i){
        std::packaged_task<int(int, int)> task(&max);
        futures.emplace_back(task.get_future());
        tasks.push_back(std::move(task));
    }
    
    //run the tasks asynchronous
    std::thread t1(std::move(tasks[0]), 3, 4);
    std::thread t2(std::move(tasks[1]), 4, 5);
    //run the tasks concurrent
    tasks[2](5, 4);
    tasks[3](3, 2);

    for(auto &data : futures){
        std::cout<<data.get()<<std::endl;
    }
    t1.join();
    t2.join();
}
