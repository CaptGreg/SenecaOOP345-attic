// GB hacked from
// http://oopscenities.net/2012/02/24/c11-stdfunction-and-stdbind/
// http://oopscenities.net/2011/07/31/c0x-c-style-listener-list/

// Threading code entirely GB

#include <functional>  // std::function<...>, std::bind
#include <vector>
#include <iostream>
#include <thread>
#include <string>
#include <exception>
using namespace std;
 
typedef function<void()> Job; // jobs are void functions without arguments

int main1()
{
  cout << "\nmain1\n";
  class JobManager {
    private:
      vector<Job> jobs;   
    public:
      void addJob(Job job)
      {
        jobs.push_back(job);
      }
   
      void runJobs() const
      { // Run jobs sequentially, one after another, one at a time
        for(auto job : jobs) 
          job();
      }
   
      void runJobsThreaded(size_t maxThreads) const
      {
        if(jobs.size() <= maxThreads) { // More thread capacity than jobs.

           // Have enough capacity to run all the jobs at once
           thread t[jobs.size()];
           for(unsigned i = 0; i < jobs.size(); i++) // run everything at once
             t[i] = thread( jobs[i] );
           for(auto& e : t) e.join();

        } else {

          thread t[maxThreads];
          size_t i;

          // start up a batch of jobs
          for(i = 0; i < maxThreads; i++) {
            t[i] = thread(jobs[i]);
          }

          // wait for the oldest job to finish, start up another job
          for( size_t oldestThread = 0; i < jobs.size() ;  i++, oldestThread++) {
            if(oldestThread == maxThreads) oldestThread = 0;

            t[oldestThread].join();            // wait for oldest job to finish
            t[oldestThread] = thread(jobs[i]); // start new job
          }

          // wait for last batch of jobs to finish
          for(auto& e : t) e.join();
        }
      }
  };

  JobManager jm;

  struct MFO { void operator() (int arg) {cout << string("job ") + to_string(arg) + "\n"; } } mfo;

  jm.addJob( [] { cout << "job 0\n"; } );
  jm.addJob( [] { cout << "job 1\n"; } );
  jm.addJob( [] { cout << "job 2\n"; } );
  jm.addJob( bind( mfo, 3 ) );
  jm.addJob( bind( mfo, 4 ) );
  jm.addJob( bind( mfo, 5 ) );
  jm.addJob( bind( [] { cout << "job 6\n"; }) );
  jm.addJob( bind( [] { cout << "job 7\n"; }) );
  jm.addJob( bind( [] { cout << "job 8\n"; }) );
  jm.addJob( bind( [] (int arg) { cout << string("job ") + to_string(arg) + "\n"; }, 9 ) );
  jm.addJob( bind( [] (int arg) { cout << string("job ") + to_string(arg) + "\n"; }, 10 ) );
  jm.addJob( bind( [] (int arg) { cout << string("job ") + to_string(arg) + "\n"; }, 11 ) );

  // jm.runJobs();

  // jm.runJobsThreaded(4);
  jm.runJobsThreaded( std::thread::hardware_concurrency() );

  return 0;
}

/////////////////////////////////////////////////////////////////////////
int main2()
{
  cout << "\nmain2\n";
  class JobManager {
    private:
      vector<Job> jobs;   
    public:
      void addJob(Job job)
      {
        jobs.push_back(job);
      }
   
      void runJobs() const
      { // Run jobs sequentially, one after another, one at a time
        for(auto job : jobs) 
          job();
      }
   
      void runJobsThreaded(size_t maxThreads) const
      {
        if(jobs.size() <= maxThreads) { // More thread capacity than jobs.

           // Have enough thread capacity to run all the jobs at once
           thread t[jobs.size()];
           for(unsigned i = 0; i < jobs.size(); i++) // run everything at once
             t[i] = thread(jobs[i]);
           for(auto& e : t) e.join();

        } else {

          vector<thread> t; // (maxThreads);
          size_t i;

          // start up a batch of jobs
          for(i = 0; i < maxThreads; i++) {
            t.push_back( thread(jobs[i]) );
          }

          // run remaining jobs one at a time as each of the oldest jobs finishes
          for( auto oldest = t.begin(); i < jobs.size() ; i++, oldest++ ) {
            if(oldest == t.end()) oldest = t.begin();

            oldest->join(); // wait for the oldest job to finish
            *oldest = thread(jobs[i]); // start new job
          }

          for(auto& e : t) e.join(); // wait for last batch of jobs to finish
        }
      }
  };

  JobManager jm;

  struct MFO { void operator() (int arg) {cout << string("job ") + to_string(arg) + "\n"; } } mfo;

  auto msSleep = [] (unsigned milliSeconds) { this_thread::sleep_for( std::chrono::milliseconds(milliSeconds) ); };
  auto nap   = [msSleep] { msSleep(rand() % 1000); };


  jm.addJob( [nap] {nap(); cout << "job 0\n"; } );
  jm.addJob( [nap] {nap(); cout << "job 1\n"; } );
  jm.addJob( [nap] {nap(); cout << "job 2\n"; } );
  jm.addJob( bind( mfo, 3 ) );
  jm.addJob( bind( mfo, 4 ) );
  jm.addJob( bind( mfo, 5 ) );
  jm.addJob( bind( [nap] {nap(); cout << "job 6\n"; }) );
  jm.addJob( bind( [nap] {nap(); cout << "job 7\n"; }) );
  jm.addJob( bind( [nap] {nap(); cout << "job 8\n"; }) );
  jm.addJob( bind( [nap] (int arg) {nap(); cout << string("job ") + to_string(arg) + "\n"; }, 9 ) );
  jm.addJob( bind( [nap] (int arg) {nap(); cout << string("job ") + to_string(arg) + "\n"; }, 10 ) );
  jm.addJob( bind( [nap] (int arg) {nap(); cout << string("job ") + to_string(arg) + "\n"; }, 11 ) );

  // jm.runJobs();

  // jm.runJobsThreaded(4);
  jm.runJobsThreaded( std::thread::hardware_concurrency() );

  return 0;
}

/////////////////////////////////////////////////////////////////////////



#include <functional>
#include <iostream>
#include <string>
#include <vector>
 
using namespace std;
 
void execute(const vector<function<void ()>>& fs) {
    for (auto& f : fs)
        f();
}
 
void plain_old_func() { cout << "I'm an old plain function" << "\n"; }
 
int main3()
{
    cout << "\nmain3\n";
    struct function_object {
        void operator()() const { cout << "I'm a function object\n"; }
    };

    vector<function<void ()>> x;
    x.push_back(plain_old_func);
     
    function_object function_object_instance;
    x.push_back(function_object_instance);

    x.push_back( [] { cout << "HI, I'm a lambda expression" << "\n"; } );
     
    execute(x);
    return 0;
}

/////////////////////////////////////////////////////////////////////////

int main()
{
  atexit( [] {cout << "atexit callback handler: main over\n"; });
  try {
    main1();
    main2();
    main3();
  } catch(std::exception& e) { 
    cerr << "main threw: " << e.what() << "\n";
 }
    return 0;
}
