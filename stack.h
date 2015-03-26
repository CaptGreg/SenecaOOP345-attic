#pragma once

/////////////
// stack.h //
/////////////

template <class T>
class Stack {
protected:
    class Node {
    public:
        Node   *link;
        T      data;
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
    void pushHead(const T& d) {
        Node *node = new Node;
        node->data = d;
        node->link = head;
        head = node;
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


// do the same thing (almost) with a C MACRO
// we need two parameters
//  1.  the name of the class to generate
//      -  afects class name, constructor, destructor
//  2.  the data type "T"
//
//  we cannot use c++ // comment in the macro
//  A c++ comment comments out the \ at the end of the line
//  Need to convert
//      ... // comment \ 
//  to
//      ... /* comment */ \ 

#define MACROSTACK(NAME,T) \
class NAME { \
protected: \
    class Node { \
    public: \
        Node   *link; \
        T      data; \
    } *head; \
public: \
    NAME() { \
        head = nullptr; \
    } \
    ~NAME() { /* delete from head */ \
        while(head != nullptr) { \
            Node *node = head->link; \
            delete head; \
            head = node; \
        } \
    } \
    bool isEmpty() const { \
        return head == nullptr; \
    } \
    unsigned count() const { \
        unsigned ret = 0; \
        for(Node *node=head; node; node=node->link) \
            ret++; \
        return ret; \
    } \
    void print() const { \
        for(Node *node=head; node; node=node->link) \
            cout << node->data << endl; \
    } \
    void pushHead(const T& d) { \
        Node *node = new Node; \
        node->data = d; \
        node->link = head; \
        head = node; \
    } \
    T popHead() { \
        T ret; \
        if(!isEmpty()) { \
            Node *node = head->link; \
            ret        = head->data; \
            delete head; \
            head = node; \
        } \
        return ret; \
    } \
    void push(const T& d) { \
        pushHead(d); \
    } \
    T pop() { \
        return popHead(); \
    } \
};
