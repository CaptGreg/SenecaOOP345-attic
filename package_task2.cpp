// http://www.slideshare.net/GlobalLogicUkraine/c11-multithreading-futures

#include <iostream>
#include <future>
#include <thread>
#include <queue>
#include <memory>  // shared_ptr

// Asynchronous Provider Object: packaged_task<R,Args...>
class async_exec_service1 {  // slide 18 of 26
  std::queue<std::function<void ()>> _exec_queue;// worker threads will take functions from _exec_queue
  // ...
  void enqueue( std::function<void ()> task) {/* put task to  _exec_queue; */ }
public:
  template <typename _Function>
  auto exec(_Function&& f) -> std::future<decltype(f())>
  {
  typedef decltype(f()) _Result;

  std::shared_ptr<std::packaged_task<_Result()>> task =
     std::make_shared<std::packaged_task<_Result()>>(f);

  std::function<void()> task_wrapper = [task]{(*task) (); };
  enqueue(task_wrapper);

  return task->get_future();
  }
};

 // Implementation for async_exec_service: 
 class async_exec_service1a { 
   std::queue<std::function<void ()>> _exec_queue; // worker threads will take functions from _exec_queue 
   // ... 
   void enqueue(std::function<void ()> task) {/* put task to _exec_queue */} 
 public: 
   template <typename _Function> 
   auto exec(_Function&& f) -> std::future<decltype(f())> { 
     typedef decltype(f()) _Result; 
     std::shared_ptr<std::packaged_task<_Result()>> task = std::make_shared<std::packaged_task<_Result()>>(f); 
     std::function<void()> task_wrapper = [task]{ (*task)(); }; 
     enqueue(task_wrapper); 
     return task->get_future(); 
   }; 
   // ... 
 };




// Yet another implementation for async_exec_service: 
class async_exec_service2 { 
  std::queue<std::function<void ()>> _exec_queue; // worker threads will take functions from _exec_queue 
  // ... 
  void enqueue(std::function<void ()> task) {/* put task to _exec_queue */} 
public: 
  template <typename _Function> 
  auto exec(_Function&& f) -> std::future<decltype(f())> { 
    typedef decltype(f()) _Result; 
    std::shared_ptr<std::promise<_Result>> result = std::make_shared<std::promise<_Result>>(); 
    std::function<void()> task_wrapper = [result, f]{ 
      try { 
        result->set_value(f()); 
      } catch (...) { 
        result->std::set_exception(std::current_exception()); 
      } 
    }; 
    enqueue(task_wrapper); 
    return result->get_future(); 
  }
};
// Asynchronous Provider Object: promise<T>
class async_exec_service2a {  // slide 20 of 26
  std::queue<std::function<void ()>> _exec_queue;// worker threads will take functions from _exec_queue
  // ...
  void enqueue( std::function<void ()> task) {/* put task to  _exec_queue; */ }
public:
  template <typename _Function>
  auto exec(_Function&& f) -> std::future<decltype(f())>
  {
    typedef decltype(f()) _Result;

    std::shared_ptr<std::promise<_Result()>> result =
       std::make_shared<std::promise<_Result()>>();

    std::function<void()> task_wrapper = [result,f]{
      try {
        result->set_value(f());
      } catch (...) {
        result->set_exception(std::current_exception());
      }

      return result->get_future();
    };
  }
};
 
int main(int argc, char**argv)
{
}

