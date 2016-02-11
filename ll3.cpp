#include <iostream>
using namespace std;


class Data {
public:
 int data;
 Data(int d) { data = d; }
};

class Node {
public:
	Node *link;
	int data;
};

class LL {
private:
	Node *head;
public:
	LL() : head(nullptr) {}
	void push(int d)
	{
		Node *n = new Node;
		n -> link = head;
		head = n;
		n -> data = d;
	}
	int pop()
	{
		if(head == nullptr)
			cout << "list is empty --- nothing to pop\n";
		else {
			int ret = head->data;
			Node *n = head;
			head = head->link;
			delete n;
			return ret;
		}
	}
	void print()
	{
		if(head) {
			for( Node *p = head; p; p=p->link)
				cout << p->data << "\n";
		} else {
			cout << "List is empty\n";
		}
	}

};

int main(int argc, char **argv)
{
	LL l;

	l.push(9999);
	l.push(888888898);
	l.push(123);
	l.push(444);
	l.print();
	
	cout << "popped: " << l.pop() << "\n";
	l.print();
	
	cout << "popped: " << l.pop() << "\n";
	
	cout << "popped: " << l.pop() << "\n";
	l.print();

	cout << "popped: " << l.pop() << "\n";
	l.print();
}

