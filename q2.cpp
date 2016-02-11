#include <iostream>
using namespace std;

#include<stdlib.h>

class Queue {
public:
  struct Node
  {
        int   Data;
        Node* link;
  } *tail, *head;
  Queue() { head=nullptr; }
  ~Queue() {
        while(head != nullptr) {
            Node *n=head->link;
            delete head;
            head=n;
        }
    }

  int pop();
  void push(int value);
  void display();
};

int Queue::pop()
{
      int ret =-1;
      if(tail!=nullptr) {
             Node *oldTail=tail;
             ret = tail->Data;
             tail = tail->link;
             delete oldTail;
      } else
             cout << "\nQueue Empty\n";
      return ret;
}

void Queue::push(int value)
{
     Node *temp = new Node;
     temp->Data=value;
     if (head == nullptr) {
           head=temp;
           head->link=nullptr;
           tail=head;
     } else {
           head->link=temp;
           head=temp;
           head->link=nullptr;
     }
}

void Queue::display()
{
     if(tail!=nullptr) {
           cout << "\nElements are as:  ";
           for(Node *var=tail; var!=nullptr; var=var->link) {
                cout << "\t" << var->Data;
           }
           cout << "\n";
     } else
           cout << "\nQueue is Empty";
}

int main()
{
     int i=0;
     Queue q;
     cout << " \n1. Push to Queue";
     cout << " \n2. Pop from Queue";
     cout << " \n3. Display Data of Queue";
     cout << " \n4. Exit\n";
     while(1) {
          cout << " \nChoose Option: ";
          cin >> i;
          switch(i) {
                case 1:
                {
                     int value;
                     cout << "\nEnter a value to push into Queue: ";
                     cin >> value;
                     q.push(value);
                     q.display();
                     break;
                }
                case 2:
                {
                     int d = q.pop();
                     cout << "popped " << d << "\n";
                     q.display();
                     break;
                }
                case 3:
                {
                     q.display();
                     break;
                }
                case 4:
                {
                     exit(0);
                }
                default:
                {
                     cout << "\nwrong choice for operation";
                }
          }
     }
}
