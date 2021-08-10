#pragma once
// https://github.com/llint/Awaitable

#include <map>
#include <list>
#include <queue>
#include <chrono>
#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <memory>     // GB shared_ptr
#include <coroutine>  // GB remove experimental

#include <iostream>   // GB

#include <cassert>

using namespace std::chrono;
using namespace std;

namespace std
{
    template<>
    struct hash<coroutine_handle<>>
    {
        size_t operator()(const coroutine_handle<>& ch) const
        {
            return hash<void*>()(ch.address());
        }
    };
}

namespace pi
{
    // NB: reference is default constructable, and equality comparable!
    template <class T>
    class reference {
    public:
        typedef T type;

        reference() noexcept : _ptr(nullptr) {}
        reference(T& ref) noexcept : _ptr(std::addressof(ref)) {}
        reference(T&&) = delete;
        reference(const reference&) noexcept = default;

        reference& operator=(const reference& x) noexcept = default;

        operator T& () const noexcept { return *_ptr; }
        T& get() const noexcept { return *_ptr; }

        bool operator==(const reference& other) const noexcept { return _ptr == other._ptr; }

    private:
        T* _ptr;
    };

    class executor
    {
    public:
        static executor& singleton()
        {
            thread_local static executor s_singleton;
            return s_singleton;
        }

        void add_ready_coro(coroutine_handle<> coro)
        {
            _ready_coros.push(coro);
        }

        void add_timed_wait_coro(std::chrono::high_resolution_clock::time_point when, coroutine_handle<> coro)
        {
            auto r = _timed_wait_coros.emplace(when, std::pair<std::list<coroutine_handle<>>, std::unordered_map<coroutine_handle<>, std::list<coroutine_handle<>>::iterator>>{});
            assert(r.first->second.first.size() == r.first->second.second.size());
            r.first->second.first.emplace_back(coro); // NB: the same coroutine cannot be suspended multiple times!
            r.first->second.second.emplace(coro, --r.first->second.first.end());
            assert(r.first->second.first.size() == r.first->second.second.size());
        }

        void remove_timed_wait_coro(std::chrono::high_resolution_clock::time_point when, coroutine_handle<> coro)
        {
            auto it = _timed_wait_coros.find(when);
            if (it != _timed_wait_coros.end())
            {
                assert(it->second.first.size() == it->second.second.size());
                auto it2 = it->second.second.find(coro);
                if (it2 != it->second.second.end())
                {
                    it->second.first.erase(it2->second);
                    it->second.second.erase(it2);
                    assert(it->second.first.size() == it->second.second.size());
                }
                if (it->second.first.empty())
                {
                    _timed_wait_coros.erase(it);
                }
            }
        }

        void increment_num_outstanding_coros()
        {
            ++_num_outstanding_coros;
        }

        void decrement_num_outstanding_coros()
        {
            --_num_outstanding_coros;
        }

        bool tick()
        {
            if (!_ready_coros.empty() || !_timed_wait_coros.empty() || _num_outstanding_coros > 0)
            {
                if (!_ready_coros.empty())
                {
                    auto coro = _ready_coros.front();
                    _ready_coros.pop();

                    coro.resume();
                }

                while (!_timed_wait_coros.empty())
                {
                    auto it = _timed_wait_coros.begin();
                    if (std::chrono::high_resolution_clock::now() < it->first)
                        break;

                    for (auto& coro : it->second.first)
                    {
                        _ready_coros.push(coro);
                    }

                    _timed_wait_coros.erase(it);
                }

                return true;
            }

            return false;
        }

        void loop()
        {
            while (tick())
                ;
        }

    private:
        executor() = default;
        ~executor() = default;

        std::queue<coroutine_handle<>> _ready_coros;
        std::map<std::chrono::high_resolution_clock::time_point, 
          std::pair<std::list<coroutine_handle<>>, 
            std::unordered_map<coroutine_handle<>, 
              std::list<coroutine_handle<>>::iterator>>> _timed_wait_coros;

        int _num_outstanding_coros = 0;
    };

