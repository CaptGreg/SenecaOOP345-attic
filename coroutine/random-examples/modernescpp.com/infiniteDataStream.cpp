// https://www.modernescpp.com/index.php/c-20-an-infinite-data-stream-with-coroutines

// infiniteDataStream.cpp
// g++-10 -fcoroutines -std=c++2a   infiniteDataStream.cpp -o infiniteDataStream -lcppcoro -pthread && ./infiniteDataStream


#if 0
The framework for writing coroutines consists of more than 20 functions that you partially have to implement and partially could overwrite. Therefore, you can tailor the coroutine to your needs. On the end, you can, for example, create a generator Generator<int> for an infinite data stream such as the following one: 

Generator<int> getNext(int start = 0, int step = 1) {
    auto value = start;
    for (int i = 0;; ++i) {
        co_yield value;
        value += step;
    }
}

 

Now, we know the destiny of our job. Lets start.
The Framework

A coroutine consists of three parts: the promise object, the coroutine handle, and the coroutine frame.

    Promise object: The promise object is manipulated from inside the coroutine, and it delivers its result via the promise object.
    Coroutine handle: The coroutine handle is a non-owning handle to resume or to destroy the coroutine frame from outside.
    Coroutine frame: The coroutine frame is an internal, typically heap-allocated state. It consists of the already mentioned promise object, the copied parameters of the coroutine, the representation of the suspension points, local variables which lifetime ends before the current suspension point, and local variables which lifetime exceeds the current suspension point.


The Simplified Workflow

When you use co_yield, co_await, or co_return in a function, the function becomes a coroutine, and the compiler transforms its body to something equivalent to the following lines. 

    {
      Promise promise;
      co_await promise.initial_suspend();
      try
      {
        <function body>
      }
      catch (...)
      {
        promise.unhandled_exception();
      }
      FinalSuspend:
        co_await promise.final_suspend();
    }

<function body> stands for  the original function body. The simplified workflow of the coroutine consists of the following phases.

The coroutine begins execution

    Allocates the coroutine frame
    Copies all functions parameters to the coroutine frame
    Creates the promise object promise
    Calls promise.get_return_object() to create the coroutine handle and keeps it in a local variable. The result of the call will be returned to the caller when the coroutine first suspends. 
    Calls promise.initial_suspend() and co_awaits its result. The promise type typically returns std::suspend_never for eagerly-started coroutines or std::suspend_always for lazily-started coroutines. 
    The body of the coroutine is executed, when co_await promise.initial_suspend() resumes

The coroutine reaches a suspension point

    The coroutine handle (promise.get_return_object()) is returned to the caller, which resumed the coroutine

The coroutine reaches co_return

    Calls promise.return_void() for co_return or co_return expression, where expression has type void
    Calls promise.return_value(expression) for co_return expression, where expression has non-type void
    Destroys all stack created variables
    Calls promise.final_suspend() and co_awaits its result

The coroutine is destroyed (by terminating via co_return, an uncaught exception, or via the coroutine handle)

    Calls the destruction of the promise object
    Calls the destructor of the function parameters
    Frees the memory used by the coroutine frame
    Transfers control back to the caller

#endif
//  Let's put the theory into praxis.
// An Infinite Data Stream with co_yield

// The following program produces an infinite data stream. The coroutine getNext uses co_yield to create a data stream that starts at start and gives on request the next value, incremented by step.

#include <coroutine>
#include <memory>
#include <iostream>

template<typename T>
struct Generator {
    
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    
    Generator(handle_type h): coro(h) {}                         // (3)
    handle_type coro;
    
    ~Generator() {
        if ( coro ) coro.destroy();
    }
    Generator(const Generator&) = delete;
    Generator& operator = (const Generator&) = delete;
    Generator(Generator&& oth) noexcept : coro(oth.coro) {
        oth.coro = nullptr;
    }
    Generator& operator = (Generator&& oth) noexcept {
        coro = oth.coro;
        oth.coro = nullptr;
        return *this;
    }
    T getValue() {
        return coro.promise().current_value;
    }
    bool next() {                                                // (5)
        coro.resume();
        return not coro.done();
    }
    struct promise_type {
        promise_type()  = default;                               // (1)
          
        ~promise_type() = default;
        
        auto initial_suspend() {                                 // (4)
            return std::suspend_always{};
        }
        auto final_suspend() {
            return std::suspend_always{};
        }
        auto get_return_object() {                               // (2)
            return Generator{handle_type::from_promise(*this)};
        }
        auto return_void() {
            return std::suspend_never{};
        }
      
        auto yield_value(const T value) {                        // (6) 
            current_value = value;
            return std::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
        T current_value;
    };

};

Generator<int> getNext(int start = 0, int step = 1) noexcept {
    auto value = start;
    for (int i = 0;; ++i){
        co_yield value;
        value += step;
    }
}

int main() {
    
    std::cout << std::endl;
  
    std::cout << "getNext():";
    auto gen = getNext();
    for (int i = 0; i <= 10; ++i) {
        gen.next();
        std::cout << " " << gen.getValue();                      // (7)
    }
    
    std::cout << "\n\n";
    
    std::cout << "getNext(100, -10):";
    auto gen2 = getNext(100, -10);
    for (int i = 0; i <= 20; ++i) {
        gen2.next();
        std::cout << " " << gen2.getValue();
    }
    
    std::cout << std::endl;
    
}
/*
The numbers in the program infiniteDataStream.cpp stand for the steps in the first iteration of the workflow.

    1. Creates the promise
    2. Calls promise.get_return_object() and keeps the result in a local variable
    3. Creates the generator
    4.  Calls promise.initial_suspend(). The generator is lazy and, therefore, suspends always.
    5. Asks for the next value and returns if the generator is consumed
    6. Triggered by the co_yield call. The next value is afterward available.
    7. Gets the next value

In additional iterations, only the steps 5 to 7 are performed. 

It is quite challenging to understand the underlying framework of coroutines. 
Playing with existing coroutines and observing the changed behavior may be the easiest way to grasp them. 
The presented coroutine that creates an infinite data stream is a good starting point for your first experiments .


*/
