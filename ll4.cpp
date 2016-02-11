#include <string>
#include <iostream>
using namespace std;

class Node  {
public:
  Node *next;
  string   data;
};

class List {
  Node *head;
public:
  List() : head(nullptr) {}

  void push(string d) 
  {
    Node *n = new Node;
    n->data = d;
    n->next = head;
    head = n;
  }
  string pop()
  {
    if(head) {
      string ret = head->data;
      Node *n = head->next;
      delete head;
      head = n;
      return ret;
    } else
      throw string("list is empty --- cannot pop.");
  }

  void print()
  {
    cout << "head:" << head << "\n";
    for(Node *p=head; p; p=p->next){
      cout << "Node:" << p << "\n";
      cout << "next=" << p->next << "\n";
      cout << "data=" << p->data << "\n";
      cout << "\n";
    }
  }
};



int main(int argc, char ** argv)
{
  cout << "argc = " << argc << "\n";
  for(int i = 0; i < argc; i++)
    cout <<"[" << i << "]==>" << argv[i] << "<==\n";

  try {
    List l;

    for(int i = 1; i < argc; i++)
      l.push(string(argv[i]));
  
    l.print();

    while(1) {
      cout << "popped " <<  l.pop() << "\n";
      l.print();
    }

  } catch (string& err) {
    cout << err << "\n";
  }
}


