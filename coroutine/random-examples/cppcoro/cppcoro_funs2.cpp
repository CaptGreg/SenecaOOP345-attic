// g++-10 -fcoroutines -std=c++2a   co_funs2.cpp -o co_funs2 -lcppcoro -pthread && ./co_funs2

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// https://mariusbancila.ro/blog/2020/06/22/a-cpp20-coroutine-example/

#include <chrono>
#include <iostream>
#include <cppcoro/generator.hpp>
#include <cppcoro/task.hpp>
#include <cppcoro/sync_wait.hpp>

void print_time()
{
   auto now = std::chrono::system_clock::now();
   std::time_t time = std::chrono::system_clock::to_time_t(now);   
 
   char mbstr[10+1]; // [11:44:44] + \0
   if (std::strftime(mbstr, sizeof(mbstr), "[%H:%M:%S] ", std::localtime(&time))) {
      std::cout << mbstr;
   }
}

cppcoro::generator<std::string> produce_items()
{
  while (true) {
     auto v = rand();
     using namespace std::string_literals;
     auto i = "item "s + std::to_string(v);
     print_time();
     std::cout << "produced " << i << '\n';
     co_yield i;
  }
}

cppcoro::task<> consume_items(int const n)
{
  int i = 1;
  for(auto const& s : produce_items()) {
     print_time();
     std::cout << "consumed " << s << '\n';
     if (++i > n) break;
  }
 
  co_return;
}

int main()
{
   try {
     cppcoro::sync_wait(consume_items(5));
   } catch (...) {
     std::cerr << "it threw ..\n";
   }
}
