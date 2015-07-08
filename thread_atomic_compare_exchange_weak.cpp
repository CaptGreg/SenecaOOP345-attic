// http://www.cplusplus.com/reference/atomic/atomic/compare_exchange_weak/
#if 0

public member function
<atomic>
std::atomic::compare_exchange_weak
(1)	

bool compare_exchange_weak (T& expected, T val,
           memory_order sync = memory_order_seq_cst) volatile noexcept;
bool compare_exchange_weak (T& expected, T val,
           memory_order sync = memory_order_seq_cst) noexcept;

(2)	

bool compare_exchange_weak (T& expected, T val,
           memory_order success, memory_order failure) volatile noexcept;
bool compare_exchange_weak (T& expected, T val,
           memory_order success, memory_order failure) noexcept;

Compare and exchange contained value (weak)
Compares the contents of the atomic object's contained value with expected:
- if true, it replaces the contained value with val (like store).
- if false, it replaces expected with the contained value .

The function always accesses the contained value to read it, and -if the comparison is true- it then also replaces it. But the entire operation is atomic: the value cannot be modified by other threads between the instant its value is read and the moment it is replaced.

The memory order used in (2) depends on the result of the comparison: if true, it uses success; if false, it uses failure.

Note that this function compares directly the physical contents of the contained value with the contents of expected; This may result in failed comparisons for values that compare equal using operator== (if the underlying type has padding bits, trap values, or alternate representations of the same value), although this comparison shall converge rapidly in a loop that preserves expected.

Unlike compare_exchange_strong, this weak version is allowed to fail spuriously by returning false even when expected indeed compares equal to the contained object. This may be acceptable behavior for certain looping algorithms, and may lead to significantly better performance on some platforms. On these spurious failures, the function returns false while not modifying expected.

For non-looping algorithms, compare_exchange_strong is generally preferred.

Parameters

expected
    Reference to an object whose value is compared to the contained value, and which -in case it doesn't match- may be overwritten with the contained value.
    T is atomic's template parameter (the type of the contained value).
val
    Value to copy to the contained object in case expected matches the contained value.
    T is atomic's template parameter (the type of the contained value).
sync
    Synchronization mode for the operation.
    This can be any of the possible values of the enum type memory_order:
    value	memory order	description
    memory_order_relaxed	Relaxed	No synchronization of side effects.
    memory_order_consume	Consume	Synchronizes the visible side effects on values carrying dependencies from the last release or sequentially consistent operation.
    memory_order_acquire	Acquire	Synchronizes all visible side effects from the last release or sequentially consistent operation.
    memory_order_release	Release	Synchronizes side effects with the next consume or acquire operation.
    memory_order_acq_rel	Acquire/Release	Reads as an acquire operation and writes as a release operation (as described above).
    memory_order_seq_cst	Sequentially consistent	Synchronizes all visible side effects with the other sequentially consistent operations, following a single total order.
success
    Synchronization mode for the operation in case expected matches the contained value.
failure
    Synchronization mode for the operation in case expected does not match the contained value. This shall not be a stronger mode than success and shall neither be memory_order_release nor memory_order_acq_rel.


Return value
true if expected compares equal to the contained value (and does not fail spuriously).
false otherwise.

#endif

// atomic::compare_exchange_weak example:
#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread
#include <vector>         // std::vector

// a simple global linked list:
struct Node { int value; Node* next; };
std::atomic<Node*> list_head (nullptr);

void append (int val) {     // append an element to the list
  Node* oldHead = list_head;
  Node* newNode = new Node {val,oldHead};

  // what follows is equivalent to: list_head = newNode, but in a thread-safe way:
  while (!list_head.compare_exchange_weak(oldHead,newNode))
    newNode->next = oldHead;
}

int main ()
{
  // spawn 10 threads to fill the linked list:
  std::vector<std::thread> threads;
  for (int i=0; i<10; ++i) threads.push_back(std::thread(append,i));
  for (auto& th : threads) th.join();

  // print contents:
  for (Node* it = list_head; it!=nullptr; it=it->next)
    std::cout << ' ' << it->value;
  std::cout << '\n';

  // cleanup:
  Node* it; while ( (it=list_head) ) {list_head=it->next; delete it;}

  return 0;
}
