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
 
typedef std::function<void ()> Job;

int main1()
{
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
   
      void runJobsThreaded(size_t threadsActive) const
      {
        if(jobs.size() <= threadsActive) { // More threads available than jobs.
           thread t[jobs.size()];
           for(unsigned i = 0; i < jobs.size(); i++)
             t[i] = thread(jobs[i]);
           for(auto& e : t) e.join();

        } else {

          thread t[threadsActive];
          size_t i;
          for(i = 0; i < threadsActive; i++) { // start batch
            t[i] = thread(jobs[i]);
          }

          // as a job finishes, start up another job
          while(i < jobs.size() ) {
            int oldestThread = (i - threadsActive) % threadsActive;

            t[oldestThread].join();            // wait for oldest job to finish
            t[oldestThread] = thread(jobs[i]); // start new job
            i++;
          }

          // wait for last batch of jobs to finish
          for(i = jobs.size() - threadsActive; i < jobs.size(); i++) {
            t[i % threadsActive].join();
          }
 
        }
      }
  };

  JobManager jm;

  struct MFO { void operator() (int arg) {cout << string("job ") + to_string(arg) + "\n"; } };
  MFO mfo;

  jm.addJob( [] {cout << "job 0\n"; } );
  jm.addJob( [] {cout << "job 1\n"; } );
  jm.addJob( [] {cout << "job 2\n"; } );
  jm.addJob( bind( mfo, 3 ) );
  jm.addJob( bind( mfo, 4 ) );
  jm.addJob( bind( mfo, 5 ) );
  jm.addJob( bind( [] {cout << "job 6\n"; }) );
  jm.addJob( bind( [] {cout << "job 7\n"; }) );
  jm.addJob( bind( [] {cout << "job 8\n"; }) );
  jm.addJob( bind( [] (int arg) {cout << string("job ") + to_string(arg) + "\n"; }, 9 ) );
  jm.addJob( bind( [] (int arg) {cout << string("job ") + to_string(arg) + "\n"; }, 10 ) );
  jm.addJob( bind( [] (int arg) {cout << string("job ") + to_string(arg) + "\n"; }, 11 ) );

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
 
void plain_old_func() { cout << "I'm an old plain function" << endl; }
 
int main2()
{
    struct function_object {
        void operator()() const { cout << "I'm a function object\n"; }
    };

    vector<function<void ()>> x;
    x.push_back(plain_old_func);
     
    function_object function_object_instance;
    x.push_back(function_object_instance);

    x.push_back( [] { cout << "HI, I'm a lambda expression" << endl; } );
     
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
  } catch(std::exception& e) { 
    cerr << "main threw: " << e.what() << "\n";
 }
    return 0;
}
