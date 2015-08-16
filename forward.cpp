// http://en.cppreference.com/w/cpp/utility/forward

#include <iostream>
#include <memory>             // std::unique_ptr
#include <string>
#include <typeinfo>     // typeid
#define TYPEID(X) std::cout << "std::typeid(" #X ").name()=" << std::typeid(X).name() << "\n";
 
struct A {
    A(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
    A(int& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
};
 
class B {
public:
    template<class T1, class T2, class T3>
    B(T1&& t1, T2&& t2, T3&& t3) :
    // B(T1& t1, T2& t2, T3& t3) :   // fails!  std::forward must have a r-value
        a1_{std::forward<T1>(t1)},
        a2_{std::forward<T2>(t2)},
        a3_{std::forward<T3>(t3)}
    { }
 
private:
    A a1_, a2_, a3_;
};
 
template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u)));
}
 
template<class T, class... U>
std::unique_ptr<T> make_unique(U&&... u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}
 
// template<class T> void foo(const T t) { std::cout << "foo(const T t) " << t << "\n"; }  // confuses compiler
template<class T> void foo(const T& t) { std::cout << "foo(const T& t) "  << t << "\n"; }
template<class T> void foo(T& t)       { std::cout << "foo(T& t) "        << t << "\n"; }
template<class T> void foo(T&& t)      { std::cout << "foo(T&& t) "       << t << "\n"; }
template<class T> void foo(const T&& t){ std::cout << "foo(const T&& t) " << t << "\n"; }

template<class T>
void wrapper(T&& arg)
{
    foo(std::forward<T>(arg)); // Forward a single argument.
}

int main()
{   
    double trouble;
    // TYPEID(trouble); // not working ???
    auto p1 = make_unique1<A>(2); // rvalue
    int i = 1;
    auto p2 = make_unique1<A>(i); // lvalue
 
    std::cout << "B\n";
    auto t = make_unique<B>(2, i, 3);
    std::cout << "C\n";
    auto t2 = make_unique<B>(2, i, 2+3);
    std::cout << "D\n";
    auto t3 = make_unique<B>(2, i, i+3);
    std::cout << "E\n";
    i = 100;
    auto t4 = make_unique<B>(2, i, 2*i+3);

    std::cout << "F\n";

    std::string s("s");
    const std::string cs("cs");

    // If a call to wrapper() passes an rvalue std::string, 
    // then T is deduced to std::string (not std::string&, const std::string&, or std::string&&), 
    // and std::forward ensures that an rvalue reference is passed to foo.
    wrapper(std::move(s));

    // If a call to wrapper() passes a const lvalue std::string, 
    // then T is deduced to const std::string&, 
    // and std::forward ensures that a const lvalue reference is passed to foo.
    wrapper(cs);

    // If a call to wrapper() passes a non-const lvalue std::string, 
    // then T is deduced to std::string&, and std::forward ensures that a non-const lvalue reference is passed to foo. 
    wrapper(s);

    wrapper(s+cs);   // GB
    wrapper(cs+cs);  // GB

    std::cout << "G\n";
    // TYPEID(i)
    auto px = make_unique1<std::string>(cs); // compiles but nothing printed with or without 'auto px ='
    // std::cout << px << "\n";  // syntax error
    // TYPEID(px)
    auto py = make_unique1<std::string>(s);  // compiles but nothing printed with or without 'auto px ='
    // std::cout << py << "\n";  // syntax error
    // TYPEID(py)

}

