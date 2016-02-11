// Lists - Doubly linked list, page 132 of OOP344 text (C++ course prior to C++11 course)
// GB - syntactical sugar changes
// GB - added count
// GB - added goPrevious

#include <iostream>
using namespace std;


class Data {
    int data;
public:
    Data(int d = 0) : data(d) {}
    int out() const { return data; }
};

struct Node {
    Data    data;
    Node    *next;
    Node    *previous;
    Node(const Data& d, Node* n, Node *p) : data(d), next(n), previous(p) {}
};

class DLList {
    Node    *head;
    Node    *tail;
    Node    *current;
    int     nodeCount;
public:
    DLList() : head(NULL), tail(NULL), current(NULL), nodeCount(0) {}
    ~DLList() {
        while(current=head) {
            head=head->next; 
            delete current;
        }
    }
    int count() const { return nodeCount; }
    void goHead() {
        current = head;
    }
    void goPrevious() {
        if(current) current = current->previous;
    }
    void goNext() {
        if(current) current = current->next;
    }
    void remove() {
        if(current) {
            nodeCount--;
            Node *node = current;
            if(current == head) {
                head = head->next;
            } else {
                current->next->previous = current->previous;
                current->previous->next = current->next;
                current = current->next;
            }
            delete node;
        }
    }
    bool end() const { return !current; }
    Data get() const { return current ? current->data: Data(); }
    void insertBefore(const Data& a) {
        if(current) {
            Node *node = new (nothrow) Node(a, current, current->previous);
            if(node) {
                if(current->previous)
                    current->previous->next = node;
                else
                    head = node;
                current->previous = node;
            }
        } else {
            Node *node = new (nothrow) Node(a, NULL, NULL);
            if(node) {
                head = tail = current = node;
            }
        }
        nodeCount++;
    }
    void insertAfter(const Data& a) {
        if(current) {
            Node *node = new (nothrow) Node(a, current->next, current);
            if(node) {
                if(current->next)
                    current->next->previous = node;
                else
                    tail = node;
                current->next = node;
            }
        } else {
            Node *node = new (nothrow) Node(a, NULL, NULL);
            if(node) {
                head = tail = current = node;
            }
        }
        nodeCount++;
    }
};

int main(int argc, char**argv)
{
    DLList r;

    // insert Data
    r.insertAfter(3);
    r.insertAfter(5);
    r.goHead();
    r.insertAfter(9);
    r.goNext();
    r.insertAfter(8);

    // Display Data
    cout << r.count() << " nodes in list\n";
    r.goHead();
    while(!r.end()) {
        cout << r.get().out() << ' ';
        r.goNext();
    }
    cout << "\n";
}
