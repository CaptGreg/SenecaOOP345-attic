#if 0
XOR, ^, is a very interesting, both bit-wise and logical, operator.  The basic rules are:


1.    0 ^ 0 = 0
2.    A ^ 0 = A
3.    A ^ ~0 = ~A
4.    A ^ A = 0
5.    A ^ B = B ^ A
6.    (A ^ B) ^ A = B
7.    (A ^ B) ^ B = A
8.    (A ^ B) ^ C = A ^ (B ^ C)

Or in English, it is the bit-by-bit difference between two values.

1.    see 4.
2.    The difference between a value and zero is the original value.
3.    The difference between a value and ones is the complement of the original value.
4.    If the values are identical, there are no differences, the result is zero.
5.    The order of operation does not matter. (^ is commutative)
6,7   bit-differences between two operands with one of the operands recovers the other operand.
8.    ^ is associative

Applying this, instead of having a ‘next’ and ‘prev’ like a normal doubly linked list, ‘NextPrev’ that holds the XOR of both the ‘next’ and ‘prev’.

    NextPrev = Next ^ Prev
    Next = NextPrev ^ Prev
    Prev = NextPrev ^ Next

Based on the rules above, both head and tail maintain the actual pointer to the next object because XOR with zero leaves the original bits.  As a side effect the head and tail pointers are the only entry points.  If there is a pointer to an object in the middle of the list, there is no way to traverse from that object without knowing the actual next or previous item.

From http://www.glassdoor.ca/Interview/Delete-a-node-from-an-XOR-linked-list-QTN_540423.htm

To delete current:
     Keep track of previous node.

     1. next     = curr.nxp ^ prev;

     2. prev.nxp = prev.nxp ^ curr ^ next;
                 = prev(prev) ^ next

     3. next.nxp = next.nxp ^ curr ^ prev;
                 = next(next) ^ prev;

     0 <-> 1 <-> 2 <-> 3 <-> 4 <-> 5 <-> 6 <-> 7 <-> 0
     delete 4

     with:
     curr = 4
     prev = 3

     next     = curr.nxp ^ prev;
              = 4.nxp ^ 3;
              = 5

     prev.nxp = prev.nxp ^ curr ^ next;
     3.nxp    = (3.nxp ^ 4) ^ 5;
              = 2 ^ 5

     next.nxp = next.nxp ^ curr ^ prev;
     5.nxp    = (5.nxp ^ 4) ^ 3;
              = 6 ^ 3

     which indeed produces
     0 <-> 1 <-> 2 <-> 3 <-> 5 <-> 6 <-> 7 <-> 0

NOTE:
     1. next     = curr.nxp ^ prev;
     2. prev.nxp ^= curr ^ next
     3. next.nxp ^= curr ^ prev;

+++++++++++++++++++++++++++++++++++++++++++++++++++++
XOR linked list

...  A        B         C         D         E  ...
        <–>  A^C  <->  B^D  <->  D^E  <->

     diff = next ^ prev
     next = diff ^ prev :  C = (A^C) ^ A
     prev = diff ^ next :  A = (A^C) ^ C
+++++++++++++++++++++++++++++++++++++++++++++++++++++
Subtraction linked list

...  A        B         C         D         E  ...
        <–>  C-A  <->  D-B  <->  E-C  <->

     diff = next - prev
     next = diff + prev :  C = (C-A) + A
     prev = next - diff :  A = C - (C-A)
+++++++++++++++++++++++++++++++++++++++++++++++++++++
Addition linked list

...  A        B         C         D         E  ...
        <–>  A+C  <->  B+D  <->  C+E  <->

     sum  = next + prev
     next = sum - prev
     prev = sum - next
+++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif

#include <iostream>
#include <cstdint>  // intptr_t
using namespace std;

// pop+push+walk from head+tail
// simple, concise, minimal, lean code

template <class T>
class LLXOR {
private:
    class Node {
    public:
        Node  *nxp; // nxp=next^previous
        T     data;
        Node(Node* nxp_, T& data_) { nxp = nxp_; data = data_;}
    };

