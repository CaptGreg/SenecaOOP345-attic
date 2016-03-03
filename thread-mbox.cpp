// http://stackoverflow.com/questions/16872593/c-c-implementation-of-a-mailbox-for-inter-thread-communication

// GB convert pthreads to C++ threads
// GB convert int try_XXX functions to bool
// GB convert messageT *msg to messageT&

#include <iostream>
#include <sstream>    // trim thread id
#include <string>     // GB expand Mailbox template with a string
#include <chrono>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

template <class messageT>
class Mailbox {

    private:
        std::condition_variable  msg_available_cv;  // Message in the mailbox?
        std::mutex               queue_mutex;       // Mutex for queue control

        std::queue<messageT>     msgQ;              // Messages

    public:
        Mailbox(void)  { }
        ~Mailbox(void) { }

        void put(messageT msg) { // Put a single message into the mailbox

            std::unique_lock<std::mutex> lock(queue_mutex);

            // Push message into mailbox
            msgQ.push(msg);

            // Signal there is a message in the mailbox
            msg_available_cv.notify_one();
        }

        bool try_put(messageT msg) { // Try to put a single message into the mailbox

            if( queue_mutex.try_lock() ) {

                // Push message into mailbox
                msgQ.push(msg);

                // Signal there is a message in the mailbox
                msg_available_cv.notify_one();

                // Unlock queue
                queue_mutex.unlock();

                return true;
            }
            // Otherwise, say mailbox is unavailable
            else
                return false;
        }

        void get(messageT& msg) { //  Get single message from a mailbox

            std::unique_lock<std::mutex> lock(queue_mutex);

            // Wait for a message to come in
            while(msgQ.empty()) {
                msg_available_cv.wait(lock);
            }

            // Pop off oldest message
            msg = msgQ.front();
            msgQ.pop();
        }

        bool try_get(messageT& msg) { //  Try to get single message from a mailbox

            int mailbox_ready = true;  // Mailbox ready

            std::unique_lock<std::mutex> lock(queue_mutex);

            // Indicate if mailbox is empty
            if(msgQ.empty())
                mailbox_ready = false;
            // Otherwise, grab the message
            else {
                // Pop off oldest message
                msg = msgQ.front();
                msgQ.pop();
            }

            return mailbox_ready;
        }

        void peek(messageT& msg) { //  Peek at single message from a mailbox

            std::unique_lock<std::mutex> lock(queue_mutex);

            // Wait for a message to come in
            while(msgQ.empty()) {
                msg_available_cv.wait(lock);
                // GB was
                // msg_available_cv.wait(queue_mutex);
                // error: no matching function for call to ‘std::condition_variable::wait(std::mutex&)’
                // msg_available_cv.wait(queue_mutex);
                //                                  ^

            }

            // Peek at most recent message
            msg = msgQ.front();
        }

        bool try_peek(messageT& msg) { //  Try to peek at single message from a mailbox

            bool mailbox_ready = true;  // Mailbox ready

            std::unique_lock<std::mutex> lock(queue_mutex);

            if(msgQ.empty())    // Indicate if mailbox is empty
                mailbox_ready = false;
            else                    // Otherwise, grab the message
                msg = msgQ.front();

            return mailbox_ready;
        }

        bool empty() { //  message available? 

            std::unique_lock<std::mutex> lock(queue_mutex);

            return msgQ.empty();
        }

        int count() { //  message count

            std::unique_lock<std::mutex> lock(queue_mutex);

            return msgQ.size();
        }
};

// trim thread id
string  formatTid( std::thread::id tid ) { stringstream ss; ss << tid; string s=ss.str(); return s.substr(s.size()-4,4); }
string  formatTid( ) { return formatTid( this_thread::get_id() ); }

int main(int argc, char**argv)
{
  std::mutex      cout_mutex;       // Mutex for 'cout'
  Mailbox<string> mbox;

  auto get     = [&mbox,&cout_mutex] () -> void { 
       string Msg; 
       bool waited = false;

       // std::chrono::time_point<std::chrono::high_resolution_clock> start;
       auto start = std::chrono::high_resolution_clock::now();

       if(mbox.empty()) {
         waited = true;
         { std::unique_lock<std::mutex> lock(cout_mutex);
           // std::cout<< "get: mailbox empty - waiting for message: " << this_thread::get_id() << "\n"; 
           std::cout<< "get: mailbox empty - waiting for message: " << formatTid() << "\n"; 
         }
       }
       mbox.get(Msg); 

       // std::chrono::time_point<std::chrono::high_resolution_clock> stop;
       auto stop  = std::chrono::high_resolution_clock::now();
       typedef std::chrono::duration<int,std::milli> millisecs_t ;
       millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
       long tWait = duration_get.count();

       { std::unique_lock<std::mutex> lock(cout_mutex);
         // std::cout<< "get: " << tWait << " msec wait time " << (waited? "message arrived ": "")  << this_thread::get_id() << " " << Msg << "\n"; 
         std::cout<< "get: " << tWait << " msec wait time " << (waited? "message arrived ": "")  << formatTid() << " " << Msg << "\n"; 
       }
  };

  auto try_get = [&mbox,&cout_mutex] () -> void { 
       string Msg; 
       std::unique_lock<std::mutex> lock(cout_mutex);
       // std::cout<< "try_get: " << this_thread::get_id() << " " << (mbox.try_get(Msg)? Msg: "mail box empty") << "\n"; 
       std::cout<< "try_get: " << formatTid() << " " << (mbox.try_get(Msg)? Msg: "mail box empty") << "\n"; 
  };

  mbox.put(string("msg 1"));  // Put a single message into the mailbox

  string Msg;
  // mbox.get(Msg); std::cout << Msg << "\n";
  try_get();

  mbox.put(string("msg 2"));  // Put a single message into the mailbox
  mbox.put(string("msg 3"));  // Put a single message into the mailbox
  mbox.put(string("msg 4"));  // Put a single message into the mailbox
  mbox.put(string("msg 5"));  // Put a single message into the mailbox
        // void put(messageT msg); // Put a single message into the mailbox
        // bool try_put(messageT msg); // Try to put a single message into the mailbox
        // void get(messageT& msg); //  Get single message from a mailbox
        // bool try_get(messageT& msg); //  Try to get single message from a mailbox
        // void peek(messageT& msg); //  Peek at single message from a mailbox
        // bool try_peek(messageT& msg); //  Try to peek at single message from a mailbox


  std::thread t1 (get);
  std::thread t2 (try_get);
  std::thread t3 (try_get);
  t1.join();
  t2.join();
  t3.join();
  try_get();
  try_get();
  try_get();
  try_get();

  std::thread t4 (get);
  this_thread::sleep_for (std::chrono::milliseconds(750));
  mbox.put(string("msg 6"));  // Put a single message into the mailbox
  t4.join();
}
