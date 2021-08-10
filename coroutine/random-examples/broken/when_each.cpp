// https://gist.github.com/kennykerr/d9ce7f8235a3777cd5e75c55b96089f2

#include <chrono>
#include <coroutine>
#include <iostream>
#include <vector>
// #include "winrt/Windows.Foundation.h"

// using namespace winrt;
// using namespace Windows::Foundation;
using namespace std::chrono;

// https://en.cppreference.com/w/cpp/chrono/duration
  constexpr auto year = 31556952ll; // seconds in average Gregorian year
   

struct CallToFinsihedCoroutine : std::logic_error {
	CallToFinsihedCoroutine() : std::logic_error("Call to a coroutine that's finished") {}
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
			throw CallToFinsihedCoroutine();
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
			throw CallToFinsihedCoroutine();
		_handle.resume();
	}
	explicit operator bool() {
		return !_handle.done();	
	}
	~Resumable() {
		_handle.destroy();
	}
};

template <typename T>  // templates global scope
auto when_each(T first, T last)
{
    for (; first != last; ++first)
    {
        co_await *first;
    }
};



int main()
{

#if 1
    auto wait_for =  [] (std::chrono::seconds duration) -> Resumable<void>
    {
        co_await duration;
        // error: no member named ‘await_ready’ in ‘std::chrono::seconds’ {aka ‘std::chrono::duration<long int>’}
        std::cout << "done " << duration.count() << "\n";
    };


    std::vector v{ wait_for(1s), wait_for(2s), wait_for(3s) };

    // Note: you need to keep the vector alive for the duration of the wait...
    when_each(v.begin(), v.end()).get();

#endif
    std::cout << "done\n";

    {
        using shakes = std::chrono::duration<int, std::ratio<1, 100000000>>;
        using jiffies = std::chrono::duration<int, std::centi>;
        using microfortnights = std::chrono::duration<float, std::ratio<14*24*60*60, 1000000>>;
        using nanocenturies = std::chrono::duration<float, std::ratio<100*year, 1000000000>>;
     
        std::chrono::seconds sec(1);
     
        std::cout << "1 second is:\n";
     
        // integer scale conversion with no precision loss: no cast
        std::cout << std::chrono::microseconds(sec).count() << " microseconds\n"
                  << shakes(sec).count() << " shakes\n"
                  << jiffies(sec).count() << " jiffies\n";
     
        // integer scale conversion with precision loss: requires a cast
        std::cout << std::chrono::duration_cast<std::chrono::minutes>(sec).count()
                  << " minutes\n";
     
        // floating-point scale conversion: no cast
        std::cout << microfortnights(sec).count() << " microfortnights\n"
                  << nanocenturies(sec).count() << " nanocenturies\n";
    }
}
