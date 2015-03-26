// some form of command queue

// http://stackoverflow.com/questions/21500381/storing-stdpromise-objects-in-a-stdpair

#include <mutex>
#include <condition_variable>
#include <future>
#include <list>
#include <utility>   // GB std::pair
#include <iostream>  // GB cout
#include <thread>    // GB
#include <chrono>    // GB this_thread::sleep_for (std::chrono::milliseconds(50));

template<class COMMAND>
class command_queue
{
private:
    std::mutex                  _queueLock;
    std::condition_variable     _queueCond;
    std::list<COMMAND>          _queue;
    bool                        _queueStarted = false;

public:
    command_queue() = default;
    virtual ~command_queue() noexcept = default;
    command_queue( const command_queue& ) = delete;              // no copy constructor
    command_queue& operator = ( const command_queue& ) = delete; // no assignment operator

    void start()
    { // acquire lock
        std::lock_guard<std::mutex> g( _queueLock );
        _queueStarted = true;
    } // release lock

    bool started()
    {
        return _queueStarted;
    }

    void stop()
    { // acquire lock
        std::lock_guard<std::mutex> g( _queueLock );
        _queueStarted = false;
        _queueCond.notify_one();
    } // release lock

    void post_front( const COMMAND& cmd )
    { // acquire lock
        std::lock_guard<std::mutex> g( _queueLock );
        _queue.push_front( cmd );
        _queueCond.notify_one();
    } // release lock

    void post_front( COMMAND&& cmd )
    { // acquire lock
        std::lock_guard<std::mutex> g( _queueLock );
        _queue.push_front( std::move(cmd) );
        _queueCond.notify_one();
    } // release lock

    void wait()
    { // acquire lock
        std::unique_lock<std::mutex> g( _queueLock );  // cannot use lock_guard, must use unique_lock
        _queueCond.wait( g, 
          [this]{return !_queue.empty() || !_queueStarted;}
        );
    } // release lock

    COMMAND pop_back()
    { // acquire lock
        std::unique_lock<std::mutex> g( _queueLock );
        auto val = std::move(_queue.back());
        _queue.pop_back();
        return val;
    } // release lock

};

template<class COMMAND, class RETURN_VALUE>
class actor
{
protected:
    bool                                                         _actorStarted;
    std::thread                                                  _actorThread;
    command_queue<std::pair<COMMAND,std::promise<RETURN_VALUE>>> _actorQueue;
public:
    actor() : _actorStarted( false ), _actorThread(), _actorQueue() { }
    virtual ~actor() noexcept { if( _actorStarted ) stop(); }
    actor( const actor& ) = delete;                  // no copy constructor
    actor& operator = ( const actor& ) = delete;     // no assignment operator
    void start()
    {
        _actorStarted = true; 
        _actorQueue.start(); 
        _actorThread = std::thread( &actor<COMMAND,RETURN_VALUE>::_entry_point, this );
    }
    void stop()
    {
        _actorStarted = false; 
        _actorQueue.stop(); 
        _actorThread.join();
    }
    std::future<RETURN_VALUE> post( const COMMAND& cmd )
    {
        std::promise<RETURN_VALUE> p;
        std::future<RETURN_VALUE> waiter = p.get_future();

        _actorQueue.post_front( std::pair<COMMAND,std::promise<RETURN_VALUE>>(cmd, std::move(p)) );

        return waiter;
    }
    virtual RETURN_VALUE process( const COMMAND& cmd ) = 0;

protected:
    void _entry_point()
    {
        while( _actorStarted )
        {
            _actorQueue.wait();
            if( !_actorStarted ) continue;
            std::pair<COMMAND,std::promise<RETURN_VALUE>> item = _actorQueue.pop_back();
            item.second.set_value( process( item.first ) );
        }
    }
};

class int_printer : public actor<int,bool>
{
public:
    virtual bool process( const int& cmd ) override
    {
        std::cout << cmd << "\n";
        return true;
    }
};

using namespace std;

int main( int argc, char* argv[] )
{
// GB what do these 3 lines do?
//    std::promise<bool> p;
//    list<std::pair<int,std::promise<bool>>> promises;
//    promises.push_back( make_pair<int,std::promise<bool>>(10,std::move(p)) );

    int_printer a;

    a.start();

    future<bool> result  = a.post( 10 );
    future<bool> result2 = a.post( 20 );

    this_thread::sleep_for (std::chrono::milliseconds(50));

    a.stop(); // returns immediately - does not wait for any active threads to complete.
    return 0;  // When main exits, all thread activity is terminated.
}
