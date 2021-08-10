// https://wandbox.org/permlink/9EygkdNmVKgJhmgm

// sm-co.cpp: state machines with coroutines

// C++20 coroutines obsolete state-machines

// g++-10 -fcoroutines -std=c++2a   sm-co.cpp -o sm-co -pthread && ./sm-co

/*
  co_return;
  co_return "!";

    return statement in a coroutine

  co_await std::suspend_always();

  co_await
    
    The unary operator co_await suspends a coroutine and returns control to the caller. 
    Its operand is an expression whose type must either define operator co_await, or be convertible to such type by means of the current coroutine's Promise::await_transform
    co_await expr 		
    
    First, expr is converted to an awaitable as follows:
    
        if expr is produced by an initial suspend point, a final suspend point, or a yield expression, the awaitable is expr, as-is.
        otherwise, if the current coroutine's Promise type has the member function await_transform, then the awaitable is promise.await_transform(expr)
        otherwise, the awaitable is expr, as-is. 
    
    Then, the awaiter object is obtained, as follows:
    
        if overload resolution for operator co_await gives a single best overload, the awaiter is the result of that call (awaitable.operator co_await() 
        for member overload, operator co_await(static_cast<Awaitable&&>(awaitable)) for the non-member overload)
        otherwise, if overload resolution finds no operator co_await, the awaiter is awaitable, as-is
        otherwise, if overload resolution is ambiguous, the program is ill-formed 
    
    If the expression above is a prvalue, the awaiter object is a temporary materialized from it. Otherwise, if the expression above is an glvalue, the awaiter object is the object to which it refers.
    
    Then, awaiter.await_ready() is called (this is a short-cut to avoid the cost of suspension if it's known that the result is ready or can be completed synchronously). 
    If its result, contextually-converted to bool is false then
    
        The coroutine is suspended (its coroutine state is populated with local variables and current suspension point). 
        awaiter.await_suspend(handle) is called, where handle is the coroutine handle representing the current coroutine. Inside that function, 
        the suspended coroutine state is observable via that handle, and it's this function's responsibility to schedule it to resume on some executor, or to be destroyed (returning false counts as scheduling)
    
            if await_suspend returns void, control is immediately returned to the caller/resumer of the current coroutine (this coroutine remains suspended), otherwise
            if await_suspend returns bool, 
    
                the value true returns control to the caller/resumer of the current coroutine
                the value false resumes the current coroutine. 
    
            if await_suspend returns a coroutine handle for some other coroutine, that handle is resumed (by a call to handle.resume()) (note this may chain to eventually cause the current coroutine to resume)
            if await_suspend throws an exception, the exception is caught, the coroutine is resumed, and the exception is immediately re-thrown 
    
        Finally, awaiter.await_resume() is called, and its result is the result of the whole co_await expr expression. 
    
    If the coroutine was suspended in the co_await expression, and is later resumed, the resume point is immediately before the call to awaiter.await_resume().
    
    Note that because the coroutine is fully suspended before entering awaiter.await_suspend(), that function is free to transfer the coroutine handle across threads, 
    with no additional synchronization. For example, it can put it inside a callback, scheduled to run on a threadpool when async I/O operation completes. 
    In that case, since the current coroutine may have been resumed and thus executed the awaiter object's destructor, 
    all concurrently as await_suspend() continues its execution on the current thread, await_suspend() should treat *this as destroyed and not access it after the handle was published to other threads.
    
    
    
  co_yield "world";
  co_yield "Hello";
  
  co_yield
    
    Yield-expression returns a value to the caller and suspends the current coroutine: it is the common building block of resumable generator functions
    co_yield expr 		
    co_yield braced-init-list 		
    
    'co_yield expr' is equivalent to 'co_await promise.yield_value(expr)'
    
    A typical generator's yield_value would store (copy/move or just store the address of, since the argument's lifetime crosses the suspension point inside the co_await) its argument into the generator object and return std::suspend_always, transferring control to the caller/resumer.
	
*/


#include <iostream>
#include <array>
#include <coroutine>

struct CallToFinishedCoroutine : std::logic_error {
	CallToFinishedCoroutine() : std::logic_error("Call to a coroutine that's finished") {}
};

struct UnhandledExceptionInCoroutine : std::logic_error {
	UnhandledExceptionInCoroutine() : std::logic_error("Unhandled exception in coroutine") {}
};

template <typename T>
struct Resumable {
	struct promise_type {
		T _returned;
		auto get_return_object() {
			return handle_type::from_promise(*this);
		}
		auto initial_suspend() {
			return std::suspend_always();
		}
		auto final_suspend() {
			return std::suspend_always();
		}
		void unhandled_exception() {
			throw(UnhandledExceptionInCoroutine());
		}
		auto yield_value(const T& value){
			_returned = value;
			return std::suspend_always();
		}
		void return_value(const T& value) {
			_returned = value;
		}
	};
	using handle_type = std::coroutine_handle<promise_type>;
	handle_type _handle;
public:
	Resumable(handle_type handle) : _handle(handle) { }
	Resumable(Resumable&) = delete;
	T operator()() {
		if (_handle.done())
			throw CallToFinishedCoroutine();
		_handle.resume();
		return _handle.promise()._returned;
	}
	explicit operator bool() {
		return !_handle.done();	
	}
	~Resumable() {
		_handle.destroy();
	}
};

template <>
struct Resumable<void> {
	struct promise_type {
		auto get_return_object() {
			return handle_type::from_promise(*this);
		}
		auto initial_suspend() {
			return std::suspend_always();
		}
		auto final_suspend() {
			return std::suspend_always();
		}
		void unhandled_exception() {
			throw(UnhandledExceptionInCoroutine());
		}
		void return_void() {
		}
	};
	using handle_type = std::coroutine_handle<promise_type>;
	handle_type _handle;
public:
	Resumable(handle_type handle) : _handle(handle) { }
	Resumable(Resumable&) = delete;
	void operator()() {
		if (_handle.done())
			throw CallToFinishedCoroutine();
		_handle.resume();
	}
	explicit operator bool() {
		return !_handle.done();	
	}
	~Resumable() {
		_handle.destroy();
	}
};

void checkLocal() {
	int local = 2;
	std::cout << "Stack is around " << &local << std::endl; 
}

int main() {
	checkLocal();
	
	auto res = [] () -> Resumable<std::string> {
		co_yield "Hello";
		int local = 3;
		std::cout << "Waiting, with locals around " << &local << std::endl;
		co_yield "world";
		checkLocal();
		std::cout << "World" << std::endl;
		co_return "!";
	}();
	
	int iteration = 0;
	while (res) {
		std::cout << "Iteration " << iteration << std::endl;
		auto got = res();
		std::cout << "Got " << got << std::endl;
		iteration++;
	}
	
	std::cout << std::endl;
	auto res2 = [] () -> Resumable<void> {
		std::cout << "Hey" << std::endl;
		co_await std::suspend_always();
		std::cout << "Planet" << std::endl;
		co_return;
	}();
	
	iteration = 0;
	while (res2) {
		std::cout << "Iteration " << iteration << std::endl;
		res2();
		iteration++;
	}
}
/*

Stack is around 0x7ffcd423e784
Iteration 0
Got Hello
Iteration 1
Waiting, with locals around 0x55f231436f0c
Got world
Iteration 2
Stack is around 0x7ffcd423e6c4
World
Got !

Iteration 0
Hey
Iteration 1
Planet

*/
