// https://code.msdn.microsoft.com/vstudio/Producer-Consumer-inter-cd30193b#contento
//-----------------------------------------------------------------------------
// File: Program.cpp
//
// Desc: Demonstrates inter-thread communication using condition variables.
//       Producer and consumer threads will ping-pong between each other.
//       Code can be modified in such a way that producer will read and queue 
//         chunks of data from some source, network stream for example, while
//         consumer thread will process data from the queue. In that scenario,
//         ping-ponging will not be required as consumer should be disposing of data
//         independently of producer queuing it.
//
//-----------------------------------------------------------------------------

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using std::cout;
using std::endl;
using std::condition_variable;
using std::lock_guard;
using std::unique_lock;
using std::mutex;
using std::queue;
using std::thread;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;

static const int MagicNumber = 30;                            // Magic number used for the sample, remove it for production code

int main()
{
    condition_variable  m_alarm;                              // Notifies threads that more work is available
    mutex               m_mutex;                              // Synchronizes access to shared variables
    queue<int>          m_queue;                              // Accumulates data chunks
    bool                m_isNotified = false;                 // This is a guard to prevent accidental spurious wakeups
    bool                m_haveData   = true;                  // Only used for this sample to end consumer thread, not required in production code

    thread producer([&m_mutex, &m_queue, &m_alarm, &m_isNotified, &m_haveData]() {
        for (int i = 0; i < MagicNumber; ++i)
        {
            sleep_for(milliseconds(500));                    // Executing some long operation
            lock_guard<mutex> lock(m_mutex);                 // Enter critical section
            cout << "producer " << i << "\n";
            m_queue.push(i);                                 // Add data chunk to the queue
            m_isNotified = true;                             // Consumer can be woken up and it is not a fluke (see spurious wakeups)
            m_alarm.notify_one();                            // Notify consumer
        }

        lock_guard<mutex> lock(m_mutex);                     // Work is done, app can exit
        m_isNotified = true;
        m_haveData = false;
        m_alarm.notify_one();
    });

    thread consumer([&m_mutex, &m_queue, &m_alarm, &m_isNotified, &m_haveData]() {
        while (m_haveData)                                    // In production, this check will be done on whether there is more data in the queue
        {
            unique_lock<mutex> lock(m_mutex);                 // Must aquire unique_lock with condition variables

            while (!m_isNotified)                             // Prevents from spurious wakeup
            {
                m_alarm.wait(lock);                           // Wait for a signal from producer thread
            }

            while (!m_queue.empty())                          // Process data and remove it from the queue
            {
                cout << "consumer " << m_queue.front() << "\n";
                m_queue.pop();
            }

            m_isNotified = false;                            // Protect from spurious wakeup
        }
    });

    // Join threads and finish app
    producer.join();
    consumer.join();

    return 0;
}
