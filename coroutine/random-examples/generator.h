#pragma once
//
// Created by rogerv on 9/29/19.
//
// Based on example code (but with significant cleanup) found in:
// Rainer Grimm, Concurrency with Modern C++ (Leanpub, 2017 - 2019), 207-209.
//

#ifndef GENERATOR_H
#define GENERATOR_H

// infiniteDataStream.cpp
#include <coroutine>
// #include <memory>
#include <iostream>           // std::exit

namespace coro_exp {

  template<typename T>
  class generator {
  public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
  private:
    handle_type coro;
  public:
    explicit generator(handle_type h) : coro(h) {}
    generator(const generator &) = delete;                 // copy ctor
    generator(generator &&oth) noexcept : coro(oth.coro) { // move ctor
      oth.coro = nullptr;
    }
    generator &operator=(const generator &) = delete;      // copy assignment operator
    generator &operator=(generator &&other) noexcept {     // move assignment operator
      coro = other.coro;
      other.coro = nullptr;
      return *this;
    }
    ~generator() {
      if (coro) {
        coro.destroy();
      }
    }

    bool next() {
      coro.resume();
      return not coro.done();
    }

    T getValue() {
      return coro.promise().current_value;
    }

    struct promise_type {
    private:
      T current_value{};
      friend class generator;
    public:
      promise_type() = default;
      ~promise_type() = default;
      promise_type(const promise_type&) = delete;                  // copy ctor
      promise_type(promise_type&&) = delete;                       // move ctor
      promise_type &operator=(const promise_type&) = delete;       // copy assignment operator
      promise_type &operator=(promise_type&&) = delete;            // move assignment operator

      auto initial_suspend() {
        return std::suspend_always{};
      }

      auto final_suspend() {
        return std::suspend_always{};
      }

      auto get_return_object() {
        return generator{handle_type::from_promise(*this)};
      }

      auto return_void() {
        return std::suspend_never{};
      }

      auto yield_value(T some_value) {
        current_value = some_value;
        return std::suspend_always{};
      }

      void unhandled_exception() {
        std::exit(1);
      }
    };
  };

} // coroutn_exp

#endif //GENERATOR_H
