// Algo 1 push head+tail, walk head+tail
// Algo 2 push head, walk head, no push tail, walk tail
// Algo 3 push head, walk head+tail, no push tail
// Algo 4 pop+push+walk from head+tail              <== best algorithm
// Algo 5 push head, walk head+tail, no push tail   <== website has nice diagrams
//        http://cybercrud.net/2012/07/04/complicating-things-with-xor-linked-lists/
#if 0
XOR is a very interesting logical operator.  The basic rules are:


1.    0 ^ 0 = 0
2.    A ^ 0 = A
3.    A ^ ~0 = ~A
4.    A ^ A = 0
5.    A ^ B = B ^ A
6.    (A ^ B) ^ A = B
7.    (B ^ A) ^ B = A
8.    (A ^ B) ^ C = A ^ (B ^ C)

Or in English, it is the bit-by-bit difference between two values.

1.    see 4.
2.    The difference between a value and zero is the original value.
3.    The difference between a value and ones is the complement of the original value.
4.    If the values are identical, there are no differences, the result is zero.
5.    The order of operation does not matter. (^ is commutative)
6,7   bit-differences between two operands with one of the operands recovers the other operand.
8.    ^ is associative

Corollories
     A ^ B = (A | B) & ~(A & B)
     A ^ B = (A & ~B) | (~A & B)

When we apply this, instead of having a ‘next’ and ‘prev’ like a normal linked list, we have one ‘NextPrev’ that holds the XOR of both the ‘next’ and ‘prev’.

    NextPrev = Next ^ Prev
    Next = NextPrev ^ Prev
    Prev = NextPrev ^ Next

Based on the rules above, both head ( with Next as 0 ) and tail (with Prev as 0) maintain the actual pointer to the next object because XOR with zero leaves the original bits.  As a side effect the head and tail pointers are the only entry points.  If there is a pointer to an object in the middle of the list, there is no way to traverse from that object without knowing the actual next or previous item.

To delete current:
Keep track of previous node.
next = current.x ^ previous;
prev.x = prev.x ^ current ^ next;
next.x = next.x ^ current ^ prev;

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
#include <stdint.h>  // intptr_t
using namespace std;

class Node {
public:
   int  data;
   Node  *npx; /* npx=XOR of next and previous node */
};

///////////////////////// Algo 1 /////////////////////////
// derived from http://stackoverflow.com/questions/3531972/c-code-for-xor-linked-list
// converted in-line code to class Algo1LL;
// Algo 1 push head+tail, walk head+tail

class Algo1LL {
private:
    Node *head;
    Node *tail;
    int count;
public:
    Algo1LL() : head(nullptr), tail(nullptr), count(0) {}
    ~Algo1LL() { }

private:
    // traverse the list given either the head or the tail
    void traverse( Node *root )  // point to head or tail
    {
       Node *prev, *curr;

       curr = prev = root;
       while ( curr ) {
          cout <<"data = " <<curr->data <<"\n";
          if ( curr->npx == curr ) {
             break; // done
          }
          if ( curr == prev ) {
             curr = curr->npx;   // start of list
          } else {
             Node *save = curr;
             curr = (Node*)((uintptr_t)prev ^ (uintptr_t)curr->npx);
             prev = save;
          }
       }
    }

    // create a new node adding it to the given end and return it
    Node* newnode( Node *prev, Node *curr, int data )
    {
       Node *next = new Node;
       next->data = data;
       next->npx   = curr;  // end node points to previous one

       count++;

        if ( curr == nullptr ) {
            ; // very first node - we'll just return it
        } else if ( prev == nullptr ) {
            // this is the second node (they point at each other)
            curr->npx = next;
            next->npx = curr;
        } else {
            // do the xor magic
            curr->npx = (Node*)((uintptr_t)prev ^ (uintptr_t)next);
        }

       return next;
    }
    void firstNode(int data)
    {
        // the first two nodes point at each other.
        // Weird param calls to get the list started
        head = tail = newnode( nullptr, nullptr, data );
    }
    void secondNode(int data)
    {
        // the first two nodes point at each other.
        // Weird param calls to get the list started
        tail = newnode( nullptr, tail, data );
    }
public:
    void pushHead(int data)
    {
        switch(count) {
            case 0:  firstNode(data);                           break;
            case 1:  secondNode(data);                          break;
            default: head = newnode( head->npx, head, data );    break;
        }
    }
    void pushTail(int data)
    {
        switch(count) {
            case 0:  firstNode(data);                           break;
            case 1:  secondNode(data);                          break;
            default: tail = newnode( tail->npx, tail, data );    break;
        }
    }
    void traverseForwards( )
    {
        traverse( head );
    }
    void traverseBackwards( )
    {
        traverse( tail );
    }
};

