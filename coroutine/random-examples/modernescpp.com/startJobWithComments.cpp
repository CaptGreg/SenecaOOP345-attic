// https://www.modernescpp.com/index.php/starting-jobs-with-coroutines

// startJobWithComments.cpp

#include <coroutine>
#include <iostream>

struct MySuspendAlways {                                  // (1)
    bool await_ready() const noexcept { 
        std::cout << "        MySuspendAlways::await_ready"  << '\n';
        return false; 
    }
    void await_suspend(std::coroutine_handle<>) const noexcept {
        std::cout << "        MySuspendAlways::await_suspend"  << '\n';

    }
    void await_resume() const noexcept {
        std::cout << "        MySuspendAlways::await_resume"  << '\n';
    }
};

struct MySuspendNever {                                  // (2)
    bool await_ready() const noexcept { 
        std::cout << "        MySuspendNever::await_ready"  << '\n';
        return true; 
    }
    void await_suspend(std::coroutine_handle<>) const noexcept {
        std::cout << "        MySuspendNever::await_suspend"  << '\n';

    }
    void await_resume() const noexcept {
        std::cout << "        MySuspendNever::await_resume"  << '\n';
    }
};
 
struct Job { 
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;
    Job(handle_type h): coro(h){}
    ~Job() {
        if ( coro ) coro.destroy();
    }
    void start() {
        coro.resume();
    }


    struct promise_type {
        auto get_return_object() { 
            return Job{handle_type::from_promise(*this)};
        }
        MySuspendAlways initial_suspend() {         // (3)
            std::cout << "    Job prepared" << '\n';
            return {}; 
        }
        MySuspendAlways final_suspend() noexcept {  // (4)
            std::cout << "    Job finished" << '\n'; 
            return {}; 
        }
        void return_void() {}
        void unhandled_exception() {}
    
    };
};
 
Job prepareJob() {
    co_await MySuspendNever();                     // (5)
}
 
int main() {

    std::cout <<  "Before job" << '\n';

    auto job = prepareJob();                      // (6)
    job.start();                                  // (7)

    std::cout <<  "After job" <<  '\n';

}
