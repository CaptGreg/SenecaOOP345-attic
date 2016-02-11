// http://www.tutorialspoint.com/cplusplus/cpp_templates.htm

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

template <class T>
class Stack {
  private:
    vector<T> elems;     // elements

  public:
    void push(T const&);  // push element
    void pop();               // pop element
    T top() const;            // return top element
    bool empty() const{       // return true if empty.
        return elems.empty();
    }
};

template <class T>
void Stack<T>::push (T const& elem)
{
    // append copy of passed element
    elems.push_back(elem);
}

template <class T>
void Stack<T>::pop ()
{
    if (elems.empty()) {
        throw out_of_range("Stack<>::pop(): empty stack");
    }
    // remove last element
    elems.pop_back();
}

template <class T>
T Stack<T>::top () const
{
    if (elems.empty()) {
        throw out_of_range("Stack<>::top(): empty stack");
    }
    // return copy of last element
    return elems.back();
}

int main()
{
    try {
        // manipulate int stack
        Stack<int>         intStack;  // stack of ints
        intStack.push(7);
        intStack.push(8);
        intStack.push(9);
        cout << intStack.top() <<"\n";

        // manipulate string stack
        Stack<string> stringStack;    // stack of strings
        stringStack.push("hello");
        stringStack.push("goodbye");
        stringStack.push("nihao");
        stringStack.push("zaijian");
        cout << stringStack.top() << "\n";
        stringStack.pop();
        stringStack.pop();
    }
    catch (exception const& ex) {
        cerr << "Exception: " << ex.what() <<"\n";
        return -1;
    }
}
