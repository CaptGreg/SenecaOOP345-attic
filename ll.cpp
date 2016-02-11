// http://www.bogotobogo.com/cplusplus/linkedlist.php

// linked list example - using struct
#include <iostream>
#include <cstring>

using namespace std;

struct node * initNode( const char *, int );
void displayNode( struct node * );
void displayList( struct node * );
void addNode( struct node * );
struct node * searchName( struct node *, char * );
void deleteNode( struct node * );
void insertNode( struct node * );
void deleteList( struct node * );

struct node {
   char name[20];
   int  id;
   struct node *next;
};

struct node *head = (struct node *) nullptr;
struct node *tail = (struct node *) nullptr;

// allocates memory for the node
// assign values to the member of the structure
struct node * initNode( const char *name, int id )
{
   struct node *ptr = new node;

   // error? then just return
   if( ptr == nullptr )                        
       return static_cast<struct node *>(nullptr);       
   	// assign it 
	// then return pointer to ne node
   else {                                   
       strcpy( ptr->name, name );           
       ptr->id = id;                        
       return ptr;                   
   }
}

// adding to the end of list
void addNode( struct node *newnode )   
{
	// if there is no node, put it to head
	if( head == nullptr ) {           
       head = newnode;                          
       tail = newnode;
	}

	// link in the new_node to the tail of the list
	// then mark the next field as the end of the list
	// adjust tail to point to the last node

	tail->next = newnode;         
	newnode->next = nullptr;        
	tail = newnode;              
}
                                                            
void insertNode( struct node *newnode )
{
   struct node *temp, *prev;                 

   if( head == nullptr ) {                     // if an empty list,  
       head = newnode;                      // set 'head' to it  
       tail = newnode;
       head->next = nullptr;                   // set end of list to nullptr 
       return;                             
   }

   temp = head;                             // start at beginning of list
					    // while currentname < newname 
   while( strcmp( temp->name, newnode->name) < 0 ) {
					    // to be inserted then 
          temp = temp->next;                // goto the next node in list
          if( temp == nullptr )                // don't go past end of list  
              break;
   }
					    // set previous node before we insert  
					    // first check to see if it's inserting   
   if( temp == head ) {		    	    // before the first node 
      newnode->next = head;                 // link next field to original list  
      head = newnode;                       // head adjusted to new node
   }
   else {				    // it's not the first node
      prev = head;			    // start of the list,
      while( prev->next != temp ) {	    // will cycle to node before temp
          prev = prev->next;
      }
      prev->next = newnode;		    // insert node between prev and next 
      newnode->next = temp;
      if( tail == prev )		    // if the new node is inserted at the 
         tail = newnode;		    // end of the list the adjust 'end' 
   }
}

struct node * searchName( struct node *ptr, char *name )
{
    while( strcmp( name, ptr->name ) != 0 ) {   
       ptr = ptr->next;                          
       if( ptr == nullptr )                         
          break;                                  
    }
    return ptr;                                  
}

struct node* searchId(struct node* ptr, int id) {
    while( id != ptr->id ) {    
       ptr = ptr->next;                          
       if( ptr == nullptr )                          
          break;                                  
    }
    return ptr; 	
}

void reverse() {
	// we need at least two nodes for the reverse to have any effect
	if(head == nullptr || head->next == nullptr) return;

	// Starting 2nd list as 'me' and 'head' is now 'me->next'
	// and 'head->next' is pointing to nullptr
	// So, the 3rd list is now 'child' of 'me'
	node *parent = head;
	node *me = head->next;
	node *child = me->next;

	// convert head to tail
	parent->next = nullptr;
	
	while(child != nullptr){
		me->next = parent;
		parent = me;
		me = child;
		child = child->next;
	}
	// when me reached the tail
	// me becomes head
	head = me;
	// the head is now pointing to the 2nd last node
	head->next = parent;
}

void deleteNode( struct node *ptr )
{
  struct node *temp, *prev;
   temp = ptr;    // node to be deleted 
   prev = head;   // start of the list, will cycle to node before temp    

   if( temp == prev ) {                    // deleting first node?  
       head = head->next;                  // moves head to next node     
       if( tail == temp )                  // is it end, only one node?   
          tail = tail->next;               // adjust end as well          
       delete temp ;                       // free up space 
   }
   else {                                  // if not the first node, then 
       while( prev->next != temp ) {       // move prev to the node before
           prev = prev->next;              // the one to be deleted       
       }
       prev->next = temp->next;            // link previous node to next  
       if( tail == temp )                  // if this was the end node,   
           tail = prev;                    // then reset the end pointer  
      delete temp;                         // free up space
   }
}

void deleteList( struct node *ptr )
{
   struct node *temp;

   if( head == nullptr ) return;   	// don't try to delete an empty list      

   if( ptr == head ) {			// if we are deleting the entire list    
       head = nullptr;			// then reset head and    
       tail = nullptr;			// end to empty                                     
   }
   else {
       temp = head;			// if it's not the entire list, readjust end   
       while( temp->next != ptr )       // locate previous node to ptr   
           temp = temp->next;
       tail = temp;                     // set end to node before ptr   
   }

   while( ptr != nullptr ) {		// while there are still nodes to delete      
      temp = ptr->next;			// record address of next node                 
      delete ptr;			// free this node                             
      ptr = temp;			// point to next node to be deleted           
   }
}


void displayNode( struct node *ptr ) {
	cout << ptr->id << ": " << ptr->name << "\n";
}


void displayList( struct node *ptr ) {
   while( ptr != nullptr )           {
      displayNode( ptr );           
      ptr = ptr->next;            
   }
}



#include <iostream>
using namespace std;

int main()
{
   	char* name;
   	int id, ch = 1;
   	struct node *ptr;

	// add
	ptr = initNode( "s1", 1 );
	addNode(ptr);
	ptr = initNode( "s2", 2 );
	addNode(ptr);
	ptr = initNode( "s3", 3 );
	addNode(ptr);
	ptr = initNode( "s4", 4 );
	addNode(ptr);
	ptr = initNode( "s5", 5 );
	addNode(ptr); 
	displayList(head);

	// delete
	name = "s2";
	ptr = searchName(head, name );
	if( ptr == nullptr ) {
		cout << "\nName: " << name << " not found" << "\n";
	}
	else {
		cout << "\nDeleting a node ...  ";
	    displayNode(ptr);
		deleteNode( ptr );
	}
	displayList(head);

	// insert
	name = "s2";
	id = 2;
	ptr = initNode( name, id );
	insertNode( ptr );
	cout << "\nInserting a node ...  ";
	displayNode(ptr);
	displayList(head);

	// reverse
	cout << "\nReversing the list ...  \n";
	reverse();
	displayList(head);

	// delete
	cout << "\nIn the end, deleting the list ...  \n";
	deleteList(head);
	displayList(head);
	return 0;
}
