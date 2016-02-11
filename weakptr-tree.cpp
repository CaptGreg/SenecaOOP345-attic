// GB - embellished from:
// http://programmers.stackexchange.com/questions/274801/raw-weak-ptr-unique-ptr-shared-ptr-etc-how-to-choose-them-wisely

#include <vector>
#include <string>
#include <memory>   // *_ptr family
#include <memory.h>  // memcpy
#include <iostream>

class ObjNew { // simple rule-of-5 class
  size_t   bs;           // buffer size
  uint8_t* bp;           // buffer pointer

public:

  ObjNew(int s) : bs(s), bp(new uint8_t(bs))  // CTOR
    { std::cout <<  "ObjNew(" << bs << ")\n"; }

  ~ObjNew()                                   // DTOR
    { 
      std::cout << "~ObjNew(), bs=" << bs << "\n";
      delete [] bp; 
    }

  ObjNew(const ObjNew& rhs)                   // copy CTOR
    { 
      std::cout << "copy ctor rhs.bs=" << rhs.bs << "\n"; 
      bp = nullptr;                           // set up to use copy = operator
      *this = rhs;                            // call it
    }

  ObjNew& operator= (const ObjNew& rhs)       // copy = operator
    { 
      std::cout << "copy = operator bs,rhs.bs=" << bs << "," << rhs.bs << "\n";
      if(this!=&rhs) { 
        bs=rhs.bs; 
        delete [] bp;
        bp = bs? (uint8_t*) memcpy(new uint8_t(bs), rhs.bp, rhs.bs): nullptr; 
      }
      return *this;
    }

  ObjNew(ObjNew&& rhs)                        // move CTOR
    { 
      std::cout << "move ctor rhs.bs=" << rhs.bs << "\n"; 
      bp = nullptr;                          // set up to use move = operator
      *this = std::move(rhs);                // call it
    }

  ObjNew&& operator= (ObjNew&& rhs)           // move = operator
    { 
      std::cout << "move = operator bs,rhs.bs=" << bs << "," << rhs.bs << "\n";
      if(this!=&rhs) { 
        delete [] bp;
        bs = rhs.bs;
        bp = rhs.bp;
        rhs.bs = 0;
        rhs.bp = nullptr;
      }
      return std::move(*this);
    }

    void print(const std::string& l)           // print out buffers size
    {
      std::cout << l << ", bs=" << bs << "\n";
    }
};

void doWork ()
{
  std::cout << "doWork start\n";
  std::vector< std::shared_ptr<ObjNew> >  v;
  std::shared_ptr<ObjNew> p;

  v.push_back( std::shared_ptr<ObjNew> (new ObjNew(1e7)) );
  v.push_back( std::shared_ptr<ObjNew> (new ObjNew(2e7)) );

  v.front() -> print("v.front");
  v.back()  -> print("v.back");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";

  for(int i = 0; i < 1000; i++)
   v.push_back( v.front() );

  for(int i = 0; i < 1000; i++)
   v.push_back( v[1] );

  std::cout << "v.size()=" << v.size() << "\n";

  v.front() -> print("v.front");
  v.back()  -> print("v.back");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";

  // Same thing again, this time using a copy of the ptr
  p = v.front();
  p->print("v.front");
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  p = v.back();
  std::cout << "front use_count=" << v.front().use_count() << "\n";
  std::cout << "back  use_count=" << v.back ().use_count() << "\n";
  p->print("v.back");

  std::cout << "doWork finish\n";
}

// shared pointer
// shared_ptr<ObjNew> spObject;

// unique pointer:
// unique_ptr<ObjNew> upObject;

// weak pointer
// weak_ptr<ObjNew> wpObject;

// shared_ptr and weak_ptr

// unique_ptr<> is useful, to be sure, but there are cases where you want two 
// parts of your code base to be able to refer to the same object and copy the 
// pointer around, while still being guaranteed proper cleanup. For example, 
// a tree might look like this, when using

template<class T>
struct NodeA {
  T value;
  std::shared_ptr <NodeA<T>> left;
  std::shared_ptr <NodeA<T>> right;
};

// In this case, we can even hold on to multiple copies of a root node, and 
// the tree will be properly cleaned up when all copies of the root node are 
// destroyed.

// This works because each shared_ptr<> holds on to not only the pointer to 
// the object, but also a reference count of all of the shared_ptr<> objects 
// that refer to the same pointer. When a new one is created, the count goes 
// up. When one is destroyed, the count goes down. When the count reaches zero, 
// the pointer is deleted.

// So this introduces a problem: Double-linked structures end up with circular 
// references. Say we want to add a parent pointer to our tree Node:

template<class T>
struct NodeB {
  T value;
  std::shared_ptr <NodeB<T>> parent;
  std::shared_ptr <NodeB<T>> left;
  std::shared_ptr <NodeB<T>> right;
};


// Now, if we remove a Node, there's a cyclic reference to it. It'll never be 
// deleted because its reference count will never be zero.

// To solve this problem, you use a std::weak_ptr<>:

template<class T>
struct NodeC {
  T value;
  std::weak_ptr   <NodeC<T>> parent;
  std::shared_ptr <NodeC<T>> left;
  std::shared_ptr <NodeC<T>> right;
};


// Now, things will work correctly, and removing a node will not leave stuck 
// references to the parent node. It makes walking the tree a little more 
// complicated, however:

#if 0
NodeC< ObjNew > node;
std::shared_ptr< NodeC<ObjNew> > parent_of_this = node->parent.lock();

// This way, you can lock a reference to the node, and you have a reasonable 
// guarantee it won't disappear while you're working on it, since you're 
// holding on to a shared_ptr<> of it.
// make_shared and make_unique

// Now, there are some minor problems with shared_ptr<> and unique_ptr<> that 
// should be addressed. The following two lines have a problem:

foo_unique(std::unique_ptr<SomeKindofObject>(new SomeKindOfObject()), thrower());
foo_shared(std::shared_ptr<SomeKindofObject>(new SomeKindOfObject()), thrower());

// If thrower() throws an exception, both lines will leak memory. And more than 
// that, shared_ptr<> holds the reference count far away from the object it 
// points to and this can mean a second allocation). That's not usually desirable.

// C++11 provides std::make_shared<>() and C++14 provides std::make_unique<>() 
// to solve this problem:

foo_unique(std::make_unique<SomeKindofObject>(), thrower());
foo_shared(std::make_shared<SomeKindofObject>(), thrower());

// Now, in both cases, even if thrower() throws an exception, there will not be 
// a leak of memory. As a bonus, make_shared<>() has the opportunity to create 
// its reference count in the same memory space as its managed object, which 
// can both be faster and can save a few bytes of memory, while giving you an 
// exception safety guarantee!

#endif


// Notes about Qt

// It should be noted, however, that Qt, which must support pre-C++11 compilers, 
// has its own garbage-collection model: Many QObjects have a mechanism where 
// they will be destroyed properly without the need for the user to delete them.

// I do not know how QObjects will behave when managed by C++11 managed pointers, 
// so I can not say that shared_ptr<QDialog> is a good idea. I do not have enough 
// experience with Qt to say for sure, but I believe that Qt5 has been adjusted 
// for this use case.


int main()
{
  std::cout << "main:\n";

  std::cout << "calling doWork\n";
  doWork();
  std::cout << "calling returned\n";

  return 0;
}
