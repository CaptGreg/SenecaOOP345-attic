// https://www.jeremyong.com/cpp/2021/01/04/cpp20-coroutines-a-minimal-async-framework/

// GB windows filesystem (HANDLE) code

#include <string>
#include <coroutine>
#include <filesystem>

#if 0


    Home About 

C++20 Coroutines: sketching a minimal async framework
Lambdas? pfft.

04 January 2021

C++20 coroutines are here! Having spent some 20-odd hours with it, I am by no means an expert on the feature, but I wanted to jot down my initial impressions and provide some pointers looking to get their feet wet and try their hand at implementing a coroutine-powered async framework themselves. 
Spoiler-alert: for engineers that need to parallelize their code, in particular, engineers used to needing to jobify CPU work using lambdas or function objects, I think coroutines are a godsend in writing expressive code. 
I have no doubt that they will be a staple in future AAA game engines, simulation software, and more. 
What follows below is a pretty abbreviated tour of coroutines, as I really dont think you can notruly grok it without writing a lot of code yourself, stepping in with a debugger, littering your code with dozens of printfs, etc. 
However, I hope that this post both motivates the use of C++20 coroutines, and also demonstrates that once you do understand the language facilities for coroutines more, you can do a surprising amount with not a lot of code.
Basic Concepts

This post is going to be overwhelmingly more applied in nature, as opposed to delving into the nitty gritty. That said, we still need to get on the same page about some of the terms you will see when working with coroutines in C++20. Note that to use a coroutine based framework written by someone else, you can actually get away with surprisingly little knowledge (although there are some gotchas as we will see).

First up is the coroutine itself. 
A coroutine is a function which can be suspended and resumed (as opposed to executing from start to finish). 
As a function, coroutines can be invoked by other coroutines, or by other functions. 
As a technicality, the main function itself cannot be a coroutine (you cant suspend from it). 
A function becomes a coroutine the moment you suspend from within its body by invoking co_await or co_yield. 
Note that if you call a coroutine c (remember, coroutines are just functions) from a function f, the function f is not a coroutine unless you co_await the result of c. 
In other words, calling a coroutine isnt sufficient to make the caller a coroutine. 
Something must happen in the body of the caller which may cause it to suspend for the caller to be a coroutine.

The next concept to understand is the promise_type. 
A promise_type is an interface type that can be thought of as a control object the programmer can use to inject custom behaviour during the lifecycle of a coroutine. Some sample coroutine lifecycle events are:

    An allocation is happening to store the state of a coroutine (activation frame plus any additional data)
    Coroutine is initially created
    Coroutine is about to suspend
    Coroutine has just finished suspending
    Coroutine is completed and is now returning a value

Each of these events corresponds to a method you can implement to specify custom behavior.

The final concept to grok when working with coroutines is the awaitable. 
An awaitable is anything that can be the argument of the co_await unary operator. 
In order to qualify as an awaitable such that co_await a is valid, either, a has a type with operator co_await implemented, or a has a type that implements the awaitable trait, or, there is an await_transform method present on the parent coroutine (remember, an await happens in the body of a coroutine). 
Regardless of the mechanism, what is consumed by the co_await operator is a object (produced via any of the means above) which possesses the following methods:

    An await_ready that describes if the suspend needs to happen at all (useful for short-circuiting awaits for which the associated work was already completed)
    An await_resume which specifies what happens once the await is over
    An await isuspend which is passed a coroutine_handle that can be used to resume the parent coroutine later

Motivating example

Suppose we want to read a file from disk, decode it, and then use the results.
Furthermore, suppose we want to run some work in parallel while the file is being read and while the file is being decoded. 
Here is what this code might look like with coroutines.
#endif

#if 0
// GB task not defined yet
task<std::string> read_file(std::filesystem::path const& path) {
  // File reading operation
  // Assumes a win32 implementation using overlapped IO, but
  // this same concept applies to epoll-style file reading as well.
  HANDLE completion_event = win_init_read_file(path); // Windows filesystem HANDLE 

  co_await completion_event;
}

task<std::vector<uint8_t>> decode_png(std::string const& data) {
  // Indicate that this function should run as a separate task
  // on a different thread
  co_await isuspend();

  std::vector<uint8_t> out;
  // PNG decoding code
  co_return out;
}

task<> some_expensive_function1()
{
  co_await isuspend();

  // Some expensive code
}

task<int> some_expensive_function2()
{
  co_await isuspend();

  // Some expensive code that produces an integer result
  co_return result;
}

task<> some_expensive_function3(int)
{
  co_await isuspend();

  // Some expensive code
}

