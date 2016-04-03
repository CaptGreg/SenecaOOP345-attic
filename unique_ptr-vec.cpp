// g++ (Ubuntu 5.2.1-22ubuntu2) 5.2.1 20151010

#include <iostream>
#include <queue>
#include <memory>
#include <vector>
#include <utility>

using namespace std;

class X {
public:
  int time;

  X()      :time(0)  { cout << "X()\n"; }
  X(int i) :time(i)  { cout << "X("<<i<<")\n"; }
  ~X() { cout << "~X() i=" << time << "\n"; }
  ostream& operator<< (ostream& os) const { os << "operator<< i=" << time << "\n"; return os; }
  void print() const { cout << "print() i=" << time << "\n"; }
};

int main()
{
  {
    cout << "\nqueue<unique_ptr<int>> q;\n";
    // create queue
    queue<unique_ptr<int>> q;
   
    // add element
    unique_ptr<int> p (new int{123});
    q.push(move(p));
   
    // try to grab the element
    auto p2 = move(q.front());
    q.pop();
    cout << *p2 << "\n";
  }

  //////////////////////////////////////////////
  {
    // this works
    cout << "\nqueue<unique_ptr<X>> q;\n";
    queue<unique_ptr<X>> q;

    q.push(move( unique_ptr<X> (new X(123)) ) );
    q.push(move( unique_ptr<X> (new X(99)) ) );

    unique_ptr<X> up (new X(1234));
    q.push(move(up));
    up = move(q.front());
    q.pop();

    up -> print ();
    up -> operator<< (cout);

    q.push(move( unique_ptr<X> (new X(99)) ) );
    up = move(q.front());

    up -> print ();
    up -> operator<< (cout);
  }

  //////////////////////////////////////////////
  {

    cout << "\npriority_queue<X, vector<unique_ptr<X>> > q;\n";

    class CompareX {
    public: 
      bool operator()(const X& l, const X& r) const { return l.time > r.time; }
    };

    class CompareXup {
    public: 
      bool operator()(const unique_ptr<X> l, const unique_ptr<X> r) const { return l->time > r->time; }
    };
    auto lambdaUp = [] (const unique_ptr<X> l, const unique_ptr<X> r) { return l->time > r->time; };
    auto lambdaX = [] (const X& l, const X& r) { return l.time > r.time; };

    // priority_queue<unique_ptr<X>, vector<unique_ptr<X>> > q;             // WORKS!, without compare function
    // priority_queue<unique_ptr<X>, vector<unique_ptr<X>>, CompareX > q;   // ERROR!
    // priority_queue<unique_ptr<X>, vector<unique_ptr<X>>, CompareXup > q; // ERROR!
    // priority_queue<unique_ptr<X>, vector<unique_ptr<X>>, decltype(lambdaUp) > q; // ERROR!

    priority_queue<X, vector<unique_ptr<X>> > q;                            // WORKS!, without compare function
    // priority_queue<X, vector<unique_ptr<X>>, CompareX > q;               // ERROR!
    // priority_queue<X, vector<unique_ptr<X>>, CompareXup > q;             // ERROR!
    // priority_queue<X, vector<unique_ptr<X>>, decltype(lambdaUp) > q;     // ERROR!

    q.push(move( unique_ptr<X> (new X(123)) ) );
    q.top() -> print();   // works

    q.push(move( unique_ptr<X> (new X(99)) ) );
    q.top() -> print();   // works

    unique_ptr<X> up (new X(456));
    q.push(move(up));
    q.top() -> print();   // works

    q.top() -> operator<< (cout);

    // auto p2 = move(q.top());       // FAILS!  top() is const.  Cannot move(const)
    // auto p2 = q.top().release();   // FAILS!

    q.pop();
  }
  {
    cout << "\npriority_queue pair, container, and compare function\n";
    // http://en.cppreference.com/w/cpp/container/priority_queue/priority_queue
    using my_pair_t = std::pair<size_t,bool>;
 
    using my_container_t = std::vector<my_pair_t>;
 
    auto my_comp =
        [](const my_pair_t& e1, const my_pair_t& e2) 
        { return e1.first > e2.first; };
    std::priority_queue<my_pair_t,
                        my_container_t,
                        decltype(my_comp)> queue(my_comp);

    queue.push(std::make_pair(5, true));
    queue.push(std::make_pair(3, false));
    queue.push(std::make_pair(7, true));

    std::cout << std::boolalpha; // print bool as false/true not 0/1
    while(!queue.empty()) {
        const auto& p = queue.top();
        std::cout << p.first << " " << p.second << "\n";
        queue.pop();
    }
  }
  {
    cout << "\nclass X hack priority_queue pair, container, and compare function\n";
    // http://en.cppreference.com/w/cpp/container/priority_queue/priority_queue
    using my_pair_t = X;
 
    using my_container_t = std::vector<my_pair_t>;
 
    auto my_comp =
        [](const my_pair_t& e1, const my_pair_t& e2) 
        { return e1.time > e2.time; };
    std::priority_queue<  my_pair_t,
                          my_container_t,
                          decltype(my_comp)
                       > queue(my_comp);       // NOTE my_comp is used twice

    cout << "queue.push(X(5));\n";
    queue.push(X(5));

    cout << "queue.push(X(3));\n";
    queue.push(X(3));

    queue.push(X(7));
    cout << "queue.push(X(7));\n";

    while(!queue.empty()) {
        const auto& p = queue.top();
        p.print();
        queue.top().print();
        queue.pop();
    }
  }
} // end main
