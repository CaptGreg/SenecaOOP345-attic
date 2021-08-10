// https://www.modernescpp.com/index.php/lazy-futures-with-coroutines-in-c-20

// lazyFuture.cpp

#include <coroutine>
#include <iostream>
#include <memory>

template<typename T>
struct MyFuture {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
 
    handle_type coro;                                               // (5)

    MyFuture(handle_type h): coro(h) {
        std::cout << "    MyFuture::MyFuture" << '\n';
    }
    ~MyFuture() { 
        std::cout << "    MyFuture::~MyFuture" << '\n';
        if ( coro ) coro.destroy();                                 // (8)
    }

    T get() {
        std::cout << "    MyFuture::get" << '\n';
        coro.resume();                                              // (6)
        return coro.promise().result;
    }

    struct promise_type {
		T result;
        promise_type() {
           std::cout << "        promise_type::promise_type" << '\n';
        }
        ~promise_type() { 
            std::cout << "        promise_type::~promise_type" << '\n';
        }
        auto get_return_object() {                                  // (3)
            std::cout << "        promise_type::get_return_object" << '\n';
            return MyFuture{handle_type::from_promise(*this)};
        }
        void return_value(T v) {
            std::cout << "        promise_type::return_value" << '\n';
            result = v;
        }
        std::suspend_always initial_suspend() {                    // (1)
            std::cout << "        promise_type::initial_suspend" << '\n';
            return {};
        }
        std::suspend_always final_suspend() noexcept {            // (2)
            std::cout << "        promise_type::final_suspend" << '\n';
            return {};
        }
        void return_void() {}
        void unhandled_exception() {
            std::exit(1);
        }
    };
};

MyFuture<int> createFuture() {
    std::cout << "createFuture" << '\n';
    co_return 2021;
}

int main() {

    std::cout << '\n';

    auto fut = createFuture();                                        // (4)
    auto res = fut.get();                                             // (7)
    std::cout << "res: " << res << '\n';

    std::cout << '\n';

}