int main1( int argc, char* argv[] )
{
    int   data = 1;
    Algo1LL ll;

    ll.pushHead(data++);
    ll.pushTail(data++);
    ll.pushTail(data++);
    ll.pushTail(data++);
    ll.pushHead(999);

    cout <<"Algo 1: Forwards:\n";
    ll.traverseForwards( );
    cout <<"Algo 1: Backwards:\n";
    ll.traverseBackwards( );

    return 0;
}


///////////////////////// Algo 2 /////////////////////////
// XOR LL #2
// http://www.geeksforgeeks.org/xor-linked-list-a-memory-efficient-doubly-linked-list-set-1/
// http://www.geeksforgeeks.org/xor-linked-list-a-memory-efficient-doubly-linked-list-set-2/
// Algo 2 push head, walk head

// GB XOR of anything with zero is anything, simplify + eliminate XOR with zero
// GB convert to C++

// Insert a node at the begining of the XORed linked list and makes the
// newly inserted node as head
class Algo2LL {
    private:
        Node *head;
    public:
        Algo2LL () { head = nullptr; }
        void insert(int data)
        {
            // Allocate memory for new node
            Node *new_node  = new Node;
            new_node->data = data;

            /* Since new node is being inserted at the begining, npx of new node
               will always be XOR of current head and nullptr */
            new_node->npx = head;

            /* If linked list is not empty, then npx of current head node will be XOR
               of new node and node next to current head */
            if ( head ) {
                // head->npx is XOR of nullptr and next. So if we do XOR of
                // it with nullptr, we get next
                Node* next = head->npx;
                head->npx = (Node*) ((uintptr_t) new_node ^ (uintptr_t) next);
            }

            // Change head
            head = new_node;
        }

        void traverse()
        {
           Node *prev, *curr;

           cout <<"Algo 2 traverse: Following are the nodes of Linked List: ";

           curr = prev = head;
           while ( curr ) {
              cout <<curr->data <<" ";
              if ( curr->npx == curr ) {
                 break; // done
              }
              if ( curr == prev ) {
                 curr = curr->npx;   // start of list
              } else {
                 Node *save = curr;
                 curr = (Node*)((uintptr_t)prev ^ (uintptr_t)curr->npx);
                 prev = save;
              }
           }
           cout <<"\n";
        }

        // prints contents of doubly linked list in forward direction
        void printList()
        {
            Node *curr = head;
            Node *prev = nullptr;
            Node *next;

            cout <<"Algo 2 printList: Following are the nodes of Linked List: ";

            while ( curr ) {
                // print current node
                cout <<curr->data <<" ";

                // get address of next node: curr->npx is next^prev, so curr->npx^prev
                // will be next^prev^prev which is next
                next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->npx);

                // update prev and curr for next iteration
                prev = curr;
                curr = next;
            }
            cout <<"\n";
        }
};

// Driver program to test above functions
int main2( int argc, char* argv[] )
{
    /* Create following Doubly Linked List
       head-->40<-->30<-->20<-->10   */
    Algo2LL ll;
    ll.insert(10);
    ll.insert(20);
    ll.insert(30);
    ll.insert(40);

    // print the created list
    ll.printList ();
    ll.traverse ();
    // problem with tail:  there is no tail!
    // cout <<"Algo 2: calling traverse2(tail)\n";
    // traverse2 (tail);
    // Need to figure out how to traverse list in reverse.
    // printList (head);

    return 0;
}


///////////////////////// Algo 3 /////////////////////////
// Wikipedia points to this code (http://en.wikipedia.org/wiki/XOR_linked_list)
// http://blog.wsensors.com/2011/04/the-xor-linked-list-in-c/
// Algo 3 push head, walk head+tail