    // NB: try keep cancellation sources in scope, and it can freely pass tokens to other coroutines without worrying about becoming dangling
    class cancellation
    {
    private:
        struct impl
        {
            typedef std::shared_ptr<impl> ptr;

            impl() = default;
            ~impl() = default;

            impl(const impl&) = delete;
            impl(impl&&) = delete;
            impl& operator=(const impl&) = delete;

            void fire()
            {
                for (auto& entry : _registry)
                {
                    for (auto& f : entry.second)
                    {
                        f();
                    }
                }

                _registry.clear();
            }

            std::unordered_map<void*, std::deque<std::function<void()>>> _registry;
        };

        impl::ptr _impl_ptr;

    public:
        cancellation()
            : _impl_ptr(std::make_shared<impl>())
        {}

        ~cancellation() = default;
        cancellation(const cancellation&) = default;
        cancellation& operator=(const cancellation&) = default;
        cancellation(cancellation&& other) = default;

        // NB: try keep the token on the stack or in scope, it would keep effective during the course of co_await!
        class token
        {
        public:
            token(impl::ptr source = nullptr)
                : _source(source)
            {
            }

            token(const token&) = default;
            token(token&&) = default;

            void register_action(std::function<void()>&& f)
            {
                if (_source)
                {
                    auto r = _source->_registry.emplace(this, std::deque<std::function<void()>>{});
                    r.first->second.emplace_back(std::move(f));
                }
            }

            void unregister()
            {
                if (_source)
                {
                    _source->_registry.erase(this);
                }
            }

            ~token()
            {
                unregister();
            }

            static token& none()
            {
                static token s_none;
                return s_none;
            }

        private:
            impl::ptr _source;
        };

        token get_token()
        {
            return { _impl_ptr };
        }

        void fire()
        {
            _impl_ptr->fire();
        }
    };

    // NB: this class is intended for fire and forget type of coroutines
    // specifically, final_suspend returns suspend_never, so the coroutine will end its course by itself
    // OTOH, awaitable's final_suspend returns suspend_always, giving await_resume a chance to retrieve any return value or propagate any exception
    struct nawaitable
    {
        struct promise_type
        {
            nawaitable get_return_object()
            {
                return {};
            }

            auto initial_suspend()
            {
                return suspend_never{};
            }

            auto final_suspend()
            {
                return suspend_never{};
            }
        };
    };

    // The API level awaitable, which can be copied freely, while all the state is saved in the internal shared_ptr
    template <typename T>
    class awaitable
    {
    private:
        class impl
        {
        public:
            typedef std::shared_ptr<impl> ptr;

            impl() = default;
            impl(const impl&) = delete;
            impl(impl&&) = delete;
            impl& operator=(const impl&) = delete;
            ~impl() = default;

            explicit impl(bool suspend)
                : _suspend(suspend)
            {
            }

            explicit impl(std::chrono::high_resolution_clock::duration timeout)
                : _when(std::chrono::high_resolution_clock::now() + timeout)
            {
            }

            template <typename X>
            struct promise_type_base
            {
                X _value = X{};

                void return_value(X&& value)
                {
                    _value = std::move(value);
                }
            };

          #if 1
            template <> // error: explicit specialization in non-namespace scope ‘class pi::awaitable<T>::impl’
            struct promise_type_base<void>
            {
                void return_void()
                {
                }
            };
          #endif

            struct promise_type : promise_type_base<T>
            {
                awaitable get_return_object()
                {
                    return awaitable{ *this };
                }

                auto initial_suspend()
                {
                    // NB: we want the coroutine to run until the first actual suspension point, unless explicitly requested to suspend
                    return suspend_never{};
                }

                // NB: we need to enforce FIFO ordering
                std::list<coroutine_handle<>> _awaiter_coros;

                auto final_suspend()
                {
                    if (!_awaiter_coros.empty())
                    {
                        for (auto coro : _awaiter_coros)
                        {
                            executor::singleton().add_ready_coro(coro);
                            executor::singleton().decrement_num_outstanding_coros();
                        }
                        _awaiter_coros.clear();
                    }
                    return suspend_always{}; // NB: if we want to access the return value in await_resume, we need to keep the current coroutine around, even though coro.done() is now true! 
                }

