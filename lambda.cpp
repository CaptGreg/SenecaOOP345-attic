#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#if 0
// http://en.cppreference.com/w/cpp/language/lambda

Lambda functions (since C++11)

Constructs a closure: an unnamed function object capable of capturing variables in scope.
Syntax
[ capture ] ( params ) mutable(optional) exception attribute -> ret { body } (1)
[ capture ] ( params ) -> ret { body }                                       (2)
[ capture ] ( params ) { body }                                              (3)
[ capture ] { body }                                                         (4)

1) Full declaration

2) Declaration of a const lambda: the objects captured by copy cannot be modified.

3) Omitted trailing-return-type: the return type of the closure's operator() is
   deduced according to the following rules:

    if the body consists of the single return statement, the return type is the
    type of the returned expression (after rvalue-to-lvalue, array-to-pointer,
    or function-to-pointer implicit conversion)

    otherwise, the return type is void

    (until C++14)

    The return type is deduced from return statements.
    (since C++14)

4) Omitted parameter list: function takes no arguments, as if the parameter list was ()
Explanation
mutable   - allows body to modify the parameters captured by copy, and to call their non-const member functions
exception - provides the exception specification or the noexcept clause for operator() of the closure type
attribute - provides the attribute specification for operator() of the closure type
capture   - specifies which symbols visible in the scope where the function is declared will be visible inside the function body.

A list of symbols can be passed as follows:

    [a,&b] where a is captured by value and b is captured by reference.
    [this] captures the this pointer by value
    [&] captures all automatic variables mentioned in the body of the lambda by reference
    [=] captures all automatic variables mentioned in the body of the lambda by value
    [] captures nothing

params - The list of parameters, as in named functions
ret    - Return type. If not present it's implied by the function return statements ( or void if it doesn't return any value)
body   - Function body


The lambda expression constructs an unnamed temporary object of unique unnamed non-union non-aggregate type, known as closure type, which has the following members:
ClosureType::operator()(params)

ret operator()(params) const { body }
(the keyword mutable was not used)

ret operator()(params) { body }
(the keyword mutable was used)


Executes the body of the lambda-expression, when invoked. When accessing a variable, accesses its captured copy (for the entities captured by copy), or the original object (for the entities captured by reference). Unless the keyword mutable was used in the lambda-expression, the objects that were captured by copy are non-modifiable from inside this operator().
Dangling references

If an entity is captured by reference, implicitly or explicitly, and the function call operator of the closure object is invoked after the entity's lifetime has ended, undefined behavior occurs. The C++ closures do not extend the lifetimes of the captured references.
ClosureType::operator ret(*)(params)()

typedef ret(*F)(params);
operator F() const;

This member function is only defined if the capture list of the lambda-expression is empty.

The value returned by this conversion function is a function pointer that, when invoked, has the same effect as invoking the closure object's function call operator directly.
ClosureType::ClosureType()

ClosureType() = delete;

ClosureType(const ClosureType& ) = default;

ClosureType(ClosureType&& ) = default;

Closure types are not DefaultConstructible. The copy constructor and the move constructor are implicitly-declared and may be implicitly-defined according to the usual rules for implicit copy constructors and move constructors.
ClosureType::operator=(const ClosureType&)

ClosureType& operator=(const ClosureType&) = delete;


Closure types are not CopyAssignable.
ClosureType::~ClosureType()

~ClosureType() = default;

The destructor is implicitly-declared.
ClosureType::CapturedParam

T1 a;

T2 b;
...


If the lambda-expression captures anything by copy (either implicitly with capture clause [=] or explicitly with a capture that does not include the character &, e.g. [a, b, c]), the closure type includes unnamed non-static data members, declared in unspecified order, that hold copies of all entities that were so captured.

The type of each data member is the type of the corresponding captured entity, except if the entity has reference type (in that case, references to functions are captured as-is, and references to objects are captured as copies of the referenced objects).

For the entities that are captured by reference (with the default capture [&] or when using the character &, e.g. [&a, &b, &c]), it is unspecified if additional data members are declared in the closure type.

(This section is incomplete)
Reason: scope rules, capture list rules, nested lambdas, implicit capture vs odr use, decltype
Example

This example shows (a) how to pass a lambda to a generic algorithm and (b) how objects resulting from a lambda declaration can be stored in function objects.
Run this code

#endif




// http://www.cprogramming.com/c++11/c++11-lambda-closures.html

// One of the most exciting features of C++11 is ability to create lambda functions
// (sometimes referred to as closures). What does this mean? A lambda function is a
// function that you can write inline in your source code (usually to pass in to another
// function, similar to the idea of a functor or function pointer). With lambda, creating
// quick functions has become much easier, and this means that not only can you start
// using lambda when you'd previously have needed to write a separate named function,
// but you can start writing more code that relies on the ability to create quick-and-easy
// functions. In this article, I'll first explain why lambda is great--with some
// examples--and then I'll walk through all of the details of what you can do with lambda.

// Why Lambdas Rock

