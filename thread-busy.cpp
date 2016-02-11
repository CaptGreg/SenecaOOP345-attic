// GB --- I cleaned this up, modified, made it loop while thread busy

// http://stackoverflow.com/questions/9094422/how-to-check-if-a-stdthread-is-still-running

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// How can I check if a std::thread is active (in a platform independent way)?
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
// If you are willing to make use of C++11 std::async and std::future for running your tasks, 
// then you can utilize the wait_for function of std::future to check if the thread is still 
// running in a neat way like this:



#include <chrono>
#include <thread>
 
void nap_nsec(int nsec)
{
    std::this_thread::sleep_for( std::chrono::nanoseconds ( nsec ));
}


void nap_msec(int msec)
{
    std::this_thread::sleep_for( std::chrono::milliseconds ( msec ));
}

void nap_sec(int sec)
{
    std::this_thread::sleep_for( std::chrono::seconds ( sec ));
}


#include <future>
#include <thread>
#include <chrono>
#include <iostream>

int main1() {
    /* Run some task on new thread. The launch policy std::launch::async
       makes sure that the task is run asynchronously on a new thread. */
    auto myThread = [] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 8;
    };
    auto future = std::async(std::launch::async, myThread);

    // Print status.
    // Use wait_for() with zero milliseconds to check thread status.
    while( future.wait_for(std::chrono::milliseconds(0))  == std::future_status::timeout) {
        std::cout << "    main1 async-future: Thread active" << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "main1 async-future: Thread finished" << "\n";

    auto result = future.get(); // Get result.
    return 0;
}

// If you must use std::thread then you can use std::promise to get a future object:

#include <future>
#include <thread>
#include <chrono>
#include <iostream>

int main2() {
    // Create a promise and get its future.
    std::promise<bool> p;
    auto future = p.get_future();

    // Run some task on a new thread.
    auto myThread = [&p] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        p.set_value(true); // Is done atomically.
    };
    std::thread t(myThread);  // start thread

    // Print status.
    // Use wait_for() with zero milliseconds to check thread status.
    while( future.wait_for(std::chrono::milliseconds(0))  == std::future_status::timeout) {
        std::cout << "    main2 thread-promise-future: Thread active" << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "main2 thread-promise-future: Thread finished" << "\n";

    t.join(); // Join thread.
    return 0;
}

// Both of these examples will output:
// Thread active

// This is of course because the thread status is checked before the task is finished.
// But then again, it might be simpler to just do it like others have already mentioned:

#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

int main3() {
    std::atomic<bool> done(false); // Use an atomic flag.

    /* Run some task on a new thread.
       Make sure to set the done flag to true when finished. */
    auto myThread = [&done] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        done = true;
    };
    std::thread t(myThread);

    // Print status.
    while (!done) {
        std::cout << "   main3 (atomic flag): Thread active" << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "main3 (atomic flag): Thread finished" << "\n";

    t.join(); // Join thread.
    return 0;
}

// There's also the std::packaged_task for use with std::thread for a cleaner solution 
// than using std::promise:

#include <future>
#include <thread>
#include <chrono>
#include <iostream>

int main4() {
    // Create a packaged_task using some task and get its future.
    std::packaged_task<void()> task([] {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    });
    auto future = task.get_future();

    // Run task on new thread.
    std::thread t(std::move(task));

    // Use wait_for() with zero milliseconds to check thread status.
    while( future.wait_for(std::chrono::milliseconds(0))  == std::future_status::timeout) {
        std::cout << "    main4 packaged_task-future: Thread active\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "main4 packaged_task-future: Thread finished\n";


    t.join(); // Join thread.
    return 0;
}

int main()
{
  main1();
  main2();
  main3();
  main4();
}