                std::exception_ptr _exp;
                void set_exception(std::exception_ptr exp)
                {
                    _exp = exp;
                }
            };

            explicit impl(coroutine_handle<promise_type> coroutine)
                : _coroutine(coroutine) {}

            bool await_ready() noexcept
            {
                // if I'm enclosing a coroutine, use its status; otherwise, suspend if not ready
                return _coroutine ? _coroutine.done() : _when != std::chrono::high_resolution_clock::time_point{} ? std::chrono::high_resolution_clock::now() >= _when : _ready;
            }

            void await_suspend(coroutine_handle<> awaiter_coro) noexcept
            {
                if (!_coroutine)
                {
                    // I'm not enclosing a coroutine while I'm awaited (await resumable_thing{};), add the awaiter's frame

                    if (_when != std::chrono::high_resolution_clock::time_point{})
                    {
                        if (std::chrono::high_resolution_clock::now() >= _when)
                        {
                            // the timer has already expired - but we should have guarded this situation in await_ready, so this should not happen
                            // however, if this does happen, we should just put the awaiter_coro into the ready queue
                            assert(false); // let's make sure this does not happen actually, but nevertheless, we add the awaiter_coro to the ready queue
                            executor::singleton().add_ready_coro(awaiter_coro);
                        }
                        else
                        {
                            _awaiter_coros.emplace_back(awaiter_coro); // NB: guarantee FIFO ordering of the awaiters ...
                            executor::singleton().add_timed_wait_coro(_when, awaiter_coro);
                        }
                    }
                    else if (_suspend)
                    {
                        _awaiter_coros.emplace_back(awaiter_coro); // NB: guarantee FIFO ordering of the awaiters ...
                        executor::singleton().increment_num_outstanding_coros();
                    }
                    else
                    {
                        executor::singleton().add_ready_coro(awaiter_coro);
                    }
                }
                else
                {
                    // I'm waiting for some other coroutine to finish, the awaiter's frame can only be queued until my awaited one finishes
                    _coroutine.promise()._awaiter_coros.emplace_back(awaiter_coro); // NB: guarantee FIFO ordering of the awaiters ...
                    executor::singleton().increment_num_outstanding_coros();
                }
            }

            template <typename X>
            struct value
            {
                X _value = X{};
                X& get() { return _value; }
                X move() { return std::move(_value); }
            };

          #if 1
            template <>
            struct value<void>
            {
                void get() {}
                void move() {}
            };
          #endif

            template <typename X>
            struct save_promise_value
            {
                static void apply(value<X>& v, promise_type& p)
                {
                    v._value = std::move(p._value);
                }
            };

          #if 1
            template <> // error: explicit specialization in non-namespace scope ‘class pi::awaitable<T>::impl’
            struct save_promise_value<void>
            {
                static void apply(value<void>&, promise_type&)
                {
                }
            };
          #endif

            T await_resume()
            {
                if (_coroutine)
                {
                    if (_coroutine.promise()._exp)
                    {
                        _exp = _coroutine.promise()._exp;
                    }
                    else
                    {
                        save_promise_value<T>::apply(_value, _coroutine.promise());
                    }

                    // the coroutine is finished, but returned from final_suspend (suspend_always), so we get a chance to retrieve any exception or value
                    assert(_coroutine.done());
                    _coroutine.destroy();
                    _coroutine = nullptr;
                }

                _awaiter_coros.clear();
                _when = std::chrono::high_resolution_clock::time_point{};

                if (_exp)
                {
                    std::rethrow_exception(_exp);
                }

                return _value.get();
            }

            void set_ready()
            {
                if (!_awaiter_coros.empty())
                {
                    for (auto coro : _awaiter_coros)
                    {
                        executor::singleton().add_ready_coro(coro);
                        if (_suspend)
                        {
                            executor::singleton().decrement_num_outstanding_coros();
                        }
                        else
                        {
                            executor::singleton().remove_timed_wait_coro(_when, coro);
                        }
                    }

                    _awaiter_coros.clear();
                    _when = std::chrono::high_resolution_clock::time_point{};
                }
                _ready = true;
            }

