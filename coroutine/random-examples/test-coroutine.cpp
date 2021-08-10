// https://gist.githubusercontent.com/owt5008137/aa7b093caddcea5a79f32d0ebf4efa88/raw/b06d4fd5e32f12559d0d7c6e4c66ce646f46dd76/test-coroutine.cpp

// https://gist.github.com/owt5008137/aa7b093caddcea5a79f32d0ebf4efa88#file-test-coroutine-cpp

// #define __cpp_coroutines 1

#include <iostream>
#include <iomanip>
#include <vector>

#include <memory>

#include <coroutine>

struct wait_some_times {
    int left_times;
    int* refer_address;
    std::coroutine_handle<> handle;
    wait_some_times(int t) : left_times(t), handle(nullptr) {}
};

struct suspend_some_times {
    wait_some_times& d;
    suspend_some_times(wait_some_times& _d) : d(_d) {}
    bool await_ready() noexcept {
        std::cout << "call await_ready: " << d.left_times << std::endl;
        return d.left_times <= 0;
    }

    void await_suspend(std::coroutine_handle <> h) noexcept {
        // 记下来handle以便后面resume用
        d.handle = h;

        std::cout << "call await_suspend: " << (--d.left_times) << std::endl;
    }

    void await_resume() noexcept {
        std::cout << "call await_resume: " << d.left_times << std::endl;
        d.handle = nullptr;
    }
};

struct coroutine_task {
    struct promise_type {
        coroutine_task get_return_object() {
            return coroutine_task{};
        }
        auto initial_suspend() {
            return std::suspend_never{};
        }
        auto final_suspend() {
            return std::suspend_never{};
        }

        void unhandled_exception() {}
        void return_void() {}
    };
};

auto operator co_await(wait_some_times& x) noexcept {
    return suspend_some_times{ x };
}

coroutine_task f(wait_some_times& waiter) {
    // 分配大一点，否则有些优化选项会把它扔进寄存器
    int stack_variable[128] = {0};
    int* sp, *bp;
    asm ("movq %%rsp, %0" : "=r" (sp) );
    asm ("movq %%rbp, %0" : "=r" (bp) );

    std::cout << "start of coroutine, address of stack_variable="<< &stack_variable[0] << ", %rsp="<< sp << ", %rbp="<< bp << std::endl;
    co_await waiter; // 只有前三次会协程切出
    stack_variable[0] = waiter.left_times;
    waiter.refer_address = &stack_variable[0];
    co_await waiter;
    stack_variable[10] = waiter.left_times;
    waiter.refer_address = &stack_variable[10];
    co_await waiter;
    stack_variable[20] = waiter.left_times;
    waiter.refer_address = &stack_variable[20];
    co_await waiter; // 这之后await_ready返回true了，不会再切出
    stack_variable[30] = waiter.left_times;
    waiter.refer_address = &stack_variable[30];
    co_await waiter;
    stack_variable[40] = waiter.left_times;
    waiter.refer_address = &stack_variable[40];
    std::cout << "end of coroutine" << std::endl;
}

int main(int argc, char* argv[]) 
{
#ifdef __cpp_coroutines
    std::cout << "__cpp_coroutines: " << __cpp_coroutines << std::endl;
#endif

    void* sp, *bp;
    asm ("movq %%rsp, %0" : "=r" (sp) );
    asm ("movq %%rbp, %0" : "=r" (bp) );

    wait_some_times waiter{ 3 };
    std::cout << "main(), address of stack variable(waiter)="<< &waiter << ", %rsp="<< sp << ", %rbp="<< bp<< std::endl;
    f(waiter);

    while (waiter.handle && !waiter.handle.done()) {
        std::cout << "about to resume: " << &waiter<< ", last refer address: "<< waiter.refer_address << std::endl;
        // 这里用传出的handle来恢复切入协程
        waiter.handle.resume();
    }

    return 0;
}
