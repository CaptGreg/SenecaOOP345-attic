#include <coroutine>
#include <iostream>
#include <vector>

// try ../templated_generator.h which works with ../generator_diy.cpp

template <typename ENCAPSULATED_TYPE>
struct Generator
{
    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;
    struct promise_type
    {
        ENCAPSULATED_TYPE current_value;

        auto get_return_object() { return Generator{coro_handle::from_promise(*this)}; }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() { return std::suspend_always{}; }
        void unhandled_exception() { std::terminate(); }

        auto yield_value(int value)
        {
            current_value = value;
            return std::suspend_always{};
        }
    };
    bool move_next() { return coro ? (coro.resume(), !coro.done()) : false; }
    ENCAPSULATED_TYPE current_value() { return coro.promise().current_value; }
    Generator(Generator const &) = delete;
    Generator(Generator &&rhs) : coro(rhs.coro) { rhs.coro = nullptr; }
    ~Generator()
    {
        if (coro)
            coro.destroy();
    }

private:
    Generator(coro_handle h) : coro(h) {}
    coro_handle coro;
};

// Coroutine gets called on need

// generator<int> 
Generator<int> 
generateNumbers(int begin, int inc = 1) 
{
  
  for (int i = begin;; i += inc) {
    co_yield i;
  }
  
}

int main() {

    std::cout << std::endl;

    const auto numbers= generateNumbers(-10);     
  
    for (int i= 1; i <= 20; ++i) 
        std::cout << numbers << " "; // Runs finite = 20 times
        // error: no match for ‘operator<<’ (operand types are ‘std::ostream’ {aka ‘std::basic_ostream<char>’} and ‘const Generator<int>’)
        // GB no clue how to cout << numbers
  
                                                         
    for (auto n: generateNumbers(0, 5)) // Runs infinite times
         std::cout << n << " "; // (3)

    std::cout << "\n\n";

}
