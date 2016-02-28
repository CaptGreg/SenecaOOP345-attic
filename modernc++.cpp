// http://herbsutter.com/elements-of-modern-c-style/

#include <vector>
#include <memory> // unique_ptr,shared_ptr,weak_ptr
#include <thread>
#include <mutex>
#include <utility> // complex

using namespace std;

class widget {
public:
    widget();
    // ... (see GotW #100) ...
    // http://herbsutter.com/gotw/_100/
private:
    class impl;
    unique_ptr<impl> pimpl;
};
 
class widget::impl { /*...*/ };
 
widget::widget() : pimpl{ new impl{ /*...*/ } } { }
// ...'

// C++11
auto pw = make_shared<widget>();

// C++11
class gadget;
 
class widget2 {
private:
    shared_ptr<gadget> g; // if shared ownership
};
 
class gadget {
private:
    weak_ptr<widget2> w;
};

// C++11
class node {
 vector<unique_ptr<node>> children;
 node* parent;
public:
 // :::
};


// C++11
// rectangle       w   { origin(), extents() };
// complex<double> c   { 2.71828, 3.14159 };  // <utility> header file
int             a[] { 1, 2, 3, 4 };
vector<int>     v   { 1, 2, 3, 4 };

// Algorithm: template<typename T> void myLock( T& t, F f ) { lock_guard hold(t); f(); }     // GB does not work
template<typename T,typename F>    void myLock( T& t, F f ) { lock_guard<T> hold(t); f(); }  // GB fixed!
int main(int argc, char**argv)
{
   mutex mut_x;


// C++11 without lambdas: already nice, and more flexible (e.g., can use timeouts, other options)
{
    lock_guard<mutex> hold { mut_x };
    // ... use x ...
}
 
// C++11 with lambdas, and a helper algorithm: C# syntax in C++
// Algorithm: template<typename T> void myLock( T& t, F f ) { lock_guard hold(t); f(); }
myLock( mut_x, [&]{
    // ... use x ...
});
}