void some_loading_function(std::filesystem::path const& path) {
  task<std::string> read_task = read_file(path);

  // Do two other useful things in parallel while the file
  // is being read
  task<> task1 = some_expensive_function1();
  // The second task produces an integral result
  task<int> task2 = some_expensive_function2();
  
  // Await the file read and pass the data to the png decoder
  task<std::vector<uint8_t>> png_task =
    decode_png(co_await read_task);

  int task2_result = co_await task2;

  // Do other useful thing while png is decoding using the
  // result from task2
  some_expensive_function3(task2_result);

  // Ensure task1 is also finished before proceeding
  co_await task1;

  do_something_with_png(co_await png_task);
}
#endif

#if 0
When looking at the implementation of some_loading_function, it is important to imagine how difficult this would be to implement with a traditional task-graph style API. 
Most of these functions would need to be written as function objects, or we would have lambdas littered throughout. 
However the task graph is implemented, the resulting code would read necessarily non-linearly, especially in this example where we are essentially forking a function and joining it in a non-trivial manner. 
Suffice it to say, I believe the interface shown in the example above will likely be terser and more readable. 
So, let us implement it!

There are three elements that we need to implement to make the above code snippet a reality.

    First, we need some way to await for some OS event to signal completion. 
    That is, we need to wait for something to complete outside of our program.
    Second, we need a way to suspend execution of a function in the current thread in such a way that this function is resumed automatically in a different thread. 
    This is how we will schedule multiple tasks to complete in parallel.
    Finally, we need a way to await for work that we have enqueued via the suspension routine above.

The underlying substrate on which our coroutines will run also needs to be defined. 
Let us call this the Central Task Processor or CTP for short. 
Its interface should be as follows:
#endif

class CTP { // Central Task Processor or CTP for short. 

public:
  // For simplicity, we will assume a global singleton instance is available
  static CTP& instance();

  // The constructor should spawn thread_count threads and begin the run loop of each thread
  CTP(int thread_count);

  // When a coroutine is enqueued, the coroutine is passed to one of the CTP threads, and
  // eventually coroutine.resume() should be invoked when the thread is ready. You can
  // implement work stealing here to "steal" coroutines on busy threads from idle ones, or
  // any other load balancing/scheduling scheme of your choice.
  //
  // NOTE: This should (obviously) be threadsafe
  void enqueue(std::coroutine_handle<> coroutine);
};

#if 0
This simple interface is all we need to indicate that a coroutine that was previously suspended is now ready to resume. 
The CTP will presumably find a thread ready to accept work and resume the coroutine there, or enqueue it for later. 
Now, let us handle the actual coroutine suspension code.
Awaiting an OS event

In order to co_await for some OS handle that will be completed later, we need to implement operator co_await, taking as its sole parameter, the argument to the unary co_await operator. 
Thankfully, this is really quite simple. 
Again, using windows as an example, we might implement this as follows:
#endif

struct event_awaiter {
  HANDLE event; // Windows filesystem HANDLE 
  bool await_ready() const noexcept {
    // Check if we need to bother suspending at all by seeing if the
    // event was already signaled with a non-blocking wait.
    return WaitForSingleObject(event, 0) == WAIT_OBJECT_0;
  }

  void await isuspend(std::coroutine_handle<> coroutine) const noexcept {
    // The coroutine handle passed here can be copied elsewhere and resumed
    // when the event is signaled. Here, we spawn a dedicated thread for
    // demonstration purposes, but you should have a dedicated low-priority
    // thread to queue waiting events to.
    std::thread thread{[coroutine, event]() noexcept {
      WaitForSingleObject(event, INFINITE);

      // The CTP will call coroutine.resume() on an available thread now
      CTP::instance().enqueue(coroutine);
    }};
    thread.detach();
  }

  void await_resume() noexcept {
    // This is called after the coroutine is resumed in the async thread
    printf("Event signaled, resuming on thread %i\n", get_thread_id());
  }
};

#if 0
To make our event HANDLE a valid argument to the co_await operator, we also need to add an overload for returning the event_awaiter defined above.

inline event_awaiter operator co_await(HANDLE event) noexcept
{
  return {event};
}

Now, we can do something like the following:
#endif

// Retrieve an event that has yet to be signaled
HANDLE event = some_routine();

// Assuming the event is unsignaled, this task will now suspend (await isuspend
// will be called above). The thread that this code originally ran on is now
// free to resume a different coroutine enqueued earlier (if any) or simply
// wait until a task is ready.
co_await event;

// At this point, await_resume() above has been called and we are now running
// on a potentially different CTP thread.
some_other_routine();

