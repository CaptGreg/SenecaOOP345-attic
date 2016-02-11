// GB motivated from http://stackoverflow.com/questions/27721052/c11-track-object-lifetime-in-lambda

// std::make_shared
// ++++++++++++++++
// template <class T, class... Args> shared_ptr<T> make_shared (Args&&... args);
// Allocates and constructs an object of type T passing args to its constructor, and returns an object 
// of type shared_ptr<T> that owns and stores a pointer to it (with a use count of 1).

// std::function
// +++++++++++++
// template <class T> function;     // undefined
// template <class Ret, class... Args> class function<Ret(Args...)>;
// 
// Function wrapper
// Class that can wrap any kind of callable element (such as functions and function objects) into a 
// copyable object, and whose type depends solely on its call signature (and not on the callable 
// element type itself).
// 
// An object of a function class instantiation can wrap any of the following kinds of callable objects: 
// a function, a function pointer, a pointer to member, or any kind of function object (i.e., an object 
// whose class defines operator(), including closures).
// 
// A decay copy of the wrapped callable object is stored internally by the object, which becomes the 
// function's target. The specific type of this target callable object is not needed in order to 
// instantiate the function wrapper class; only its call signature.
// 
// The function object can be copied and moved around, and can be used to directly invoke the callable 
// object with the specified call signature (see member operator()).
// 
// function objects can also be in a state with no target callable object. In this case they are known 
// as empty functions, and calling them throws a bad_function_call exception.
// 

#include <iostream>
#include <memory>
#include <string>

class Foo {
    std::string name;
    std::string scope;
public:
    Foo(const std::string& n) : name(n), scope("inscope")
      { std::cout <<  "Foo ctor: " << "name=" << name << " " << this << "\n"; }
    ~Foo() 
      { scope = "out-of-scope"; std::cout <<  "Foo dtor: " << "name=" << name << " " << this << "\n"; }

    std::function<void()> getName() { return [this]() { std::cout << scope << " name=" << name << " " << this << "\n"; }; }
};

class Bar {
    std::string name;
    std::string scope;
public:
    Bar(const std::string& n) : name(n)
      { std::cout <<  "Bar ctor: " << "name=" << name << " " << this << "\n"; }
    ~Bar() 
      { std::cout <<  "Bar dtor: " << "name=" << name << " " << this << "\n"; }

    std::function<void()> getName() { return [this]() { std::cout << "name=" << name << " " << this << "\n"; }; }
};

int main() {
    std::function<void()> f;

    {
        auto foo = std::make_shared<Foo>("{foo}");
        f = foo->getName();
        f();
    } // foo out-of-scope: f now points to something that does not exist!

    std::cout << "main: which Foo will f() call?\n";
    // f();  // segfaults as it should

    auto bar = std::make_shared<Bar>("bar");
    // auto boo = std::make_shared<Foo>("boo");

    bar->getName();
    std::cout << "main: which Foo will f() call?\n";
    std::cout << "main: f points to member function 'name' of deleted instance 'foo' of class 'Foo'\n";
    std::cout << "main: Bar bar created\n";
    std::cout << "main: Foo boo created\n";
    f();

    return 0;
}
