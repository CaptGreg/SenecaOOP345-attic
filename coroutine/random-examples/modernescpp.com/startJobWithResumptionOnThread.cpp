// https://www.modernescpp.com/index.php/automatically-resuming-a-job-with-coroutines

// startJobWithResumptionOnThread.cpp

#include <coroutine>
#include <functional>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

std::random_device seed;
auto gen = std::bind_front(std::uniform_int_distribution<>(0,1), 
                           std::default_random_engine(seed()));
 
struct MyAwaitable {
    std::jthread& outerThread;
    bool await_ready() const noexcept {                    
        auto res = gen();
        if (res) std::cout << " (executed)" << '\n';
        else std::cout << " (suspended)" << '\n';
        return res;                                        // (6)   
    }
    void await_suspend(std::coroutine_handle<> h) {        // (7)
        outerThread = std::jthread([h] { h.resume(); });   // (8)
    }
    void await_resume() {}
};

 
struct Job{
    static inline int JobCounter{1};
    Job() {
        ++JobCounter;
    }
    
    struct promise_type {
        int JobNumber{JobCounter};
        Job get_return_object() { return {}; }
        std::suspend_never initial_suspend() {         // (2)
            std::cout << "    Job " << JobNumber << " prepared on thread " 
                      << std::this_thread::get_id();
            return {}; 
        }
        std::suspend_never final_suspend() noexcept {  // (3)
            std::cout << "    Job " << JobNumber << " finished on thread " 
                      << std::this_thread::get_id() << '\n';
            return {}; 
        }
        void return_void() {}
        void unhandled_exception() { }
    };
};
 
Job performJob(std::jthread& out) {
    co_await MyAwaitable{out};                        // (1)
}
 
int main() {

    std::vector<std::jthread> threads(8);             // (4)
    for (auto& thr: threads) performJob(thr);         // (5)

}
