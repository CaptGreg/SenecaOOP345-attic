// Running a class member function as a thread
// case 1: - run a member function as a thread for a given instance of the class
// case 2: - same as case 1 but use bind
// case 3: - run a member function as a thread from another member function of the same class.

#include <thread>
#include <iostream>
using namespace std;

int NUM_PROCS = thread::hardware_concurrency();

thread::id main_thread_id = this_thread::get_id();

void threaded() { if(main_thread_id != this_thread::get_id()) cout<<"New thread:"; }

int main()
{
  class X {
  public:
    void voidvoid()         { threaded(); cout << "'voidvoid'\n"; }
    void voidint(int i)     { threaded(); cout << "'voidint', i=" << i << "\n"; }
    void voidrefint(int& i) { threaded(); cout << "'voidrefint', i=" << i << "\n"; }

    void callAll() { // no magic, just run the member functions
      cout << "member function callAll start\n";
      int i=456; 
      cout<<"  "; voidvoid  (); 
      cout<<"  "; voidint   (i); 
      cout<<"  "; voidrefint(i); 
      cout << "member function callAll stop\n";
    }

    void threadAll() { // run the member functions as threads
      cout << "\ntheading case 3: - run a member function as a thread from another member function of the same class.\n";
      cout << "member function threadAll start\n";
      int i=789; 
      // Need to specify function pointer and in the case of a class, a pointer to the instance.
      // The instance is 'this'.
      cout<< "  "; thread(&X::voidvoid,   this         ).join();
      cout<< "  "; thread(&X::voidint,    this, i      ).join();
      cout<< "  "; thread(&X::voidrefint, this, ref(i) ).join(); // must inform compiler parm 'i' is call-by-ref.
      cout << "member function threadAll stop\n";
    }
  };

  X x;
  int i = 123;

  cout << "No threading, just have a member function call other member functions\n";
  x.callAll();

  cout << "\ntheading case 1: - run a member function as a thread for a given instance of the class\n";
  cout << "main threading all start\n";
  // Need to specify function pointer and in the case of a class, a pointer to the instance.
  // The instance pointer is '&x'.
  cout<< "  "; thread(&X::voidvoid,&x         ).join();
  cout<< "  "; thread(&X::voidint,&x,i        ).join();
  cout<< "  "; thread(&X::voidrefint,&x,ref(i)).join(); // must inform compiler parm 'i' is call-by-ref
  cout << "main case 1 stop\n";

  cout << "\ntheading case 2: - same thing again but use bind\n";
  cout << "main threading all start\n";
  // Need to specify function pointer and in the case of a class, a pointer to the instance.
  // The instance pointer is '&x'.
  auto b1 = bind( &X::voidvoid,   &x         ); cout<<  "  "; thread( b1 ).join();
  auto b2 = bind( &X::voidint,    &x,     i  ); cout<<  "  "; thread( b2 ).join();
  auto b3 = bind( &X::voidrefint, &x, ref(i) ); cout<<  "  "; thread( b3 ).join();
  cout << "main case 2 stop\n";

  x.threadAll();
}