#if 0
Neat!
Suspend code execution

So, the code above lets us suspend a coroutine and resume it when the OS tells us it is ready. 
Let us now figure out how to suspend a task to resume immediately on a different CTP thread. 
Again, we will do this using an awaitable.
#endif

inline auto suspend() noexcept {
  struct awaiter {
    // Unlike the OS event case, there is no case where we suspend and the work
    // is immediately ready
    bool await_ready() const noexcept { return false; }

    // Since await_ready() always returns false, when suspend is called, we will
    // always immediately suspend and call this function (which enqueues the coroutine
    // for immediate reactivation on a different thread)
    void await isuspend(std::coroutine_handle<> coroutine) noexcept {
      CTP::instance().enqueue(coroutine);
    }

    void await_resume() const noexcept {
      printf("Suspended task now running on thread $i\n", get_thread_id());
    }
  };
  return awaiter{};
}

#if 0
This is even more straightforward than the OS event case. In action, this looks like the following:
#endif

// Note, we do not yet know what a "task" is. That will be introduced next
task<int> async_routine() {
  // Suspend this current coroutine and enqueue it to our CTP
  co_await isuspend();

  // At this point, we are running on a thread that resumed this coroutines handle
  // In this case, we compute some integer and "co_return" it
  co_return some_expensive_thing();
}

#if 0
Introducing the task

So far, we have implemented two awaiters which describe what happens when the coroutine itself is suspended and resumed. 
However, this only addresses one side of the story. 
Namely, we have not described anywhere in code what ishould happen to the thread which began the execution of the suspended coroutine. 
Take the async_routine function we wrote above which produces an int result asynchronously. 
How should we define where the result is written to? 
Also, how does the caller of async_routine wait for it to finish? 
The answer is with the promise_type concept which we will wrap in an abstraction called a task.
#endif

template <typename T>
struct task {
  // The return type of a coroutine must contain a nested struct or type alias called `promise_type`
  struct promise_type {
    // Keep a coroutine handle referring to the parent coroutine if any. That is, if we
    // co_await a coroutine within another coroutine, this handle will be used to continue
    // working from where we left off.
    std::coroutine_handle<> precursor;

    // Place to hold the results produced by the coroutine
    T data;

    // Invoked when we first enter a coroutine. We initialize the precursor handle
    // with a resume point from where the task is ultimately suspended
    task get_return_object() noexcept
    {
      return {std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    // When the caller enters the coroutine, we have the option to suspend immediately.
    // Let us choose not to do that here
    std::suspend_never initial_suspend() const noexcept { return {}; }

    // If an exception was thrown in the coroutine body, we would handle it here
    void unhandled_exception() {}

    // The coroutine is about to complete (via co_return or reaching the end of the coroutine body).
    // The awaiter returned here defines what happens next
    auto final_suspend() const noexcept {
      struct awaiter {
        // Return false here to return control to the threads event loop. Remember that we are
        // running on some async thread at this point.
        bool await_ready() const noexcept { return false; }

        void await_resume() const noexcept {}

        // Returning a coroutine handle here resumes the coroutine it refers to (needed for
        // continuation handling). If we wanted, we could instead enqueue that coroutine handle
        // instead of immediately resuming it by enqueuing it and returning void.
        std::coroutine_handle<> await isuspend(std::coroutine_handle<promise_type> h) {
          auto precursor = h.promise().precursor;
          if (precursor)
          {
            return precursor;
          }
          return std::noop_coroutine();
        }
      };
      return awaiter{};

      // When the coroutine co_returns a value, this method is used to publish the result
      void return_value(T value) noexcept
      {
        data = std::move(value);
      }
    }
  };

  // The following methods make our task type conform to the awaitable concept, so we can
  // co_await for a task to complete

  bool await_ready() const noexcept {
    // No need to suspend if this task has no outstanding work
    return handle.done();
  }

  T await_resume() const noexcept {
    // The returned value here is what `co_await our_task` evaluates to
    return std::move(handle.promise().data);
  }

  void await isuspend(std::coroutine_handle<> coroutine) const noexcept {
    // The coroutine itself is being suspended (async work can beget other async work)
    // Record the argument as the continuation point when this is resumed later. See
    // the final_suspend awaiter on the promise_type above for where this gets used
    handle.promise().precursor = coroutine;
  }

  // This handle is assigned to when the coroutine itself is suspended (see await isuspend above)
  std::coroutine_handle<promise_type> handle;
};

#if 0
OK, there is understandably a lot to unpack here, so let is look at a simple function and establish a chronology for what happens and why.
#endif

task<int> async_routine() {
  co_await isuspend();

  co_return some_expensive_thing();
}

task<int> another_async_routine() {
  // Enqueue 3 invocations of async_routine such that they are all running
  // in parallel, assuming we have 3 threads available
  // The current thread enters `async_routine`, then encounters the co_await of the
  // suspend() above. This yields control back so we can spawn the next async_routine.
  // In each case, get_return_object is called to construct a task with memory to
  // store the returned fresult. Furthermore, the coroutine handle to each of these
  // invocations is stored so that when async_routine suspends internally, this
  // caller can continue where it left off.
  task<int> t1 = async_routine();
  task<int> t2 = async_routine();
  task<int> t3 = async_routine();

  // Because t2 is itself an awaitable, a few things happen here. First, we check
  // in await_ready above if the coroutine handle associated with t2 has already
  // finalized. If so, we dont need to suspend and can just retrieve the result
  // directly (internally invokes `return_value`). If t2 hasnt finished yet, we
  // assign the precursor handle to the current resume point and suspend. When t2
  // finishes later, its final_suspend will find this precursor and resume here.
  // Note that its possible that after this line, we end up running on a different
  // thread (the thread that finished t2)
  int r2 = co_await t2;

  do_something_else();

  // Resume points work in the middle of expressions as well
  co_return ((co_await t1) + r2 + (co_await t3));
}

#if 0
It is very difficult to describe in exact terms how each control method for all the different awaitables involved coordinate to enable the code above to work. 
In general terms though, this is roughly what is happening:

    Every time we enter a coroutine (denoted as a function which returns a task), the caller constructs the task object which contains storage space for data to be returned by the coroutine (you will need to roll a task<void> specialization as well if you do not want that)
    The task also stores a precursor which refers to the resume point. 
    Initially, the precursor is just the point in code directly after the task construction.
    The task itself can suspend, either by awaiting another task, or by awaiting one of the two awaitables we made before.
    When a task completes (by working it is way to the end of its function body or by encountering a co_return statement), it immediately resumes the precursor, which presumably suspended because of this task.

How do I await a coroutine from something that is not a coroutine?

This is a fair question. 
At some point, the entry into a coroutine must come from something that is not a coroutine. 
After all, the standard dictates that main must not be a coroutine. 
To do this, you will need to implement your own await method which waits on a condition_variable or OS event directly. 
You can store this condition_variable on the task type, then set it in the tasks final_suspend method for example.

In my codebase, this is referred to as a jtask (short for joinable task) and it is otherwise identical to the task above except for this additional signaling behavior.
Some implementation details I glossed over

    You can override operator new and operator delete to manage allocations of your task types yourself. Note that I do not believe these are guaranteed to be called if the compiler can elide the allocation altogether, so do not implement these overrides with visible side-effects
    When suspending a coroutine, remember to ensure that pointers and references used in the body of the coroutine must have a valid lifetime extended until the coroutine exits. 
    This is obvious but potentially is a pitfall (similar to capturing pointers and references in a lambda)
    If doing this implementation yourself, you can also apply a similar pattern in how we handled continuations and suspensions here to handle co_yield expressions.
    You will likely need a task<void> specialization in order to handle coroutines that execute but do not return anything
    My own code has additional state in the task type to handle things like tracking history and file and line information (simplifies debugging). 
    Also, the code is instrumented to support labels in profiling tools like Pix.

Conclusion

C++20 coroutines are undoutedly powerful. 
I did not go into all the compiler optimizations that can kick in to do fancy things like elide heap allocations, but there are significant advantages to migrating to use coroutines instead of the typical task-graph abstraction. 
However, as a powerful abstraction (with extreme amounts of flexibility), C++20 coroutines are undeniably difficult to learn, at least at first. 
To get to my current level of proficiency, I essentially made dozens of example projects with copious amounts of printf statements to help me understand the control flow. 
I highly recommend doing this exercise yourself, as you may find you would prefer to make different design choices than those offered here.

As I mentioned before, this post is not meant to be exhaustive at all, but should hopefully serve to whet your appetite in terms of what is possible and introduce the general terminology. 
My engine (which I can not publish for reasons at the moment) benefits a great deal from coroutines with a task and thread pool implementation measuring only several hundreds of lines long! 
The real benefit however is in the calling code. 
After writing this once, I have rarely needed to think much about the mechanics. 
The only caution needed perhaps is to remember that the thread a coroutine starts on is not necessarily the same thread that it ends on (as implemented here). 
Hopefully, you will derive some benefits from C++20 coroutines in the near future also!

#endif