    Node *head;
    Node *tail;

public:
    LLXOR() : head(nullptr), tail(nullptr) { 
        cout<<"ctor "<<this<<"\n"; 
    }
    ~LLXOR() { 
        cout<<"dtor "<<this<<"\n";
        clear();
    }
    void clear() { // 
        Node *curr = head;
        Node *prev = nullptr;
        Node *next;

        while (nullptr != curr) {
            next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->nxp);
            prev = curr;
            curr = next;
            delete prev;
        }
        head=tail=nullptr;
    }
    LLXOR& operator=(const LLXOR& rhs)  //  assignment operator
    {
        if ( this != &rhs ) {
            clear(); // FUBAR if we call clear when this==&rhs

            Node *curr = rhs.head;
            Node *prev = nullptr;
            Node *next;

            while (nullptr != curr) {
                pushTail( curr->data );
                next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->nxp);
                prev = curr;
                curr = next;
            }
        }
        return *this;
    }
    LLXOR(const LLXOR& src)   //  copy ctor
    {
        head = tail = nullptr;
        std::cout <<"LLXOR:: copy ctor: this=" <<this <<" &src="  <<&src <<"\n";
        *this = src; // invoke assignment operator
    }
    void pushHead(T& data)
    {
        // cout << "adding " << data << " at head\n";
        if ( nullptr == head ) { // list is empty.  Add at head which is also the tail.
            head = tail = new Node(nullptr, data);
        } else {
            Node *node = new Node(head,data);
            head->nxp = (Node*) ((uintptr_t) node ^ (uintptr_t) head->nxp);
            head      = node;
        }
    }
    void pushTail(T& data)
    {
        // cout << "adding " << data << " at tail\n";
        if ( nullptr == head ) { // list is empty.  Add at head which is also the tail.
            head = tail = new Node(nullptr, data);
        } else {
            Node *node = new Node(tail,data);
            tail->nxp = (Node*) ((uintptr_t) node ^ (uintptr_t) tail->nxp);
            tail      = node;
        }
    }
    bool popHead(T& data)
    {
        bool ret = false;
        if ( nullptr == head ) {
            cerr << "Empty list.\n";
        } else {                // deleting from head
            Node *node = head;
            data       = node->data;
            Node *next = node->nxp;
            if ( nullptr == next )
                tail = nullptr;
            else
                next->nxp = (Node*) ((uintptr_t) node ^ (uintptr_t) next->nxp);
            head = next;
            delete node;
            node  = nullptr;
            ret = true;
        }
        return ret;
    }
    bool popTail(T& data)
    {
        bool ret = false;
        if ( nullptr == head ) {
            cerr << "Empty list.\n";
        } else {               // deleting from tail
            Node *node = tail;
            data       = node->data;
            Node *prev = node->nxp;
            if ( nullptr == prev )
                head = nullptr;
            else
                prev->nxp = (Node*) ((uintptr_t) node ^ (uintptr_t) prev->nxp);
            tail = prev;
            delete node;
            node = nullptr;
            ret = true;
        }
        return ret;
    }
    bool empty()
    {
        return !head;
    }
    size_t count()
    {
        size_t ret = 0;
        Node *curr = head;
        Node *prev = nullptr;
        Node *next;

        while (curr) {
            ret++;
            next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->nxp);
            prev = curr;
            curr = next;
        }
        return ret;
    }
    void walk(Node *curr, bool verbose=false)
    {
        Node *prev = nullptr;
        Node *next;

        if ( verbose ) {
            char *source;
            if(curr == head) source = (char*) " (head)"; else source = (char*) " (tail)";
            cout << "verbose walk linked list " << this 
            <<" head=" << head 
            <<" tail=" << tail 
            << " starting at curr=" << curr << source
            <<"\n";
        }
        while (curr) {
            if ( verbose ) {  // 
                cout << curr << ":";
                cout << curr->nxp << " ";
                cout << curr->data << "\n";
            } else {
                cout << curr->data << " ";
            }
            next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->nxp);
            prev = curr;
            curr = next;
        }
        cout << "\n";
    }
    void walkFWD(bool verbose=false) // 
    {
        walk(head,verbose);  // start with head
    }
    void walkREV(bool verbose=false) // 
    {
        walk(tail,verbose);  // start with tail
    }
};

int main(int argc, char *argv[])
{
    LLXOR<int> ll;
    for ( int i = 1; i <= 10; i++ ) {
        if(i < 6)
            ll.pushTail(i);
        else
            ll.pushHead(i);
    }

    ll.walkFWD(); // 10 9 8 7 6 1 2 3 4 5
    ll.walkREV(); // 5 4 3 2 1 6 7 8 9 10

    int data;
    for ( int i = 1; i <= 4; i++ ) {
        bool fromTail = i < 3;
        if(fromTail)
            ll.popTail(data);
        else
            ll.popHead(data);
        cout << "deleted " << data << (fromTail? " from tail\n": " from head\n");
    }

    ll.walkFWD(); // 8 7 6 1 2 3
    ll.walkREV(); // 3 2 1 6 7 8
    ll.walkFWD(true);
    ll.walkREV(true);

    LLXOR<int> ll2;
    ll2 = ll;              // assigment operator
    ll2.walkFWD();

    LLXOR<int> ll3 = ll2;  // copy ctor
    ll3.walkFWD();

    return 0;
}
