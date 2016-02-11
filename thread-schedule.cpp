// http://codereview.stackexchange.com/questions/28437/multithreaded-task-scheduler

// This is (supposedly) a multi-threaded scheduler for one-time and/or repeating tasks. The tasks are simple std::function<void()> objects. I built it to be a crucial part of a larger project I'm working on, but I developed it stand-alone, so no context is missing for a review.
// 
// I'm making heavy use of C++11 language and library features (especially thread support and chrono stuff).
// 
// Tasks are supposed to be scheduled by specifying a start time_point, or a delay (converted to a time_point by adding it to now().) An optional duration specifies repeat intervals for the task (if it's non-zero).
// 
// It should be possible to de-schedule tasks, preventing them from being started for execution from then on. (Already running tasks won't be stopped, to keep things a bit simpler, and also because I couldn't figure out a clean way to do it anyway.)
// 
// I've never done anything with multithreading of this scale/complexity, and in case my brain never recovers from repeatedly being torn into 5 or more threads, I'd like to get some review/feedback from others. Specifically, race conditions/deadlocks/other threading-unpleasantness I didn't spot, lifetime issues, or really anything problematic.
// 
// Some simple code at the very bottom demonstrates how it's meant to be used. It seemed to work when compiled with clang 3.3 and libc++.

#include <chrono>
#include <condition_variable>
#include <deque>
#include <list>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <algorithm>   // GB find

namespace scheduling {
    template <class Clock>
    class Scheduler {
        typedef Clock clock_type;
        typedef typename clock_type::time_point time_point;
        typedef typename clock_type::duration duration;
        typedef std::function<void()> task_type;
    private:
        struct Task {
        public:
            Task (task_type&& task, const time_point& start, const duration& repeat) : task(std::move(task)), start(start), repeat(repeat) { }
            task_type task;
            time_point start;
            duration repeat;

            bool operator<(const Task& other) const {
                return start < other.start;
            }
        };
    public:
        typedef typename std::list<Task>::iterator task_handle;
    private:
        std::mutex mutex;
        std::condition_variable tasks_updated;

        std::deque<task_handle> todo;
        std::condition_variable modified;

        bool running;
        std::list<Task> tasks;
        std::list<task_handle> handles;

        std::vector<std::thread> threads;
    public:
        Scheduler() : threads(4) {

        }

        ~Scheduler() {
            halt();
        }

        task_handle schedule(task_type&& task, const time_point& start, const duration& repeat=duration::zero()) {
            task_handle h;
            {
                std::lock_guard<std::mutex> lk(mutex);
                h = tasks.emplace(tasks.end(), std::move(task), start, repeat);
                handles.push_back(h);
            }
            tasks_updated.notify_all();
            return h;
        }

        task_handle schedule(task_type&& task, const duration& delay=duration::zero(), const duration& repeat=duration::zero()) {
            return schedule(std::move(task, clock_type::now()+delay, repeat));
        }

        void unschedule(const task_handle& handle) {
            {
                std::lock_guard<std::mutex> lk(mutex);
                auto handle_it = std::find(handles.begin(), handles.end(), handle);
                if (handle_it != handles.end()) {
                    tasks.erase(handle);
                    todo.remove(handle);
                    handles.erase(handle_it);
                }
            }
            tasks_updated.notify_all();
        }

        void clear() {
            {
                std::lock_guard<std::mutex> lk(mutex);
                tasks.clear();
                handles.clear();
            }
            tasks_updated.notify_all();
        }

        void run() {
            {
                std::lock_guard<std::mutex> lk(mutex);
                if (running) return;
                running = true;
                for (auto& t : threads) {
                    t = std::thread([this]{this->loop();});
                }
            }
            while (true) {
                std::unique_lock<std::mutex> lk(mutex);
                if (!running) break;

                auto task_it = min_element(tasks.begin(), tasks.end());
                time_point next_task = task_it == tasks.end() ? clock_type::time_point::max() : task_it->start;
                if (tasks_updated.wait_until(lk, next_task) == std::cv_status::timeout) {
                    if (task_it->repeat != clock_type::duration::zero()) {
                        task_it->start += task_it->repeat;
                    }
                    else {
                        handles.remove(task_it);
                        tasks.erase(task_it);
                    }
                    todo.push_back(task_it);
                    modified.notify_all();
                }
            }
            for (auto& t : threads) {
                t.join();
            }
        }

        void halt() {
            {
                std::lock_guard<std::mutex> lk(mutex);
                if (!running) return;
                running = false;
            }
            tasks_updated.notify_all();
            modified.notify_all();
        }

    private:
        void loop() {
            while (true) {
                std::function<void()> f;
                {
                    std::unique_lock<std::mutex> lk(mutex);
                    while (todo.empty() && running) {
                        modified.wait(lk);
                    }
                    if (!running) {
                        return;
                    }
                    f = todo.front()->task;
                    todo.pop_front();
                }
                f();
            }
        }
    };
}

#include <iostream>

void outp(const std::string& outp) {
    static std::mutex m;
    std::lock_guard<std::mutex> lk(m);
    std::cout << std::this_thread::get_id() << ": " << outp << "\n";
}

int main(int argc, char* argv[]) {
    scheduling::Scheduler<std::chrono::steady_clock> sched;

    sched.schedule([&sched]{outp("Task 1");}, std::chrono::steady_clock::now());
    sched.schedule([&sched]{outp("Task 2");}, std::chrono::steady_clock::now()+std::chrono::seconds(2), std::chrono::seconds(2));
    sched.schedule([&sched]{outp("Task 3");}, std::chrono::steady_clock::now()+std::chrono::seconds(2), std::chrono::seconds(2));
    sched.schedule([&sched]{outp("Task 4");}, std::chrono::steady_clock::now()+std::chrono::seconds(2), std::chrono::seconds(2));
    sched.schedule([&sched]{outp("Task 5");}, std::chrono::steady_clock::now()+std::chrono::seconds(2), std::chrono::seconds(2));
    sched.schedule([&sched]{outp("Task 6");}, std::chrono::steady_clock::now()+std::chrono::seconds(3));
    sched.schedule([&sched]{outp("Task 7");}, std::chrono::steady_clock::now()+std::chrono::seconds(3));
    sched.schedule([&sched]{outp("Task 8");}, std::chrono::steady_clock::now()+std::chrono::seconds(3));
    sched.schedule([&sched]{outp("Task 9");}, std::chrono::steady_clock::now()+std::chrono::seconds(3));
    sched.schedule([&sched]{outp("Task 10"); sched.halt(); }, std::chrono::steady_clock::now()+std::chrono::seconds(5));

    sched.run();
}
