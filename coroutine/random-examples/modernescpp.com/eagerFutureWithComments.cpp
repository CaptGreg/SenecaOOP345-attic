// https://www.modernescpp.com/index.php/lazy-futures-with-coroutines-in-c-20

// eagerFutureWithComments.cpp

#include <coroutine>
#include <iostream>
#include <memory>

template<typename T>
struct MyFuture {
    std::shared_ptr<T> value;
    MyFuture(std::shared_ptr<T> p): value(p) {                         // (3)
        std::cout << "    MyFuture::MyFuture" << '\n';
    }
    ~MyFuture() { 
        std::cout << "    MyFuture::~MyFuture" << '\n';
    }
    T get() {
        std::cout << "    MyFuture::get" << '\n';
        return *value;
    }

    struct promise_type {                                              // (4)
        std::shared_ptr<T> ptr = std::make_shared<T>();                // (11)
        promise_type() {
            std::cout << "        promise_type::promise_type" << '\n';
        }
        ~promise_type() { 
            std::cout << "        promise_type::~promise_type" << '\n';
        }
        MyFuture<T> get_return_object() {
            std::cout << "        promise_type::get_return_object" << '\n';
            return ptr;
        }
        void return_value(T v) {
            std::cout << "        promise_type::return_value" << '\n';
            *ptr = v;
        }
        std::suspend_never initial_suspend() {                         // (6)
            std::cout << "        promise_type::initial_suspend" << '\n';
            return {};
        }
        std::suspend_never final_suspend() noexcept {                 // (7)
            std::cout << "        promise_type::final_suspend" << '\n';
            return {};
        }
        void return_void() {}
        void unhandled_exception() {
            std::exit(1);
        }
    };                                                                 // (5)
};

MyFuture<int> createFuture() {                                         // (2)
    std::cout << "createFuture" << '\n';
    co_return 2021;
}

int main() {

    std::cout << '\n';

    auto fut = createFuture();                                         // (1)
    auto res = fut.get();                                              // (8)
    std::cout << "res: " << res << '\n';

    std::cout << '\n';

}                                                                     // (12)
