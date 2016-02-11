// Q code adapted from
// http://libraryofcprograms.blogspot.ca/2013/03/linkedlist-classes.html

#include <iostream>
using namespace std;

class Q
{
    public:
        Q();
        ~Q();
        void push(int value);
        int pop();
        bool empty();
        void print();
    private:
        //int index;
        int value;
        Q *next;   // data node
        Q *head;
        Q *tail;
};

Q::Q()
{
    head = nullptr, tail = nullptr;
}


Q::~Q()
{
    Q *p = head;
    while(p)
    {
        Q *next = p->next;
        delete p;
        p = next;
    }
    head = nullptr, tail = nullptr;
}


void Q::push(int value)
{
    Q *node = new Q;
    node -> next = 0;

    //push at tail.
    if (head && tail)
    {
        tail->next = node;
        tail = node;
    }
    else
    {
        head = node;
        tail = node;
    }
    node->value = value;
}

bool Q::empty() { return head == tail; }

int Q::pop() // GB test this!
{
    int value = 0;
    if(tail) {
        value = tail->value;
    }
    // find node that points to tail
    for(Q *p = head; p; p = p->next)
    {
        if(p->next == tail) 
        {
            delete tail;
            p->next = nullptr;
            tail = p;
            break;
        }
    }
    return value;
}

void Q::print()
{
    for(Q *p = head; p; p = p->next)
    {
        cout << p->value << "\n";
    }
}

int main()
{
    Q q;
    while (1)
    {
        cout << "1. Insert number\n2. Print All\nCMD> "; int opt; cin >> opt;
        switch(opt)
        {
            case 1:
                int value;
                cout << "number? ";
                cin >> value;
                q.push(value);
                continue;
            case 2:
                q.print();
                continue;
            default:
                break;
        }
        break;
    }
    return 0;
}
