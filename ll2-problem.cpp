// http://tnvbalaji.com/2012/04/04/linkedlist/
// GB: =op + copy ctor

// g++ -std=c++0x ll2.cpp -o ll2

#include <iostream>
using namespace std;
class Node {
private:
  int   _val;
  Node* _next;
public:
  Node  () :_next(nullptr) {}
  ~Node ()                 {}
  void  next(Node *n)      { this->_next = n; }
  Node *next()             { return this->_next; }
  void val(int v)          { this->_val = v; }
  int  val()               { return this->_val; }
};

class LL
{
private:
  Node *_head;
public:
  void head(Node *head) { _head=head;};
  Node *head()          { return _head; }  // makes trouble in assignment operator
  LL();
  ~LL();
  LL(const LL& src); // GB
  LL& operator=(const LL& src); // assignment operator GB

  void Print(const char*msg=nullptr);         // Prints the contents of linkedlist
  void Append(int val); // Adds a new node at the end of the linkedlist
  void Delete(int val); // Deletes the specified node from the linkedlist

  void AddatBeg(int val);// Adds a new node at the beginning of the linkedlist
  void AddAfter(int pos, int val); // Adds a new node after specified number of nodes
  int Count();          // Counts number of nodes present in the linkedlist
  void clear();         // delete everything
};

LL::LL()
  : _head(nullptr)
{
  head(nullptr);
  cout<< "ctor\n";
}

LL::~LL()
{
  cout<< "dtor\n";
  clear();
}

LL::LL(const LL& src)            // copy ctor
  : _head(nullptr)
{
  head(nullptr);
  cout<< "copy ctor\n";
  *this = src;
}

LL& LL::operator=(const LL& src)    // assignment operator
// LL& LL::operator=(const LL& s)   // assignment operator
{
    // LL src = const_cast<LL&> (s);         // compiles, but run-time error assignment operator loops
    cout<< "=op\n";

    if(this != &src) {
        this->clear();

        Node* pDst = nullptr;
        Node* pSrc = nullptr;

//      if(src.head() == nullptr)             // getter head() won't compile
        if(src._head == nullptr)
        {
            head( nullptr );
        } else {
            head( new Node );
//          head()->val( src.head()->val() ); // getter head() won't compile
            head()->val( src._head->val() );

            pDst = head();
//          pSrc = src.head()->next();        // getter head() won't compile
            pSrc = src._head->next();
        }

        while(pSrc) {
            pDst->next( new Node );
            pDst = pDst->next();
            pDst->val( pSrc->val() );

            pSrc = pSrc->next();
        }
        pDst->next( nullptr );
    }
    return *this;
}

void LL::clear()
{
  cout<< "clear\n";
  while(head() != nullptr) {
    Node *n = head()->next() ;
    delete head();
    head( n );
  }
  head( nullptr );
}

// Prints the contents of linkedlist
void LL::Print(const char*msg)
{
  cout <<this <<" ++++++ " <<msg << "\n";
  if (head() == nullptr) {
    cout<< "EMPTY" << "\n";
    return;
  }

  //Traverse
  for(Node* n = head(); n != nullptr;  n = n->next())
    cout<<n <<" " <<n->val()<<"\n";
  cout<<"\n";
}

// Adds a new node at the end of the linkedlist
void LL::Append(int val)
{
  Node *newNode = new Node;
  newNode->val( val );
  newNode->next( nullptr );

  if(head() == nullptr) { //create first node
    head( newNode );
  } else {
    //Traverse
    Node *n = head();
    while(n->next() != nullptr) {
      n = n->next();
    }

    //add node to the end
    n->next( newNode );
  }
}

// Deletes the specified node from the linkedlist
void LL::Delete( int val )
{
  Node *n;

  n = head();
  //If node to be deleted is first node
  if( n->val() == val ) {
    head( n->next() );
    delete n;
    return;
  }

  // traverse list till the last but one node is reached
  Node *n2 = n;
  while( n!=nullptr ) {
    if( n->val() == val ) {
      n2->next( n->next() );
      delete n;
      return;
    }

    n2 = n;
    n = n->next();
  }
  cout<< "\nElement "<<val<<" not Found." ;
}

// Adds a new node at the beginning of the linkedlist
void LL::AddatBeg(int val)
{

  //add new node
  Node *n = new Node;
  n->val( val );
  n->next(  head() );
  head( n );
}

//Adds a new node after specified number of nodes
void LL::AddAfter(int position, int val)
{
  Node *n;
  Node *n2;
  int i;
  //Skip to the desired portion
  for( i = 0, n = head(); i<position; i++) {
    n = n->next();

    //if end of linked list is encountered
    if(n == nullptr) {
      cout<<"\n"<< "There are less than "<<position<<" elements" ;
      return;
    }
  }

  //insert new node
  n2 = new Node;
  n2->val(val);
  n2->next(  n->next() );
  n->next( n2 );
}

// Counts number of nodes present in the linkedlist
int LL::Count()
{
  int c = 0;
  //Traverse the entire Linked List
  for (Node *n = head(); n != nullptr; n = n->next())
    c++;
  return c;
}

int main(int argc, char**argv)
{
  LL* ll = new LL();
  ll->Append(11);
  ll->Append(22);
  ll->Append(33);
  ll->Print("ll");

  ll->Delete(33);
  ll->Print("ll");

  ll->AddatBeg(44);
  ll->Print("ll");

  ll->AddAfter(1, 55);
  ll->Print("ll");

  LL ll2;
  ll2 = *ll;
  ll2.Print("ll2");

  LL ll3 = *ll;
  ll3.Print("ll3");

  delete ll;
}
