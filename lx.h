#pragma once

//////////
// lx.h //
//////////

// Member functions:
//    isEmpty
//    count
//    pushHead
//    popHead
//    pushTail
//    popTail
//    print
template<class T>
class List
{
private:
    class Node {
    public:
        Node   *link;
        T      data;
    Node(Node* l, T d) : link(l), data(d) {}
    } *head, *tail;
public:
    List() {
        head = tail = nullptr;
    }
    ~List() { // delete from head
        while(head != nullptr) {
            Node *node = head->link;
            delete head;
            head = node;
        }
    }
    bool isEmpty() const {
        return head == nullptr;
    }
    size_t count() const {
        size_t ret = 0;
        for(Node *node = head; node; node = node->link)
            ret++;
        return ret;
    }
    void print() const {
        for(Node *node = head; node; node = node->link)
            cout << node->data << endl;
    }
    void walk( void (*f)(T& d) ) const { // call 'f(T& d)' for each data value in list
        cout << "walk:\n";
        for(Node *node = head; node; node = node->link)
            f(node->data);
    }
    void clear() const { // compiles with 'const'
        for(Node *node = head; node; node = node->link)
            delete node;
        head = tail = nullptr;
    }
    void pushHead(const T& d) {
        Node *node = new Node(head, d);
        if(head == nullptr) // empty list: make a one-node list
            tail = node;    // tail points -> new node
        head = node;        // head points -> new node
    }
    T& popHead() {
        T* ret = nullptr;
        if(head) {
            Node *node = head->link;
            ret        = &head->data;
            delete head;
            head = node;
        } else 
            throw "List::popHead - list is empty";
        return *ret;
    }
    bool popHead(T& d) {
        bool ret = false;
        if(head) { d = popHead(); ret = true; }
        return ret;
    }
    void pushTail(const T& d) {
        Node *node = new Node(nullptr, d);
        if(head == nullptr)    // empty list: make a one-node list
            head = node;       // head points -> new node
        else
            tail->link = node; // tail->link points -> new node
        tail = node;           // tail now points -> new node
    }
    T& popTail() {
        T* ret = nullptr;
        if(head) {
	    ret = &tail->data;
            if(head == tail) { // only one node
                ret = &head->data;
                delete head;
                head = tail = nullptr;
            } else { 
                for(Node *node = head ; node; node = node->link) {
                    if(node->link == tail) { // following node is tail?
                        delete tail;
                        node->link = nullptr;
                        tail = node;
                    }
                }
            }
        } else 
            throw "List::popTail - list is empty";
        return *ret;
    }
    bool popTail(T& d) {
        bool ret = false;
        if(head) { d = popTail(); ret = true; }
        return ret;
    }
};
