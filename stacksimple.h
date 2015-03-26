#pragma once

///////////////////
// stacksimple.h //
///////////////////

template <class T>
class Stack {
protected:
    class Node {
    public:
        Node   *link;
        T      data;
        Node(T d,Node*l=nullptr) { data=d; link=l; }
    } *head;
public:
    Stack() {
        head = nullptr;
    }
    ~Stack() { // delete from head
        while(head != nullptr) {
            Node *node = head->link;
            delete head;
            head = node;
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
    void pushHead(const T& d) {
        head = new Node(d,head);
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
    void push(const T& d) {
        pushHead(d);
    }
    T pop() {
        return popHead();
    }
};