// Implementation
// This is the header file for the list.  The behavior of the list is:
//     If there are no entries head and tail are 0
//     If there is one entry, head is the entry and tail is 0
//     If there are 2 or more entries head and tail are both set as are the items npx variable that holds both next and previous

class Algo3LL
{
public:
    Algo3LL()  { head = tail = nullptr; }
    ~Algo3LL() {while( RemoveFromHead() ); head=tail=nullptr;}

    void AddToTail( Node* entry )
    {
        if ( !head ) { //no items
            entry->npx = nullptr;
            head = entry;
        } else if ( !tail ) { //just head is set, one item in list
            tail      = entry;
            tail->npx = head;
            head->npx = tail;
        } else { //two items or more, insert
            entry->npx = tail;
            tail->npx = (Node*)( (uintptr_t)(tail->npx) ^ (uintptr_t)entry);
            tail = entry;
        }
    }
    Node *RemoveFromHead()
    {
        Node* item = head;

        if ( !head ) {
            return nullptr;
        } else if ( !tail ) { // just one item
            head      = nullptr;
            return item;
        } else if ( head->npx == tail
               &&  tail->npx == head) { // only two items
            head      = tail;
            tail      = nullptr;
            head->npx = nullptr;
            return item;
        } else {
            head      = head->npx;
            head->npx = (Node*)((uintptr_t)(head->npx) ^(uintptr_t) item);
            return item;
        }
    }
   void traverseForwards( )
    {
        traverse( head );
    }
    void traverseBackwards( )
    {
        traverse( tail );
    }
    void push(int data)
    {
        Node *newItem = new Node;
        // cout <<"Algo3: inserting " <<data <<" @" <<newItem <<"\n";
        newItem->data = data;
        AddToTail(newItem);
        // cout <<"Added item: " <<newItem->data <<"\n";
    }
    int pop()
    {
        int ret;
        // cout <<"Algo3: retrieving " <<"\n";
        Node *retrievedItem = RemoveFromHead();
        // cout <<"Retrieved item: " << retrievedItem->data <<" @" <<retrievedItem <<"\n";
        ret = retrievedItem->data;
        delete retrievedItem;

        return ret;
    }
private:
    void traverse( Node *start )  // point to head or tail (cloned from Algo1)
    {
        Node *prev, *curr;

        curr = prev = start;
        while ( curr ) {
            cout <<"traverse = " <<curr->data <<"\n";
            if ( curr->npx == curr ) {
                break; // done
            }
            if ( curr == prev ) {
                curr = curr->npx;   // start of list
            } else {
                Node *save = curr;
                curr = (Node*)((uintptr_t)prev ^ (uintptr_t)curr->npx);
                prev = save;
            }
        }
    }
private:
    Node *tail;
    Node *head;
};

int main3( int argc, char *argv[] )
{
    Algo3LL  *list = new Algo3LL;

    int x, count = 10;

    cout << "Algo 3:\n";
    for( x = 0;  x < count; x++ ) { //add all items to list
        list->push(x);
    }
    cout << "Algo 3 forward:\n";
    list->traverseForwards( );
    cout << "Backwards:\n";
    list->traverseBackwards( );

    cout <<"Algo 3 Algo 3 popped: ";
    for ( x = 0; x < count; x++ ) {
        int data = list->pop();
        cout << data <<" ";
    }
    cout << "\n";

    delete list;

    return 0;
}

///////////////////////// Algo 4 /////////////////////////
// http://eddmann.com/posts/implementing-a-xor-doubly-linked-list-in-c/
// Algo 4 pop+push+walk from head+tail              <== best algorithm
// GB converted to c++
// GB simplified unnecessary A^0 operations using property A^0=A
// By far, algo 4 is the best algorithm: pop+push+walk from head+tail
// GB simple, concise, minimal, lean code

