// http://stackoverflow.com/questions/10673585/start-thread-with-member-function

// GB how to start a thread that runs a member function
// GB read the discussion on copying parameters

#include <thread>
#include <iostream>

class bar {
public:
  void foo(int i) {
    std::cout << "hello from member function, i=" << i << "\n";
  }
  void foofoo() {  // GB
    std::thread t(&bar::foo, bar(), 22);
    t.join();
  }
};

int main()
{
  // GB NOTE:
  // parmaeter 1 is the fully qualified name to the member function
  // parameter 2 invokes the class name constructor
  // parameter 3 ... are the arguments

  std::thread t(&bar::foo, bar(), 44);
  t.join();

  bar ofSoap;       // GB
  ofSoap.foofoo();  // GB
}