// Imagine that you had an address book class, and you want to be able to provide a search
// function. You might provide a simple search function, taking a string and returning all
// addresses that match the string. Sometimes that's what users of the class will want. But
// what if they want to search only in the domain name or, more likely, only in the username
// and ignore results in the domain name? Or maybe they want to search for all email addresses
// that also show up in another list. There are a lot of potentially interesting things to
// search for. Instead of building all of these options into the class, wouldn't it be nice
// to provide a generic "find" method that takes a procedure for deciding if an email address
// is interesting? Let's call the method findMatchingAddresses, and have it take a "function"
// or "function-like" object.

class Foo
{
public:
    Foo () : _x( 3 ) {}
    void func ()
    {
        // a very silly, but illustrative way of printing out the value of _x
        [this] () { cout << _x; } ();
    }

private:
        int _x;
};

int main1()
{
    Foo f;
    f.func(); // prints 3
    cout << "\n";
    return 0;
} // main1

int main2()
{

    [] { cout << "Hi there, my friends\n"; }(); // prints as expected

    auto i = [] () { return 42; }; // compiler knows this returns an integer
    cout << i << "\n";  // prints 1 --- ???

    auto j = []    { return 43; }; // compiler knows this returns an integer
    cout << j << "\n";  // prints 1 --- ???

    auto k = []    { return 44; }; // compiler knows this returns an integer
    cout << k << "\n";  // prints 1 --- ???

    auto l = [] () -> int { return 45; }; // now we're telling the compiler what we want
    // l++; // error: no ‘operator++(int)’ declared for postfix ‘++’
    cout << l << "\n";  // prints 1 --- ???

    // replace
    vector<int> v1;
    v1.push_back( 10 );
    v1.push_back( 20 );
    //...
    for ( auto itr = v1.begin(), end = v1.end(); itr != end; itr++ )
    {
        cout << *itr << "\n";
    }

    // with
    vector<int> v2;
    v2.push_back( 30 );
    v2.push_back( 40 );
    //...
    for_each( v2.begin(), v2.end(), [] (int val) { cout << val << "\n"; } );

    return 0;
} // main2
// A Note About Function Pointers

// Under the final C++11 spec, if you have a lambda with an empty capture specification,
// then it can be treated like a regular function and assigned to a function pointer.
// Here's an example of using a function pointer with a capture-less lambda:

typedef int (*func)();
func f = [] () -> int { return 2; };
int main3()
{
  f();
  return 0;
} // main3

// This works because a lambda that doesn't have a capture group doesn't need its own class--it
// can be compiled to a regular old function, allowing it to be passed around just like a normal
// function. Unfortunately, support for this feature is not included in MSVC 10, as it was added
// to the standard too late.
// Making Delegates with Lambdas

// Let's look at one more example of a lambda function--this time to create a delegate. What's a
// delgate, you ask? When you call a normal function, all you need is the function itself. When you
// call a method on an object, you need two things: the function and the object itself. It's the
// difference between func() and obj.method(). To call a method, you need both. Just passing in the
// address of the method into a function isn't enough; you need to have an object to call the method on.

// Let's look at an example, starting with some code that again expects a function as an argument,
// into which we'll pass a delegate.

#include <functional>
#include <string>

class EmailProcessor
{
public:
    void receiveMessage (const string& message)
    {
        if ( _handler_func )
        {
            _handler_func( message );
        }
        // other processing
    }
    void setHandlerFunc (function<void (const string&)> handler_func)
    {
        _handler_func = handler_func;
    }

private:
        function<void (const string&)> _handler_func;
};

// This is a pretty standard pattern of allowing a callback function to be registered with a class
// when something interesting happens.

// But now let's say we want another class that is responsible for keeping track of the longest message
// received so far (why do you want to do this? Maybe you are a bored sysadmin). Anyway, we might create
// a little class for this:

#include <string>

class MessageSizeStore
{
public:
    MessageSizeStore () : _max_size( 0 ) {}
    void checkMessage (const string& message )
    {
        const int size = message.length();
        if ( size > _max_size )
        {
            _max_size = size;
        }
    }
    int getSize ()
    {
        return _max_size;
    }

private:
    int _max_size;
};

int main4()
{
// What if we want to have the method checkMessage called whenever a message arrives? We can't just pass
// in checkMessage itself--it's a method, so it needs an object.

// EmailProcessor processor;
// MessageSizeStore size_store;
// processor.setHandlerFunc( checkMessage ); // this won't work

// We need some way of binding the variable size_store into the function passed to setHandlerFunc. Hmm,
// sounds like a job for lambda!

    EmailProcessor processor;

    MessageSizeStore size_store;

    processor.setHandlerFunc(
        [&] (const string& message) { size_store.checkMessage( message ); }
);

// Isn't that cool? We are using the lambda function here as glue code, allowing us to pass a regular function
// into setHandlerFunc, while still making a call onto a method--creating a simple delegate in C++.

  return 0;
} // main4

