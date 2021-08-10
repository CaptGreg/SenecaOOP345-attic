// https://en.cppreference.com/w/cpp/language/coroutines

#include <coroutine>
#include <iostream>
#include <thread>    // jthread
 
auto switch_to_new_thread(std::jthread& out) {
  struct awaitable {
    std::jthread* p_out;
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
      std::jthread& out = *p_out;
      if (out.joinable())
        throw std::runtime_error("switch_to_new_thread::awaitable Output jthread parameter not empty");
      out = std::jthread([h] { h.resume(); });
      std::cout << "switch_to_new_thread::awaitable New thread ID: " << out.get_id() << "\n";
    }
    void await_resume() {}
  };
  return awaitable{&out};
}

struct task {
  struct promise_type {
    task               get_return_object   ()          { return {}; }
    std::suspend_never initial_suspend     ()          { return {}; }
    std::suspend_never final_suspend       () noexcept { return {}; }
    void               return_void         ()          { }
    void               unhandled_exception ()          { }
  };
};
 
task resuming_on_new_thread(std::jthread& out) 
{ 

  std::cout << "resuming_on_new_thread: Coroutine started on thread: " << std::this_thread::get_id() << "\n";

  co_await switch_to_new_thread(out);

  // awaiter destroyed here
  std::cout << "resuming_on_new_thread: Coroutine resumed on thread: " << std::this_thread::get_id() << "\n";
}
 
auto mainTid = std::this_thread::get_id();
int  main() {
  std::cout << "main: thread ID: " << mainTid << "\n";
  std::jthread out;
  resuming_on_new_thread(out);
  return EXIT_SUCCESS;
}
/*
  main: thread ID: 139986610030400
  resuming_on_new_thread: Coroutine started on thread: 139986610030400
  switch_to_new_thread::awaitable New thread ID: 139986610026240
  resuming_on_new_thread: Coroutine resumed on thread: 139986610026240
*/
