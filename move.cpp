// http://en.cppreference.com/w/cpp/language/move_operator

#include <iostream>
#include <utility>
#include <vector>
#include <string> 

struct A {
    std::string s;
    A() : s("test") {}
    A(const A& o) : s(o.s) { std::cout << "move failed!\n";}
    A(A&& o) : s(std::move(o.s)) { std::cout << "move constructor\n"; }
    A& operator=(const A&) { std::cout << "copy assigned\n"; return *this; }
    A& operator=(A&& other) {
         s = std::move(other.s);
         std::cout << "move assigned\n";
         return *this;
    }
};
 
A f(A a) { return a; }
 
struct B : A {
     std::string s2; 
     int n;
     // implicit move assignment operator B& B::operator=(B&&)
     // calls A's move assignment operator
     // calls s2's move assignment operator
     // and makes a bitwise copy of n
};
 
struct C : B {
    ~C() {}; // destructor prevents implicit move assignment
};
 
struct D : B {
    D() {}
    ~D() {}; // destructor would prevent implicit move assignment
    D& operator=(D&&) = default; // force a move assignment anyway 
};
 
void main1()
{
    // http://en.cppreference.com/w/cpp/utility/move

    std::cout << "\nmain1\n";

    std::string str = "Hello";
    std::vector<std::string> v;
 
    // uses the push_back(const T&) overload, which means 
    // we'll incur the cost of copying str
    std::cout << "Before copy, str is \"" << str << "\"\n";
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
 
    // uses the rvalue reference push_back(T&&) overload, 
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
 
    std::cout << "The contents of the vector are \"" << v[0]
                                         << "\", \"" << v[1] << "\"\n";
 
    // string move assignment operator is often implemented as swap,
    // in this case, the moved-from object is NOT empty
    std::string str2 = "Good-bye";
    std::cout << "Before move from str2, str2 = '" << str2 << "'\n";
    v[0] = std::move(str2);
    std::cout << "After move from str2, str2 = '" << str2 << "'\n";
}

void main2()
{
    std::cout << "\nmain2\n";

    A a1, a2;
    std::cout << "Trying to move-assign A from rvalue temporary\n";
    a1 = f(A()); // move-assignment from rvalue temporary
    std::cout << "Trying to move-assign A from xvalue\n";
    a2 = std::move(a1); // move-assignment from xvalue
 
    std::cout << "Trying to move-assign B\n";
    B b1, b2;
    std::cout << "Before move, b1.s = \"" << b1.s << "\"\n";
    b2 = std::move(b1); // calls implicit move assignment
    std::cout << "After move, b1.s = \"" << b1.s << "\"\n";
 
    std::cout << "Trying to move-assign C\n";
    C c1, c2;
    c2 = std::move(c1); // calls the copy assignment operator
 
    std::cout << "Trying to move-assign D\n";
    D d1, d2;
    d2 = std::move(d1);
}

int main()
{
    main1();
    main2();
    return 0;
}
