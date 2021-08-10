// https://wandbox.org/permlink/9EygkdNmVKgJhmgm

// prog.cpp

#include <iostream>
#include <array>
#include <coroutine>

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
