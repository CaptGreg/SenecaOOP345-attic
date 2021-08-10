// https://modernescpp.com/index.php/c-20-thread-synchronization-with-coroutines

// senderReceiver.cpp
// g++-10 -fcoroutines -std=c++2a   senderReceiver.cpp -o senderReceiver -lcppcoro -pthread && ./senderReceiver

#include <coroutine>
// #include <chrono>
#include <iostream>
// #include <functional>
// #include <string>
#include <stdexcept>
#include <atomic>
#include <thread>

class Event {
 public:

    Event() = default;

    Event(const Event&) = delete;
    Event(Event&&) = delete;
    Event& operator=(const Event&) = delete;
    Event& operator=(Event&&) = delete;

    class Awaiter;
    Awaiter operator co_await() const noexcept;

    void notify() noexcept;

 private:

    friend class Awaiter;
  
    mutable std::atomic<void*> suspendedWaiter{nullptr};
    mutable std::atomic<bool> notified{false};

};

class Event::Awaiter {
 public:
  Awaiter(const Event& eve): event(eve) {}

  bool await_ready() const;
  bool await_suspend(std::coroutine_handle<> corHandle) noexcept;
  void await_resume() noexcept {}

 private:
    friend class Event;

    const Event& event;
    std::coroutine_handle<> coroutineHandle;
};

bool Event::Awaiter::await_ready() const {                            // (7)
  
    // allow at most one waiter
    if (event.suspendedWaiter.load() != nullptr){
        throw std::runtime_error("More than one waiter is not valid");
    }
  
    // event.notified == false; suspends the coroutine
    // event.notified == true; the coroutine is executed such as a usual function
    return event.notified;
}
                                                                     // (8)
bool Event::Awaiter::await_suspend(std::coroutine_handle<> corHandle) noexcept {

    coroutineHandle = corHandle;                                    
  
    if (event.notified) return false;
  
    // store the waiter for later notification
    event.suspendedWaiter.store(this);

    return true;
}

void Event::notify() noexcept {                                        // (6)
    notified = true;
  
    // try to load the waiter
    auto* waiter = static_cast<Awaiter*>(suspendedWaiter.load());
 
    // check if a waiter is available
    if (waiter != nullptr) {
        // resume the coroutine => await_resume
        waiter->coroutineHandle.resume();
    }
}

Event::Awaiter Event::operator co_await() const noexcept {
    return Awaiter{ *this };
}

struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

Task rx(Event& event) {                                        // (3)
  auto start = std::chrono::high_resolution_clock::now();
  co_await event;                                                 
  std::cout << "Got the notification! " << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Waited " << elapsed.count() << " seconds." << std::endl;
}

using namespace std::chrono_literals;

int app()
{
  std::cout << std::endl;
  
  std::cout << "++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << " Notification before waiting" << std::endl;
  Event event{};
  auto txThread = std::thread([&event]{ event.notify(); });  // (1)
  auto rxThread = std::thread(rx, std::ref(event));   // (4)
  
  rxThread.join();
  txThread.join();
  std::cout << std::endl;
  
  std::cout << "++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "Notification after 2 seconds waiting" << std::endl;
  // Event event2{};
  rxThread = std::thread(rx, std::ref(event));  // (5)
  txThread = std::thread([&event] { 
    std::this_thread::sleep_for(2s); 
    event.notify(); 
  }); // (2)
  
  rxThread.join();
  txThread.join();
  std::cout << std::endl;


  std::cout << "++++++++++++++++++++++++++++++++++++++++++\n";
  std::cout << "reuse threads: Notification after 1 seconds waiting" << std::endl;
  rxThread = std::thread(rx, std::ref(event));  // (5)
  txThread = std::thread([&event]{
    std::cout << "txThread sleeping\n";
    std::this_thread::sleep_for(1s);
    std::cout << "txThread woke up\n";
    event.notify();                                               // (2)
    std::cout << "txThread notified event\n";
  });
  
  rxThread.join();
  txThread.join();
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

int main()
{
  int r;

  try {
    r = app();
    return r;
  } catch (const std::exception e) {
    std::cerr << "It threw " << e.what() << "\n";
  } catch (...) {
    std::cerr << "It threw ...\n";
  }
  return EXIT_FAILURE;
}
