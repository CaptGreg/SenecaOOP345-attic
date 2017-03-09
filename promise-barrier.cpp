// http://en.cppreference.com/w/cpp/thread/promise

// std::promise
// Thread support library
// std::promise
// Defined in header <future>
// 		
// template< class R > class promise;
// 	(1) 	(since C++11)
// template< class R > class promise<R&>;
// 	(2) 	(since C++11)
// template<>          class promise<void>;
// 	(3) 	(since C++11)
// 		
// 1) base template
// 2) non-void specialization, used to communicate objects between threads
// 3) void specialization, used to communicate stateless events
// 
// The class template std::promise provides a facility to store a value or an exception that is later acquired asynchronously via a std::future object created by the std::promise object.
// 
// Each promise is associated with a shared state, which contains some state information and a result which may be not yet evaluated, evaluated to a value (possibly void) or evaluated to an exception. A promise may do three things with the shared state:
// 
//     make ready: the promise stores the result or the exception in the shared state. Marks the state ready and unblocks any thread waiting on a future associated with the shared state.
//     release: the promise gives up its reference to the shared state. If this was the last such reference, the shared state is destroyed. Unless this was a shared state created by std::async which is not yet ready, this operation does not block.
//     abandon: the promise stores the exception of type std::future_error with error code std::future_errc::broken_promise, makes the shared state ready, and then releases it. 
// 
// The promise is the "push" end of the promise-future communication channel: the operation that stores a value in the shared state synchronizes-with (as defined in std::memory_order) the successful return from any function that is waiting on the shared state (such as std::future::get). Concurrent access to the same shared state may conflict otherwise: for example multiple callers of std::shared_future::get must either all be read-only or provide external synchronization.
// 

// MEMBER FUNCTIONS

// (constructor) constructs the promise object
// (public member function) (destructor) 
//    destructs the promise object
// (public member function) operator= 
//    assigns the shared state
// (public member function) swap s
//    waps two promise objects
// (public member function) Getting the result 
//    get_future returns a future associated with the promised result
// (public member function) Setting the result s
//    et_value sets the result to specific value
// (public member function) set_value_at_thread_exit 
//    sets the result to specific value while delivering the notification only at thread exit
// (public member function) set_exception 
//    sets the result to indicate an exception
// (public member function) set_exception_at_thread_exit 
//    sets the result to indicate an exception while delivering the notification only at thread exit
// (public member function) Non-member functions std::swap(std::promise)
//    (C++11) specializes the std::swap algorithm (function template)
// Helper classes std::uses_allocator<std::promise>
//    (C++11) specializes the std::uses_allocator type trait (class template specialization)


#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>
 
void accumulate(std::vector<int>::iterator first,
                std::vector<int>::iterator last,
                std::promise<int> accumulate_promise)
{
    int sum = std::accumulate(first, last, 0);
    accumulate_promise.set_value(sum);  // Notify future
}
 
void do_work(std::promise<void> barrier)
{
    std::cout << "do_work: sleeping zzz, zzz, zzz, zzz, ...\n";              // GB
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "do_work: setting barrier\n";              // GB
    barrier.set_value();
}
 
int main()
{
    // Demonstrate using promise<int> to transmit a result between threads.
    std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
                            std::move(accumulate_promise));
    accumulate_future.wait();  // wait for result
    std::cout << "result=" << accumulate_future.get() << '\n';
    work_thread.join();  // wait for thread completion
 
    // Demonstrate using promise<void> to signal state between threads.
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::move(barrier));
    std::cout << "main: waitng for barrier\n";              // GB
    barrier_future.wait();
    std::cout << "main: passed barrier\n";              // GB
    new_work_thread.join();
}
