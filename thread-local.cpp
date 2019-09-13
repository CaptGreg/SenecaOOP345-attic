// http://en.cppreference.com/w/cpp/language/storage_duration

// The thread_local keyword is only allowed for objects declared at 
// namespace scope, objects declared at block scope, and static data 
// members. It indicates that the object has thread storage duration. 
// It can be combined with static or extern to specify internal or 
// external linkage (except for static data members which always have 
// external linkage), respectively, but that additional static doesn't 
// affect the storage duration. 

// thread storage duration. The object is allocated when the thread 
// begins and deallocated when the thread ends. Each thread has its 
// own instance of the object. Only objects declared thread_local 
// have this storage duration. thread_local can appear together with 
// static or extern to adjust linkage. 

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

using namespace std;
 
thread_local unsigned int rage = 1; // one copy per thread
mutex cout_mutex;
 
void increase_rage(const string& thread_name)
{
    ++rage; // modifying outside a lock is okay; this is a thread-local variable
    lock_guard<mutex> lock(cout_mutex);
    cout << "Rage counter for " << thread_name << ": " << rage << '\n';
}
 
int main1()
{
    thread a(increase_rage, "a"), b(increase_rage, "b");
 
    { 
      lock_guard<mutex> lock(cout_mutex);
      cout << "Rage counter for main: " << rage << '\n';
    }
 
    a.join();
    b.join();

    return 0;
}

thread_local int tli = 42; // one copy per thread

int main()
{
  main1();

  cout << "main tli=" << tli << "\n";

  thread([] {
    cout << "A tli="<<tli<<", rage=" << rage << "\n"; 
    tli = 3; 
    cout << "A tli="<<tli<< "\n"; 
  }).join();

  thread([] {
    cout << "B tli="<<tli<<", rage=" << rage << "\n"; 
    tli = 4; 
    cout << "B tli="<<tli<< "\n"; 
  }).join();

  thread([] {
    cout << "C tli="<<tli<<", rage=" << rage << "\n"; 
    tli = 5; 
    cout << "C tli="<<tli<< "\n"; 
  }).join();

  thread([] {
    cout << "D tli="<<tli<<", rage=" << rage << "\n"; 
    tli = 6; 
    cout << "D tli="<<tli<< "\n"; 
  }).join();

  cout << "main tli=" << tli << "\n";

  return 0;
}

