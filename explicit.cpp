// http://en.cppreference.com/w/cpp/language/explicit
/*
explicit specifier
 
C++
 
C++ language
 
Classes
 

Specifies constructors and conversion operators (since C++11) that don't allow implicit conversions or copy-initialization.
Syntax
explicit class_name ( params ) 	(1) 	
explicit operator type ( ) (since C++11) 	(2) 	
1) specifies that this constructor is only considered for direct initialization (including explicit conversions);
2) specifies that this user-defined conversion function is only considered for direct initialization (including explicit conversions).
Notes

A constructor with a single non-default parameter (until C++11) that is declared without the function specifier explicit is called a converting constructor.

An explicit default constructor can be used to perform both default initialization and value initialization: the use of explicit with a default constructor has no effect unless it has default arguments that would allow it to be used in copy-initialization context.

Both constructors (other than copy/move) and user-defined conversion functions may be function templates; the meaning of explicit doesn't change.
Example
Run this code
*/

#include <iostream>

struct A
{
    A(int) { }      // converting constructor
    A(int, int) { } // converting constructor (C++11)
    operator int() const { return 0; }
};
 
struct B
{
    explicit B(int) { }
    explicit B(int, int) { }
    explicit operator int() const { return 0; }
};
 
int main()
{
    A a1 = 1;      // OK: copy-initialization selects A::A(int)
    A a2(2);       // OK: direct-initialization selects A::A(int)
    A a3 {4, 5};   // OK: direct-list-initialization selects A::A(int, int)
    A a4 = {4, 5}; // OK: copy-list-initialization selects A::A(int, int)
    int na1 = a1;  // OK: copy-initialization selects A::operator int()
    int na2 = static_cast<int>(a1); // OK: static_cast performs direct-initialization
    A a6 = (A)1.;  // OK: explicit cast performs static_cast
 
//  B b1 = 1;      // error: copy-initialization does not consider B::B(int)
    B b2(2);       // OK: direct-initialization selects B::B(int)
    B b3 {4, 5};   // OK: direct-list-initialization selects B::B(int, int)
//  B b4 = {4, 5}; // error: copy-list-initialization does not consider B::B(int,int)
//  int nb1 = b2;  // error: copy-initialization does not consider B::operator int()
    int nb2 = static_cast<int>(b2); // OK: static_cast performs direct-initialization
    B b5 = (B)1;   // OK: explicit cast performs static_cast
    B b6 = (B)1.;  // OK: explicit cast performs static_cast

  std::cout << "compiled and ran\n";
}