int main5()
{
    vector<int> c { 1,2,3,4,5,6,7 };
    cout << "c: "; for (auto i: c) { cout << i << ' '; } cout << '\n';

    int x = 5;
    c.erase(remove_if(c.begin(), c.end(), [x](int n) { return n < x; } ), c.end());

    cout << "c: "; for (auto i: c) { cout << i << ' '; } cout << '\n';

    // the type of a closure cannot be named, but can be inferred with auto
    auto func1 = [](int i) { return i+4; };
    cout << "func1(0): " << func1(0) << '\n';
    cout << "func1(1): " << func1(1) << '\n';
    cout << "func1(6): " << func1(6) << '\n';
    cout << "func1(100): " << func1(100) << '\n';
    cout << "func1(100.): " << func1(100.) << '\n';
    cout << "func1(100.f): " << func1(100.f) << '\n';
    cout << "func1(100L): " << func1(100L) << '\n';

    // like all callable objects, closures can be captured in function
    // (this may incur unnecessary overhead)
    function<int(int)> func2 = [](int i) { return i+4; };
    cout << "func2: " << func2(6) << '\n';

    return 0;  // GB fixed c dtor segfault
} // main5

// http://www.cprogramming.com/c++11/c++11-lambda-closures.html
#include <string>
#include <vector>

class AddressBook
{
    public:
    // using a template allows us to ignore the differences between functors, function pointers 
    // and lambda
    template<typename Func>
    // std::vector<std::string> findMatchingAddresses (Func func)
    std::vector<std::string> find (Func func)
    { 
        std::vector<std::string> results;
        for ( auto itr = _addresses.begin(), end = _addresses.end(); itr != end; ++itr )
        {
            // call the function passed into findMatchingAddresses and see if it matches
            if ( func( *itr ) )
            {
                results.push_back( *itr );
            }
        }
        return results;
    }

    private:
    std::vector<std::string> _addresses;
};


AddressBook global_address_book;

vector<string> findAddressesFromOrgs ()
{
    // change 'findMatchingAddress' to 'find'
    // return global_address_book.findMatchingAddresses
    return global_address_book.find
    ( 
        // we're declaring a lambda here; the [] signals the start
        [] (const string& addr) { return addr.find( ".org" ) != string::npos; } 
    );
}

int main6()
{
#if 0
  // read in the name from a user, which we want to search

  cout << " enter name";
  string name;
  cin>> name;

 
  // change 'findMatchingAddress' to 'find'
  // notice that the lambda function uses the the variable 'name'
  // global_address_book.findMatchingAddresses ( [&] (const string& addr) { return addr.find( name ) != string::npos; } );
  global_address_book.find ( [&] (const string& addr) { return addr.find( name ) != string::npos; } );
  //  error: ‘class AddressBook’ has no member named ‘find’
  //  global_address_book.find( [&] (const string& addr) { return addr.length() >= min_len; } );
  //                      ^

  int min_len = 0;
  cout << " enter min_len";
  cin >> min_len;

  // change 'findMatchingAddress' to 'find'
  global_address_book.find( [&] (const string& addr) { return addr.length() >= min_len; } );
  //  error: ‘class AddressBook’ has no member named ‘find’
  //  global_address_book.find( [&] (const string& addr) { return addr.length() >= min_len; } );
  //                      ^

  cout << "\n";
#endif
  return 0;
} // main6

// http://blog.smartbear.com/c-plus-plus/c11-tutorial-lambda-expressions-the-nuts-and-bolts-of-functional-programming/

// Note: According to the C++11 standard, implementations #include <initializer_list> implicitly when 
// necessary. Therefore, you’re not supposed to #include this header in your programs. However, certain 
// compilers (GCC 4.7 for example) aren’t fully compliant with the C++11 standard yet. Therefore, if 
// the new initialization notation causes cryptic compilation errors, add the directive 
// #include <initializer_list> to your code manually.
// #include <initializer_list> // did not help, errors galore
class accountant { public: char *name; double salary; };

int main7()
{
  vector <accountant> emps {{"Josh", 2100.0}, {"Kate", 2900.0}, {"Rose",1700.0}};

  const auto min_wage = 1600.0;

  const auto upper_limit = 1.5*min_wage;

  //report which accountant has a salary that is within a specific range

  // syntax errors galore
  // std::find_if(emps.begin(), emps.end(),
              // [=](const accountant& a)        {return a.salary()>=min_wage && a.salary() < upper_limit;});
              // [=](const accountant& a) ->bool {return a.salary()>=min_wage && a.salary() < upper_limit;});
  return 0;
} // main7

int main(int argc, char**argv)
{
  int ret = -1;

  try {
    cout <<"\nmain1\n"; main1();
    cout <<"\nmain2\n"; main2();
    cout <<"\nmain3\n"; main3();
    cout <<"\nmain4\n"; main4();
    cout <<"\nmain5\n"; main5();
    cout <<"\nmain5\n"; main5();
    cout <<"\nmain6\n"; main6();
    cout <<"\nmain7\n"; main7();
    ret =  0;
  } catch (exception& e) {
    cout << "standard exception " << e.what() << '\n';
  } catch (...) {
    cout << "default exception\n";
  }

  return ret;
}
