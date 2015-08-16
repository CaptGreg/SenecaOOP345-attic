// GB - try to make this compile, need a class X, headers, ...
#include <utility>       // std::move
#include <memory>        // std::shared_ptr
#include <type_traits>   // std::remove_reference
#include <iostream>

using namespace std;
template <class T> 
class X {
  T t;
public:
  X()             {}
  X(T tt) : t(tt) {}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// http://thbecker.net/articles/rvalue_references/section_07.html
//  Perfect Forwarding: The Problem
// The other problem besides move semantics that rvalue references were designed to solve is the perfect forwarding problem. Consider the following simple factory function:

template<typename T, typename Arg> 
shared_ptr<T> factory(Arg arg)
{ 
  return shared_ptr<T>(new T(arg));
} 

// Obviously, the intent here is to forward the argument arg from the factory function to T's constructor. Ideally, as far as arg is concerned, everything should behave just as if the factory function weren't there and the constructor were called directly in the client code: perfect forwarding. The code above fails miserably at that: it introduces an extra call by value, which is particularly bad if the constructor takes its argument by reference.

// The most common solution, chosen e.g. by boost::bind, is to let the outer function take the argument by reference:

template<typename T, typename Arg> 
shared_ptr<T> factory2(Arg& arg)
{ 
  return shared_ptr<T>(new T(arg));
} 

// That's better, but not perfect. The problem is that now, the factory function cannot be called on rvalues:

// factory2<X>(hoo()); // error if hoo returns by value
int& hoo() { int *p =  new int; *p =1; return *p; } // GB
// template <>
// factory2<int>(hoo()); // error if hoo returns by value // GB
// factory2<X>(41); // error

// This can be fixed by providing an overload which takes its argument by const reference:

template<typename T, typename Arg> 
shared_ptr<T> factory3(Arg const & arg)
{ 
  return shared_ptr<T>(new T(arg));
} 

#if 0
// There are two problems with this approach. Firstly, if factory had not one, but several arguments, you would have to provide overloads for all combinations of non-const and const reference for the various arguments. Thus, the solution scales extremely poorly to functions with several arguments.

// Secondly, this kind of forwarding is less than perfect because it blocks out move semantics: the argument of the constructor of T in the body of factory is an lvalue. Therefore, move semantics can never happen even if it would without the wrapping function.
// It turns out that rvalue references can be used to solve both these problems. They make it possible to achieve truly perfect forwarding without the use of overloads. In order to understand how, we need to look at two more rules for rvalue references. 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// http://thbecker.net/articles/rvalue_references/section_08.html

 // Perfect Forwarding: The Solution
// The first of the remaining two rules for rvalue references affects old-style lvalue references as well. Recall that in pre-11 C++, it was not allowed to take a reference to a reference: something like A& & would cause a compile error. C++11, by contrast, introduces the following reference collapsing rules1:
// 
    // A& & becomes A&
    // A& && becomes A&
    // A&& & becomes A&
    // A&& && becomes A&&

// Secondly, there is a special template argument deduction rule for function templates that take an argument by rvalue reference to a template argument:

template<typename T>
void foo(T&&);

// Here, the following apply:

