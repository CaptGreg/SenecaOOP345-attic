// https://stackoverflow.com/questions/66239578/create-a-generator-in-c-using-coroutines

//
//  main.cpp
//  PlayWithCoroutines
//
//  Created by Georgios Tsoumplekas on 17/2/21. (2021-02-17)
//

#include <iostream>
#include <coroutine>

using namespace std;
struct resumable_thing
{
    struct promise_type
    {
            int const* _current;
            ~promise_type()
            {
              std::cout << "~promise_type()\n";
            }
            promise_type & get_return_object() noexcept
            {
              std::cout << "promise_type()\n";
                return *this;
            }
            auto initial_suspend() { 
              std::cout << "promise_type::initial_suspend()\n";
              return suspend_always{}; 
            }
            auto final_suspend() { 
              std::cout << "promise_type::final_suspend()\n";
              return suspend_always{}; 
            }
            auto yield_value(int const& value) {
              std::cout << "promise_type::yield_value()\n";
                _current = &value;
                return suspend_always{};
            }
            void unhandled_exception()
            {
              std::cout << "promise_type::unhandled_exception()\n";
                auto ex = std::current_exception();
                std::rethrow_exception(ex);
                //// MSVC bug? should be possible to rethrow with "throw;"
                //// rethrow exception immediately
                // throw;
            }
            void return_void() noexcept
            {
              std::cout << "promise_type::return_void()\n";
            }
    };
   // using coroutine_handle = coroutine_handle<promise_type>;
    resumable_thing() = default;
    resumable_thing(resumable_thing const &  ) = delete;
    resumable_thing & operator = (resumable_thing const &  ) =delete;
    resumable_thing(promise_type && promise)
            : _coroutine(coroutine_handle<promise_type>::from_promise(promise)) 
    {
        std::cout << "resumable_thing::resumable_thing(promise_type && promise)\n";
    }
    resumable_thing(resumable_thing&& other) : _coroutine(other._coroutine)
    {
        std::cout << "resumable_thing::resumable_thing(resumable_thing&& other)\n";
            other._coroutine = nullptr;
    }
    resumable_thing & operator = (resumable_thing const &&  other)
    {
        std::cout << "resumable_thing:: resumable_thing & operator = (resumable_thing const &&  other)\n";
        _coroutine = other._coroutine;
        _coroutine = nullptr;
        return *this;
    }
    explicit resumable_thing ( coroutine_handle <promise_type> coroutine) 
      :_coroutine(coroutine)
    {
        std::cout << "resumable_thing::resumable_thing(coroutine_handle<promise_type> coroutine)\n";
    }
    ~resumable_thing()
    {
        std::cout << "resumable_thing dtor\n";
        if(_coroutine)
        {
        std::cout << "resumable_thing dtor calling _coroutine.destroy()\n";
        std::cout << "resumable_thing dtor _coroutine.destroy() generates memory error - SIGSEGV\n";
        flush(std::cout);
            _coroutine.destroy();
        }
    }
    void resume()
    {
        std::cout << "resumable_thing::resume\n";
        std::cout << "_coroutine_valid=" << _coroutine_valid() << "\n";
        _coroutine.resume();
        
    }
    coroutine_handle<promise_type> _coroutine = nullptr;
    bool  _coroutine_valid () { return _coroutine != nullptr; }
};

resumable_thing counter()
{
    std::cout << "counter: called\n";
    for(int i = 0 ; ++i;)
    {
        co_await suspend_always{};
        std::cout << "counter: resumed (#" << i << ")\n";
    }
}

#include "funs.h"

int app(int argc, const char * argv[]) {
    // insert code here...
    register_sigaction();

    std::cout << "main:    calling counter\n";
    resumable_thing the_counter = counter();
    std::cout << "main:    resuming counter\n";
    the_counter.resume();
    //the_counter.resume();
    std::cout << "main:done" << std::endl;
    return EXIT_SUCCESS;
}

int main(int argc, const char * argv[]) {
  try {
    app(argc, argv);
    return EXIT_SUCCESS;
  } catch (std::exception& e) {
    std::cerr << "It threw " << e.what() << "\n";
  } catch (...) {
    std::cerr << "It threw ...\n";
  }
  return EXIT_FAILURE;
}

