// https://gist.github.com/DaemonSnake/ec57b1c54f242d1320243720fb06ff0e

// g++-10 -fcoroutines -fexceptions    -std=c++2a -fmax-errors=1 -Wfatal-errors -Wall -ggdb variant_generator.cpp -o variant_generator -lcppcoro -pthread && ./variant_generator

#include <concepts>
#include <coroutine>
#include <type_traits>
#include <variant>

// in godbolt: #include "https://raw.githubusercontent.com/DaemonSnake/unconstexpr-cpp20/godbolt/godbolt.hpp"
#include "godbolt.hpp"


namespace lazy_auto {

template<class T>
consteval unconstexpr::id_value unique_id(T const &item) {
    return unconstexpr::unique_id<T>{item};
}

template<class... Args>
struct type_list
{
    template<class T>
    using add = type_list<Args..., T>;

    template<template<class...> class H>
    using transfer = H<Args...>;
};

template<class... Args>
using auto_variant = std::variant<std::monostate, Args...>;

template<template<class...> class Holder, class meta, template<class, class...> class Writer>
struct final_eval
{
    struct id_t {};
    static constexpr auto id = unique_id(id_t{});
    static constexpr auto current = unconstexpr::current_value<meta, id>();
    using type = std::remove_pointer_t<decltype(current)>;
    using res = typename type::template transfer<Holder>;

    template<class... Args>
    using writer = Writer<res, Args...>;

    static_assert(type::template transfer<writer>::value);
};

#ifndef __clang__
# pragma GCC diganostic push
# pragma GCC diagnostic ignored "-Wnon-template-friend"
#endif

template<class, class T>
struct init_tag {
    friend constexpr void init(init_tag, void *&, T&&);
    friend constexpr void init(init_tag, void *&, T&);
    friend constexpr void init(init_tag, void *&, T const &);
};

template<class>
struct cleanup_tag {
    friend constexpr void destroy(cleanup_tag, void *);
};

#ifndef __clang__
# pragma GCC diagnostic pop
#endif

template<class Ret, class V>
constexpr void init(void *&addr, V&& item) {
    if (addr)
        *static_cast<Ret*>(addr) = std::forward<V>(item);
    else
        addr = new Ret(std::forward<V>(item));
}

template<class Self, class Ret, class T>
struct friend_writer_type
{
    friend constexpr void init(init_tag<Self, T>, void *&addr, T&& item) { init<Ret>(addr, std::move(item)); } 
    friend constexpr void init(init_tag<Self, T>, void *&addr, T& item) { init<Ret>(addr, item); } 
    friend constexpr void init(init_tag<Self, T>, void *&addr, T const item) { init<Ret>(addr, item); } 
};

template<class Self, class Ret, class... Args>
struct friend_writer_types: friend_writer_type<Self, Ret, Args>...
{
    friend constexpr void destroy(cleanup_tag<Self>, void *data) {
        if (data)
            delete static_cast<Ret*>(data);
    }

    static constexpr bool value = true;
};

template<class T>
consteval T *null_of() { return nullptr; }

template<template<class...> class Holder = auto_variant, unconstexpr::id_value Id = unique_id([]{})>
class variant_generator
{
public:
    struct promise_type;

private:
    using meta = unconstexpr::meta_value<null_of<type_list<>>(), 1, Id>;
    using handle_t = std::coroutine_handle<promise_type>;
    handle_t handle = {};

    using cleanup_tag = lazy_auto::cleanup_tag<variant_generator>;
    template<class Ret, class... Args>
    using friend_writer = lazy_auto::friend_writer_types<variant_generator, Ret, Args...>;

    template<class T>
    using holder_t = Holder<T>;

    template<class T>
    static consteval bool add_type() {
        struct reader {};
        constexpr auto read_id = unique_id(reader{});
        constexpr auto curr_value = unconstexpr::current_value<meta, read_id>();
        using cur_type = std::remove_pointer_t<decltype(curr_value)>;
        using new_type = typename cur_type::template add<std::decay_t<T>>;
        constexpr auto new_value = static_cast<new_type *>(nullptr);
        struct writer {};
        return noexcept(unconstexpr::set<meta, new_value, unique_id(writer{})>());
    }

    template<class T>
    static constexpr auto handle_as(handle_t const &handle) {
        auto *ptr = (handle ? handle.promise().template as<T>() : nullptr);
        return (ptr != nullptr ? std::move(*ptr) : T{});
    }

public:
    explicit constexpr variant_generator(handle_t &&handle):
        handle(std::move(handle))
        {}

    variant_generator() = delete;

    constexpr variant_generator(variant_generator &&other)
    {
        std::swap(handle, other.handle);   
    }

    variant_generator& operator=(const variant_generator&) = delete;

    constexpr ~variant_generator() {
        using res = typename final_eval<Holder, meta, friend_writer>::res;
        if (handle) handle.destroy();
    }
 
    struct promise_type
    {
        constexpr promise_type() = default;

        auto get_return_object() & {
            return variant_generator{handle_t::from_promise(*this)};
        }

        static consteval std::suspend_never initial_suspend() noexcept { return {}; }
        static consteval std::suspend_always final_suspend() noexcept { return {}; }

        template<class T, bool = add_type<T>()>
        constexpr void return_value(T&& item) {
            using type = std::decay_t<T>;
            using tag = init_tag<variant_generator, type>;
            init(tag{}, data, std::forward<T>(item));
        }

        template<class T, bool = add_type<T>()>
        constexpr std::suspend_always yield_value(T&& item) {
            using type = std::decay_t<T>;
            using tag = init_tag<variant_generator, type>;
            init(tag{}, data, std::forward<T>(item));
            return {};
        }

        constexpr ~promise_type() {
            destroy(cleanup_tag{}, this->data);
        }

        void await_transform() = delete;
        [[noreturn]] static void unhandled_exception() { throw; }

        template<class T>
        constexpr T *as() const {
            return static_cast<T*>(data);
        }

    private:
        void *data = nullptr;
    };

    template<bool Owner>
    class iter_t {
    private:
        handle_t handle;

    public:
        constexpr ~iter_t() {
            if constexpr (Owner) handle.destroy();
        }

        constexpr void operator++() {
            handle.resume(); 
        }

        constexpr auto operator*() const {
            using res = typename final_eval<Holder, meta, friend_writer>::res;
            return handle_as<res>(handle);
        }

        constexpr bool operator==(std::default_sentinel_t) const {
            return !handle || handle.done(); 
        }
 
        explicit iter_t(const handle_t& coroutine):
            handle{coroutine} {}
    };

    constexpr auto begin() const & { return iter_t<false>{handle}; }
    constexpr auto begin() && {
        handle_t tmp = {};
        std::swap(tmp, handle);
        return iter_t<true>{std::move(tmp)};
    }
    constexpr auto end() const & { return std::default_sentinel_t{}; }

    constexpr auto operator*() const {
        using res = typename final_eval<Holder, meta, friend_writer>::res;
        return handle_as<res>(handle);
    }
}; // class variant_generator
    
} // namespace lazy_auto
int main()
{
}
