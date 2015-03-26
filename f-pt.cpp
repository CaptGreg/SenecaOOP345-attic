// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2709.html

#include <thread>
#include <future>
#include <cassert>
#include <memory> // unique_ptr

int calculate_the_answer_to_life_the_universe_and_everything()
{
    return 42;
}

template<typename F>
std::future<typename std::result_of<F()>::type> spawn_task1(F f)
{
    typedef typename std::result_of<F()>::type result_type;
    std::packaged_task<result_type()> task(std::move(f));
    std::future<result_type> res(task.get_future());
    std::thread(std::move(task));
    return res;
}

template<typename F>
std::future<typename std::result_of<F()>::type> spawn_task2(F f)
{
    typedef typename std::result_of<F()>::type result_type;

    struct local_task
    {
        std::promise<result_type> promise;
        F func;

        local_task(local_task const& other)=delete;

        local_task(F func_): func(func_) {}

        local_task(local_task&& other): promise(std::move(other.promise)), func(std::move(other.func)) {}

        void op()  // GB was operator
        {
            try {
                promise.set_value(f());
            } catch(...) {
                promise.set_exception(std::current_exception());
            }
        }
    };
    
    local_task task(std::move(f));

    std::future<result_type> res(task.promise.get_future());
    std::thread(std::move(task));
    return res;
}

int main(int argc, char**argv)
{
  std::packaged_task<int()> pt(calculate_the_answer_to_life_the_universe_and_everything);
  auto fi=pt.get_future();

  std::thread task(std::move(pt)); // launch task on a thread

  fi.wait(); // wait for it to finish
    
  // assert(fi.is_ready());
  // assert(fi.has_value());
  // assert(!fi.has_exception());
  // assert(fi.get_state()==std::future_state::ready);

  assert(fi.get()==42);

  task.join();
}
