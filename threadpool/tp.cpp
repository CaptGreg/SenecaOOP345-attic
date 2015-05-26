// http://www.programering.com/a/MDO0QzNwATE.html`


#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <iostream>
#include <chrono>

namespace concurrent {

    namespace futures {

	class ThreadPoolExecutor {
	    typedef std::function<void()>   Function;
	    std::vector<std::thread>        _workers;
	    std::queue<Function>            _tasks;
	    std::mutex                      _cond_mutex;
	    std::condition_variable         _cond_var;
	    std::atomic<bool>               _exit_flag;
	public:
	    ThreadPoolExecutor(int maxworkers): _exit_flag(false)
	    {
		int num;
		if (maxworkers > 0) {
		    num = maxworkers;
		} else {
		    num = std::max(1, (int)std::thread::hardware_concurrency());
		}
		for ( ; num > 0; num--) {
		    _workers.emplace_back(&ThreadPoolExecutor::_run, this);
		}
	    }
	    ~ThreadPoolExecutor()
	    {
		if (!_exit_flag.load()) {
		    stop();
		}
	    }

	    void submit(Function&& f)
	    {
		{ // acquire lock
		    std::lock_guard<std::mutex> lg(_cond_mutex);
		    _tasks.emplace(f);
		}
		_cond_var.notify_all();
	    }

	    void join()
	    {
		for (auto &t: _workers) {
		    t.join();
		}
	    }

	    void stop()
	    {
		_exit_flag.store(true);
		_cond_var.notify_all();
		join();
	    }

	private:
	    void _run()
	    {
		Function job;
		auto wakeup = [&]{ return _exit_flag.load() || !_tasks.empty(); };
		while (!_exit_flag.load()) {
		    { // acquire lock
			std::unique_lock<std::mutex> ul(_cond_mutex);
			_cond_var.wait(ul, wakeup);
			if (_exit_flag.load()) break;
			job = _tasks.front();
			_tasks.pop();
		    }
		    job();
		}
	    }
	}; // class ThreadPoolExecutor

    }; // namespace futures

}; // namespace concurrent

void print(int num, char c)
{
    for (int i = 0; i < num; ++i) {
	std::cerr << c << ' ';
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

using namespace concurrent::futures;

int main(int argc, char* argv[])
{
    std::vector<char> sample{'a', 'b', 'c', 'd', 'e'};
    ThreadPoolExecutor pool(0);

    for (auto v: sample) {
	pool.submit(std::bind(print, 100, v));
    }
    pool.join();

    return 0;
}