            template <typename U = T, typename std::enable_if<!std::is_void<U>::value>::type* = nullptr>
            void set_ready(U&& value)
            {
                _value._value = std::forward<U>(value);
                set_ready();
            }

            void set_exception(std::exception_ptr exp)
            {
                _exp = exp;
                set_ready();
            }

            value<T>& get_value()
            {
                return _value;
            }

        private:
            value<T> _value;

            std::exception_ptr _exp;

            // the coroutine this awaitable is enclosing; this is created by promise_type::get_return_object
            coroutine_handle<promise_type> _coroutine{ nullptr };

            // the awaiter coroutine when this awaitable is a primitive - i.e. it doesn't enclose a coroutine, set only when _coroutine is nullptr!
            std::list<coroutine_handle<>> _awaiter_coros;
            std::chrono::high_resolution_clock::time_point _when; // NB: this should be initialized in the constructor, and cannot be modified 

            bool _ready = false;
            bool _suspend = false;
        };

        typename impl::ptr _impl_ptr;

        explicit awaitable(typename impl::promise_type& promise)
            : _impl_ptr(std::make_shared<impl>(coroutine_handle<typename impl::promise_type>::from_promise(promise)))
        {
        }

    public:
        struct promise_type : impl::promise_type
        {
        };

        awaitable()
            : _impl_ptr(std::make_shared<impl>())
        {
        }

        explicit awaitable(bool suspend)
            : _impl_ptr(std::make_shared<impl>(suspend))
        {
        }

        explicit awaitable(std::chrono::high_resolution_clock::duration timeout)
            : _impl_ptr(std::make_shared<impl>(timeout))
        {
        }

        awaitable(awaitable const&) = default;
        awaitable& operator=(awaitable const&) = default;
        awaitable(awaitable&& other) = default;
        ~awaitable() = default;

        bool operator==(const awaitable& other) const
        {
            return _impl_ptr == other._impl_ptr;
        }

        bool await_ready() noexcept
        {
            return _impl_ptr->await_ready();
        }

        void await_suspend(coroutine_handle<> awaiter_coro) noexcept
        {
            _impl_ptr->await_suspend(awaiter_coro);
        }

        T await_resume()
        {
            return _impl_ptr->await_resume();
        }

        void set_ready()
        {
            _impl_ptr->set_ready();
        }

        template <typename U = T, typename std::enable_if<!std::is_void<U>::value>::type* = nullptr>
        void set_ready(U&& value)
        {
            _impl_ptr->set_ready(std::forward<U>(value));
        }

        void set_exception(std::exception_ptr exp)
        {
            _impl_ptr->set_exception(exp);
        }

        T get_value()
        {
            return _impl_ptr->get_value().get();
        }

    private:
        // NB: use of template template parameter is to avoid recursive template instantiation when retrieving the proxy type!
        //template < template <typename> class _awaitable > // TODO: try without template template parameter
        static nawaitable await_one(awaitable a, awaitable<awaitable> r, cancellation::token ct = cancellation::token::none())
        {
            // NB: the cancellation token will remain in scope until the current function returns
            ct.register_action([&a] { a.set_exception(std::make_exception_ptr(std::exception("await_one.cancellation"))); });

            try
            {
                // TODO: await_resume moves the result out, and here we need to retain the value, and probably need to set it back to 'a'
                // the challenge now is to deal gracefully both the void and non-void return types with co_await!
                co_await a;
                r.set_ready(a);
            }
            catch (...)
            {
                r.set_exception(std::current_exception());
                return;
            }
        }

        // NB: use of template template parameter is to avoid recursive template instantiation when retrieving the proxy type!
        //template < template <typename> class _awaitable > // TODO: try without template template parameter
        //static nawaitable await_one(awaitable<awaitable> aa, awaitable<awaitable> r, cancellation::token ct = cancellation::token::none())
        //{
        //    // NB: the cancellation token will remain in scope until the current function returns
        //    ct.register_action([&aa] { aa.set_exception(std::make_exception_ptr(std::exception("await_one.cancellation"))); });

        //    try
        //    {
        //        co_await aa;
        //        r.set_ready(aa.get_value());
        //    }
        //    catch (...)
        //    {
        //        r.set_exception(std::current_exception());
        //        return;
        //    }
        //}

