// GB not quite sure this works...
// from the print traffic, looks like it works

#include <iostream>
using namespace std;

template<typename T>
class CircularQ {

    class node
    {
    public:
        T           data;
        node        *next;
    } *head, *tail;

    public:
    CircularQ() : head(nullptr), tail(nullptr) {}
    void push(T data)  // push at tail
    {
        node *n=new node;
        n->data=data;
        if(head==nullptr) { // empty
            head=n;
        } else {
            tail->next=n;
        }
        tail=n;
        tail->next=head;
    }

    bool pop(T& data) // pop from head
    {
        if(head!=nullptr) {
            node *n=head;
            data=n->data;
            delete n;
            if(head==tail) { // only 1 node
                 head=nullptr;
                 tail=nullptr;
             } else {
                 head=head->next;
                 tail->next=head;
             }
             return true;
        }
        return false;
    }

    bool peekHead(T& data)
    {
        if(head!=nullptr) {
            data=head->data;
            return true;
        }
        return false;
    }

    bool peekTail(T& data)
    {
        if(tail!=nullptr) {
            data=tail->data;
            return true;
        }
        return false;
    }

    void print()
    {
        node *n=head;
        int count=0;
        while(n) {
            cout << count++ << ": " << n->data << "\n";
            if(n==tail) break;
            n=n->next;
        }
    }
};

int main(int argc, char**argv)
{
    int data;
    CircularQ<int> cirq;
    cout << "empty - new circular q\n";
    cirq.print();

    cout << "pushing " << -99 << "\n";
    cirq.push(-99);
    if(cirq.peekHead(data)) { cout << "head=" << data << "\n"; }
    if(cirq.peekTail(data)) { cout << "tail=" << data << "\n"; }
    cout << "\n";

    for(int i=0; i < 3; i++ ) {
        cout << "pushing " << i << "\n";
        cirq.push(i);
        if(cirq.peekHead(data)) { cout << "head=" << data << "\n"; }
        if(cirq.peekTail(data)) { cout << "tail=" << data << "\n"; }
        cout << "\n";
    }

    cout << "populated\n";
    cirq.print();

    while(cirq.pop(data)) {
        cout << "popped " << data <<"\n";
    }

    cout << "empty - popped everything\n";
    cirq.print();
}