class Algo4LL {
private:
    Node *head;
    Node *tail;

public:
    Algo4LL() : head(nullptr), tail(nullptr) { cout<<"Algo4 ctor "<<this<<"\n"; }
    ~Algo4LL() { cout<<"Algo4 dtor "<<this<<"\n"; clear(); } // (GB)
    void clear() { // (GB)
        Node *curr = head;
        Node *prev = nullptr, *next;

        while (nullptr != curr) {
            next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->npx);
            prev = curr;
            curr = next;
            delete prev;
        }
        head=tail=nullptr;
    }
    Algo4LL& operator=(const Algo4LL& rhs)  // (GB) assignment operator
    {
        if ( this != &rhs ) {
            clear();

            Node *curr = rhs.head;
            Node *prev = nullptr, *next;

            while (nullptr != curr) {
                bool at_tail = true;
                push( curr->data, at_tail);
                next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->npx);
                prev = curr;
                curr = next;
            }
        }
        return *this;
    }
    Algo4LL(const Algo4LL& src)   // (GB) copy ctor
    {
        head = tail = nullptr;
        std::cout <<"Algo4LL:: copy ctor: this=" <<this <<" &src="  <<&src <<"\n";
        *this = src; // invoke assignment operator
    }

    void push(int data, bool at_tail)
    {
        cout << "adding " << data << (at_tail? " at tail\n": " at head\n");
        Node *ptr = new Node;
        ptr->data = data;

        if ( nullptr == head ) { // list is empty.  Add at head.
            ptr->npx = nullptr;
            head     = tail = ptr;
        } else if ( at_tail ) {
            ptr->npx  = tail;
            tail->npx = (Node*) ((uintptr_t) ptr ^ (uintptr_t) tail->npx);
            tail      = ptr;
        } else {
            ptr->npx  = head;
            head->npx = (Node*) ((uintptr_t) ptr ^ (uintptr_t) head->npx);
            head      = ptr;
        }
    }
    bool pop(int& data, bool from_tail)
    {
        if ( nullptr == head ) {
            cerr << "Empty list.\n";
            return false;
        } else if ( from_tail ) { // deleting from tail
            Node *ptr  = tail;
            data       = ptr->data;
            Node *prev = ptr->npx;
            if ( nullptr == prev )
                head = nullptr;
            else
                prev->npx = (Node*) ((uintptr_t) ptr ^ (uintptr_t) prev->npx);
            tail = prev;
            delete ptr;
            ptr = nullptr;
        } else {                // deleting from head
            Node *ptr  = head;
            data       = ptr->data;
            Node *next = ptr->npx;
            if ( nullptr == next )
                tail = nullptr;
            else
                next->npx = (Node*) ((uintptr_t) ptr ^ (uintptr_t) next->npx);
            head = next;
            delete ptr;
            ptr  = nullptr;
        }
        return true;
    }
    void list(Node *curr, bool verbose=false)
    {
        Node *prev = nullptr, *next;

        if ( verbose ) cout <<"tree " << this <<" head="<<head <<" tail=" <<tail <<"\n";
        while (nullptr != curr) {
            if ( verbose ) {  // (GB)
                cout << curr << ":";
                cout << curr->npx << " ";
                cout << curr->data << "\n";
            } else {
                cout << curr->data << " ";
            }
            next = (Node*) ((uintptr_t) prev ^ (uintptr_t) curr->npx);
            prev = curr;
            curr = next;
        }
        cout << "\n";
    }
    void listFWD(bool verbose=false) // (GB)
    {
        list(head,verbose);  // start with head
    }
    void listREV(bool verbose=false) // (GB)
    {
        list(tail,verbose);  // start with tail
    }
};

int main4(int argc, char *argv[])
{
    Algo4LL ll;
    for ( int i = 1; i <= 10; i++ ) {
        bool fromTail = i < 6;
        ll.push(i, fromTail);
    }

    ll.listFWD(); // 10 9 8 7 6 1 2 3 4 5

    int data;
    for ( int i = 1; i <= 4; i++ ) {
        bool fromTail = i < 3;
        ll.pop(data, fromTail);
        cout << "Algo 4 deleted " << data << (fromTail? " from tail\n": " from head\n");
    }

    ll.listFWD(); // 8 7 6 1 2 3
    ll.listFWD(true);
    cout << "Algo 4 reverse order\n";
    ll.listREV(); // 8 7 6 1 2 3
    ll.listREV(true);

    Algo4LL ll2;
    ll2 = ll;
    ll2.listFWD();

    Algo4LL ll3 = ll2;;
    ll3.listFWD();

    return 0;
}

