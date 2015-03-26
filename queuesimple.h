#pragma once

///////////////////
// queuesimple.h //
///////////////////

template<class T>
class Queue
{
private:
    class Node {
    public:
        Node   *link;
        T      data;
        Node(T d,Node*l=nullptr) { data=d; link=l; }
    } *head, *tail;
public:
    Queue() {
        head=tail=nullptr;
    }
    ~Queue() { // delete from head
        while(head != nullptr) {
            Node *node=head->link;
            delete head;
            head=node;
        }
    }
    bool isEmpty() const {
        return head == nullptr;
    }
    unsigned count() const {
        unsigned ret = 0;
        for(Node *node=head; node; node=node->link)
            ret++;
        return ret;
    }
    void print() const {
        for(Node *node=head; node; node=node->link)
            cout << node->data << endl;
    }
    void walk(void (*f)(T d)) const {
        for(Node *node=head; node; node=node->link)
            f(node->data);
    }
    T popHead() {
        T ret;
        if(!isEmpty()) {
            Node *node = head->link;
            ret        = head->data;
            delete head;
            head = node;
        }
        return ret;
    }
    void pushTail(const T& d) {
        Node *node=new Node(d,nullptr);
        if(head!=nullptr)
            tail->link=node;
        else
            head=node;
        tail=node;
    }
    void push(const T& d) {
        pushTail(d);
    }
    T pop() {
        return popHead();
    }
};