    // When foo is called on an lvalue of type A, then T resolves to A& and hence, by the reference collapsing rules above, the argument type effectively becomes A&.
    // When foo is called on an rvalue of type A, then T resolves to A, and hence the argument type becomes A&&. 

// Given these rules, we can now use rvalue references to solve the perfect forwarding problem as set forth in the previous section. Here's what the solution looks like:
// 
template<typename T, typename Arg> 
shared_ptr<T> factory(Arg&& arg)
{ 
  return shared_ptr<T>(new T(std::forward<Arg>(arg)));
} 

// where std::forward is defined as follows:

template<class S>
S&& forward(typename remove_reference<S>::type& a) noexcept
{
  return static_cast<S&&>(a);
} 

// (Don't pay attention to the noexcept keyword for now. It lets the compiler know, for certain optimization purposes, that this function will never throw an exception. We'll come back to it in Section 9.) To see how the code above achieves perfect forwarding, we will discuss separately what happens when our factory function gets called on lvalues and rvalues. Let A and X be types. Suppose first that factory<A> is called on an lvalue of type X:

X x;
factory<A>(x);

// Then, by the special template deduction rule stated above, factory's template argument Arg resolves to X&. Therefore, the compiler will create the following instantiations of factory and std::forward:

shared_ptr<A> factory(X& && arg)
{ 
  return shared_ptr<A>(new A(std::forward<X&>(arg)));
} 

X& && forward(remove_reference<X&>::type& a) noexcept
{
  return static_cast<X& &&>(a);
} 

// After evaluating the remove_reference and applying the reference collapsing rules, this becomes:

shared_ptr<A> factory(X& arg)
{ 
  return shared_ptr<A>(new A(std::forward<X&>(arg)));
} 

X& std::forward(X& a) 
{
  return static_cast<X&>(a);
} 

// This is certainly perfect forwarding for lvalues: the argument arg of the factory function gets passed on to A's constructor through two levels of indirection, both by old-fashioned lvalue reference.

// Next, suppose that factory<A> is called on an rvalue of type X:

X foo();
factory<A>(foo());

// Then, again by the special template deduction rule stated above, factory's template argument Arg resolves to X. Therefore, the compiler will now create the following function template instantiations:

shared_ptr<A> factory(X&& arg)
{ 
  return shared_ptr<A>(new A(std::forward<X>(arg)));
} 

X&& forward(X& a) noexcept
{
  return static_cast<X&&>(a);
} 

// This is indeed perfect forwarding for rvalues: the argument of the factory function gets passed on to A's constructor through two levels of indirection, both by reference. Moreover, A's constructor sees as its argument an expression that is declared as an rvalue reference and does not have a name. By the no-name rule, such a thing is an rvalue. Therefore, A's constructor gets called on an rvalue. This means that the forwarding preserves any move semantics that would have taken place if the factory wrapper were not present.

// It is perhaps worth noting that the preservation of move semantics is in fact the only purpose of std::forward in this context. Without the use of std::forward, everything would work quite nicely, except that A's constructor would always see as its argument something that has a name, and such a thing is an lvalue. Another way of putting this is to say that std::forward's purpose is to forward the information whether at the call site, the wrapper saw an lvalue or an rvalue.

// If you want to dig a little deeper for extra credit, ask yourself this question: why is the remove_reference in the definition of std::forward needed? The answer is, it is not really needed at all. If you use just S& instead of remove_reference<S>::type& in the defintion of std::forward, you can repeat the case distinction above to convince yourself that perfect forwarding still works just fine. However, it works fine only as long as we explicitly specify Arg as the template argument of std::forward. The purpose of the remove_reference in the definition of std::forward is to force us to do so.

// Rejoice. We're almost done. It only remains to look at the implementation of std::move. Remember, the purpose of std::move is to pass its argument right through by reference and make it bind like an rvalue. Here's the implementation:

template<class T> 
typename remove_reference<T>::type&&
std::move(T&& a) noexcept
{
  typedef typename remove_reference<T>::type&& RvalRef;
  return static_cast<RvalRef>(a);
} 

// Suppose that we call std::move on an lvalue of type X:

X<int> x;
X<int> xx = std::move(x);

// By the new special template deduction rule, the template argument T will resolve to X&. Therefore, what the compiler ends up instantiating is

typename remove_reference<X&>::type&&
std::move(X& && a) noexcept
{
  typedef typename remove_reference<X&>::type&& RvalRef;
  return static_cast<RvalRef>(a);
} 

// After evaluating the remove_reference and applying the new reference collapsing rules, this becomes

X&& std::move(X& a) noexcept
{
  return static_cast<X&&>(a);
} 

// That does the job: our lvalue x will bind to the lvalue reference that is the argument type, and the function passes it right through, turning it into an unnamed rvalue reference.

// I leave it to you to convince yourself that std::move actually works fine when called on an rvalue. But then you may want to skip that: why would anybody want to call std::move on an rvalue, when its only purpose is to turn things into rvalues? Also, you have probably noticed by now that instead of

std::move(x);

// you could just as well write

static_cast<X&&>(x);

// However, std::move is strongly preferred because it is more expressive. 

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// http://en.cppreference.com/w/cpp/types/remove_reference
template<class T1, class T2>
void print_is_same() {
  std::cout << std::is_same<T1, T2>() << '\n';
}
 
int main() {
  std::cout << std::boolalpha;
 
  print_is_same<int, int>();    // true
  print_is_same<int, int &>();  // false
  print_is_same<int, int &&>(); // false
 
  print_is_same<int, std::remove_reference<int>::type>();    // true
  print_is_same<int, std::remove_reference<int &>::type>();  // true
  print_is_same<int, std::remove_reference<int &&>::type>(); // true
}
