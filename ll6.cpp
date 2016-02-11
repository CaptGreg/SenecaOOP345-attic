#include <string>
#include <iostream>
using namespace std;

template <typename T>
class Node {
public:
  Node<T> *link;
  T   data;
};


template <typename T>
class List {
  Node<T> *head;
public:

  List() : head(nullptr) {}
  void push(T d)
  {
    Node<T> *n = new Node<T>;
    n->data = d;
    n->link = head;
    head = n;
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

  void print()
  {
    for(Node<T> *n=head; n; n=n->link) {
      cout << n->data << "\n";
   }
  }
  void printAddresses()
  {
    cout << "head=" << head << "\n";

    for(Node<T> *n=head; n; n=n->link) {
      cout << "NODE:" << n << "\n";
      cout << "  link=" << &n->link << "H " << n->link << "\n";
      cout << "  data=" << &n->data << "H " << n->data << "\n";
   }
  }
};



int main(int argc, char **argv)
{
   cout <<"argc=" << argc << "\n";
   for(int i = 0; i < argc; i++)
      cout << "argv[" << i << "]==>" << argv[i] << "<==\n";

   try {
     List<string> ll;

     ll.printAddresses();

     for(int i = 1; i < argc; i++) {
        ll.push( argv[i] );
        ll.printAddresses();
     }

     ll.print();

     while(1) {
       cout << "popped: " << ll.pop() << "\n";
     }
   } catch(string& msg) {
      cout << msg << "\n";
   }
}