///////////////////////// Algo 5 /////////////////////////
// http://cybercrud.net/2012/07/04/complicating-things-with-xor-linked-lists/
// Algo 5 push head, walk head+tail
// no pop

class Algo5LL {
private:
    Node *head;
    Node *tail;
    int count;    // GB debug lost memory
private:
    void traverse(Node *root) {
        Node *current, *prev, *next;

        prev    = nullptr;
        current = root;

        while (current) {
            std::cout << "Node found: " << current->data << "\n";

            next    = nextNode(current, prev);
            prev    = current;
            current = next;
        }

        std::cout << "\n";
    }
    void insertAfter(Node *newNode, int data) {
        Node *prev, *current, *next;

        prev    = nullptr;
        current = head;

        while ( current ) {
            next = nextNode(current, prev);

            if ( current->data == data ) {
                // Fix the current "next" node
                if ( next )
                    next->npx = (Node *) ((uintptr_t) next->npx
                                        ^ (uintptr_t) current
                                        ^ (uintptr_t) newNode);

                // Fix current node
                current->npx = (Node *) ((uintptr_t) newNode
                                       ^ (uintptr_t) next
                                       ^ (uintptr_t) current->npx);

                // Fix new node
                newNode->npx = (Node *) ((uintptr_t) current ^ (uintptr_t) next);

                break;
            }

            prev    = current;
            current = next;
        }
    }

  public:

    Algo5LL() {head=tail=nullptr; count=0;}
    ~Algo5LL() { deleteList(); }

    Node *nextNode(Node *node, Node *prevNode) {
        return ((Node *) ((uintptr_t) node->npx ^ (uintptr_t) prevNode));
    }

    void deleteList(void) {
        std::cout << "Algo5: deleteList: node count " << count << "\n";
        Node *prev, *current;

        prev    = nullptr;
        current = head;

        while ( current ) {
            std::cout << "Node removed: " << current->data << "\n";
            current->npx = nextNode(current, prev);

            if ( prev ) {
                delete prev;
                count--;    // GB debug lost memory
                prev = nullptr;
            }

            if ( !current->npx ) {
                delete current;
                current = nullptr;
                count--;    // GB debug lost memory

                continue;
            }

            prev    = current;
            current = current->npx;
        }
        head=tail=nullptr;

        std::cout << "\n";
    }

    void traverseFWD() {
        traverse(head);
    }
    void traverseREV() {
        traverse(tail);
    }
    void insert(int data) {
        Node *newNode = new Node;

        if ( !newNode ) {
            std::cerr << "[ERROR] Failed to insert new node." << "\n";
            return;
        }

        newNode->data = data;
        newNode->npx = nullptr;
        count++;    // GB debug lost memory

        std::cout << "Node added: " << newNode->data << "\n";

        if ( !head )
            head = tail = newNode;
        else {
            insertAfter(newNode, tail->data);
            tail = newNode;
        }

        return;
    }
};

// main5.cpp

int main5(int argc, char *argv[]) {
    Algo5LL *list = new Algo5LL;

    int nodeCount;

    if ( argc < 2 ) {
        std::cerr << "Algo5 usage: " << argv[0] <<  " <nodes>" << "\n";
        return 1;
    }

    nodeCount = atoi(argv[1]);

    std::cout << "Algo 5 # Adding " << nodeCount << " nodes to list" << "\n";

    for ( int i = 0; i < nodeCount; i++ ) {
        list->insert(i);
    }

    std::cout << "\n";

    std::cout << "Algo 5 # Forward traversal" << "\n";
    list->traverseFWD();

    std::cout << "Algo 5 # Backward traversal" << "\n";
    list->traverseREV();

    std::cout << "Algo 5 # Removing nodes from list" << "\n";
    list->deleteList();

    delete list;
    list = nullptr;

    return 0;
}


int main( int argc, char* argv[] )
{
    main1( argc, argv );
    main2( argc, argv );
    main3( argc, argv );
    main4( argc, argv );
    main5( argc, argv );
}
