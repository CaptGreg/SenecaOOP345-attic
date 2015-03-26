// size_t is defined in at least iostream, cstddef, cstdlib, maybe others
// #include <iostream>  // need size_t
// #include <cstddef>  // need size_t
#include <cstdlib>  // need size_t

template <class T>
class StackArray {
  T*     s;           // stack
  T*     sp;          // stack pointer
  size_t _capacity;   // stack _capacity
public:
  StackArray(size_t capacity = 10) {
    s = sp = new T[capacity]; 
    _capacity = capacity; 
  }

  ~StackArray() {
    delete [] s; 
  }

  void push(T data) {
    if(size() < _capacity) { 
      *sp++ = data; 
    } else 
      throw "push: stack overflow (capacity exceeded)"; 
  }

  T pop() {
    if(size() > 0) { 
      return *--sp; 
    } else 
      throw "pop: stack underflow (pop on empty stack empty)";
  }

  T peek(size_t index) { // used to print stack
    if( index < size() ) {
      return sp[-index];
    } else
      throw "peek: stack underflow";
  }

  size_t size() { 
    return sp-s; 
  }

  size_t capacity() {
    return _capacity;
  }
};
