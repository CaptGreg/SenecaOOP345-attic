#include <string>     // throw message
#include <iostream>
#include <functional> // std::function + std::bind
#include <cmath>      // sin + cos + M_PI
using namespace std;

template <typename T>
class Node {
public:
  Node<T> *link;
  T   data;
  Node(Node<T>* l, T d) : link(l), data(d) {}
};


template <typename T>
class List {
  Node<T> *head;
public:

  List() : head(nullptr) {}
  void push(T d)
  {
    head = new Node<T> (head,d);
  }
  T pop()
  {
    if(head) {
      T ret = head->data;
      Node<T> *n = head->link;
      delete head;
      head = n;
      return ret;
    } else
      throw string("list is empty --- cannot pop");
  }
};



int main(int argc, char **argv)
{
   try {
     List< std::function<double(void)> > ll;
     
     cout << "pushing lambda + 5 sin values, arg= 0,30,45,60,90\n";
     ll.push(  [] () -> double {
       cout << "pushing std::bind(sin,arg), arg = 0,30,45,60,90\n";
       cout << "NOTE: the following '12345' is the valued returned by this lambda function\n";
       return 12345.;
     } );
     extern double sin(double arg);  // compiler confused.  FIX: prototype it!
     ll.push( std::bind(sin,  0. * M_PI/180.) );
     ll.push( std::bind(sin, 30. * M_PI/180.) );
     ll.push( std::bind(sin, 45. * M_PI/180.) );
     ll.push( std::bind(sin, 60. * M_PI/180.) );
     ll.push( std::bind(sin, 90. * M_PI/180.) );

     cout << "pushing lambda + 5 cos values, arg= 0,30,45,60,90\n";
     ll.push(  [] () -> double {
       cout << "pushing std::bind(cos,arg), arg = 0,30,45,60,90\n";
       cout << "NOTE: the following '12345' is the valued returned by this lambda function\n";
       return 12345.;
     } );
     extern double cos(double arg);  // compiler confused.  FIX: prototype it!
     ll.push( std::bind(cos,  0. * M_PI/180.) );
     ll.push( std::bind(cos, 30. * M_PI/180.) );
     ll.push( std::bind(cos, 45. * M_PI/180.) );
     ll.push( std::bind(cos, 60. * M_PI/180.) );
     ll.push( std::bind(cos, 90. * M_PI/180.) );

     cout << "popping function off stack and printing function call\n";
     while(1) {
       // two lines, for clarity
       std::function<double(void)> f = ll.pop();  // might throw
       cout << f() << "\n";
       // cout << (ll.pop()) () << "\n"; // one line, not quite the same because of the "cout <<" THROW
     }
   } catch(string& msg) {
      cout << msg << "\n";
   }
}