        static nawaitable await_one(awaitable a, awaitable<void> r, size_t& count = 0, cancellation::token ct = cancellation::token::none())
        {
            // NB: the cancellation token will remain in scope until the current function returns
            ct.register_action([&a] { a.set_exception(std::make_exception_ptr(std::exception("await_one.cancellation"))); });

            try
            {
                co_await a;

                if (count > 0 && --count == 0)
                {
                    r.set_ready();
                }
            }
            catch (...)
            {
                r.set_exception(std::current_exception());
                return;
            }
        }

    public:
        static awaitable<awaitable> when_any(std::deque<awaitable>& awaitables, cancellation::token ct = cancellation::token::none())
        {
            awaitable<awaitable> r{ true };

            for (auto a : awaitables)
            {
                // NB: cannot register the cancellation action here, since we cannot maintain the call frame here
                // the cancellation token will be destructed when this function goes out of scope even before await_one ends
                // thus unregisters the registered action!
                await_one(a, r, ct);
            }

            return r;
        }

        // NB: input type of awaitable&& makes no sense, since the final result is a reference to an most decayed awaitable, an rvalue is temporary in nature, and thus should not be referenced!
        // so we don't define the following verion
        // friend awaitable<ref> operator||(awaitable&& a1, awaitable&& a2);
        // as well as the combinations of awaitable&& with other awaitable types

        // [1]
        friend awaitable<awaitable> operator||(awaitable a1, awaitable a2)
        {
            awaitable<awaitable> r{ true };
            await_one(a1, r);
            await_one(a2, r);
            return r;
        }

        // [2-1]
        friend awaitable<awaitable> operator||(awaitable<awaitable> a1, awaitable a2)
        {
            // NB: instead of creating another awaitable<awaitable>, we just reuse a1! 
            await_one(a2, a1);
            return a1;
        }

        // [2-2]
        friend awaitable<awaitable> operator||(awaitable a1, awaitable<awaitable> a2)
        {
            return a2 || a1;
        }

        // [3] - would this collide with the method as defined in the next level (when instantiating awaitable<awaitable> - probably
        //friend awaitable<awaitable> operator||(awaitable<awaitable> a1, awaitable<awaitable> a2)
        //{
        //    awaitable<awaitable> r{ true };
        //    await_one(a1, r);
        //    await_one(a2, r);
        //    return r;
        //}

        static awaitable<void> when_all(std::deque<awaitable>& awaitables, cancellation::token ct = cancellation::token::none())
        {
            awaitable<void> r{ true };

            size_t count = awaitables.size(); // NB: count remains on the stack due to the co_await below
            for (auto a : awaitables)
            {
                await_one(a, r, count, ct);
            }

            co_await r;
        }

        friend awaitable<void> operator&&(awaitable a1, awaitable a2)
        {
            awaitable<void> r{ true };

            size_t count = 2; // NB: count remains on the stack due to the co_await below
            await_one(a1, r, count);
            await_one(a2, r, count);

            co_await r;
        }

        template<typename U>
        friend std::enable_if_t<!std::is_void<U>::value, awaitable<void>> operator&&(awaitable<void> a1, awaitable<U> a2);

        template<typename U>
        friend std::enable_if_t<!std::is_void<U>::value, awaitable<void>> operator&&(awaitable<U> a1, awaitable<void> a2);
    };

    template <typename T>
    std::enable_if_t<!std::is_void<T>::value, awaitable<void>> operator&&(awaitable<void> a1, awaitable<T> a2)
    {
        awaitable<void> r{ true };

        size_t count = 2; // NB: count remains on the stack due to the co_await below
        awaitable<void>::await_one(a1, r, count);
        awaitable<T>::await_one(a2, r, count);

        co_await r;
    }

    template <typename T>
    std::enable_if_t<!std::is_void<T>::value, awaitable<void>> operator&&(awaitable<T> a1, awaitable<void> a2)
    {
        return a2 && a1;
    }

    auto operator co_await(std::chrono::high_resolution_clock::duration duration)
    {
        return awaitable<void>{duration};
    }
}

