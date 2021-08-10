// https://modernescpp.com/index.php/implementing-futures-with-coroutines

// eagerFuture.cpp

#include <coroutine>
#include <iostream>
#include <memory>

template<typename T>
struct MyFuture {
    std::shared_ptr<T> value;                           // (3)
    MyFuture(std::shared_ptr<T> p): value(p) {}
    ~MyFuture() { }
    T get() {                                          // (10)
        // std::cout << "MyFuture::get(): use_count=" << value.use_count() 
                  // << ", returning " << *value << "\n";
        return *value;
    }

    struct promise_type {
        std::shared_ptr<T> ptr = std::make_shared<T>(); // (4)
        ~promise_type() { }
        MyFuture<T> get_return_object() {              // (7)
            return ptr;
        }
        void return_value(T v) {
            *ptr = v;
        }
        std::suspend_never initial_suspend() {          // (5)
            return {};
        }
        std::suspend_never final_suspend() noexcept {  // (6)
            return {};
        }
        void unhandled_exception() {
            std::exit(1);
        }
    };
};

MyFuture<int> createFuture() {                         // (1)
    co_return 2021;                                    // (9)
}

int main() {

    std::cout << '\n';

    auto fut = createFuture();
    std::cout << "fut.get(): " << fut.get() << '\n';   // (2)

    std::cout << '\n';

}
