// C++11 code from
// https://gist.github.com/pfultz2/d0eea79bec9f7353339a
// http://ideone.com/zTzwIL shows how to use it with boost.  GB adopted to C++11 template

// referenced from
// https://isocpp.org/blog/2016/02/selecting-by-interface-an-idea-almost-to-good-to-be-true
// https://meetingcpp.com/index.php/br/items/selecting-by-interface-an-idea-almost-to-good-to-be-true.html

// see http://ideone.com/zTzwIL for sample use

#include <string>
#include <iostream>
#include <utility> // std::declval

// Update - how to do it in C++11 only
// Paul Fultz II pointed out at twitter, that decltype and declval can do the same, 
// simply by using auto and return type deduction:

////////////////////////////////////////////////////////////////////////////////
// http://en.cppreference.com/w/cpp/language/decltype 

// decltype(arg) function only has a single argument.
// comma operator ignores additional arguments

// http://www.cplusplus.com/reference/utility/declval/
// std::declval
// template <class T>
//   typename add_rvalue_reference<T>::type declval() noexcept;

// Declaration value
// Returns an rvalue reference to type T without referring to any object.
// This function shall only be used in unevaluated operands (such as the 
// operands of sizeof and decltype).
// T may be an incomplete type.
// This is a helper function used to refer to members of a class in unevaluated 
// operands, especially when either the constructor signature is unknown or when 
// no objects of that type can be constructed (such as for abstract base classes).

// Parameters none
// Return value An rvalue reference to type T.
// The return type uses add_rvalue_reference to return T&& when T is an object type.
////////////////////////////////////////////////////////////////////////////////
// http://www.cplusplus.com/reference/utility/declval/
// function template
// <utility>
// std::declval

// template <class T>
//   typename add_rvalue_reference<T>::type declval() noexcept;

// Declaration value
// Returns an rvalue reference to type T without referring to any object.

// This function shall only be used in unevaluated operands (such as the operands of sizeof and decltype).

// T may be an incomplete type.

// This is a helper function used to refer to members of a class in unevaluated operands, especially when either the constructor signature is unknown or when no objects of that type can be constructed (such as for abstract base classes).

// Parameters none

// Return value An rvalue reference to type T.
// The return type uses add_rvalue_reference to return T&& when T is an object type.
////////////////////////////////////////////////////////////////////////////////

template<class Interface>
struct GenericInterface
{
  template<class T = Interface>
  // auto query_interface() const -> decltype(std::declval<T>().interface(), std::string()) // comma operator, second arg ignored
  auto query_interface() const -> decltype(std::declval<T>().interface())
  {
    return i.interface();
  }
  template<class T = Interface>
  // auto query_interface() const -> decltype(std::declval<T>().get_my_interface(), std::string()) // comma operator, second arg ignored
  auto query_interface() const -> decltype(std::declval<T>().get_my_interface())
  {
    return i.get_my_interface();
  }
  template<class T = Interface>
  // auto query_interface() const -> decltype(std::declval<T>().myInterface(), std::string()) // comma operator, second arg ignored
  auto query_interface() const -> decltype(std::declval<T>().myInterface())
  {
    return i.myInterface();
  }

  void print()
  {
    std::cout << query_interface() << "\n";
  }

  Interface i;
};

int main()
{
  // http://ideone.com/zTzwIL sample use
  struct Interface       { std::string interface()        const { return "interface"; } }; 
  struct my_interface    { std::string get_my_interface() const { return "get_my_interface"; } }; 
  struct myInterfaceType { std::string myInterface()      const { return "myInterface"; } };

  // need to match up above member functions with member functions in GenericInterface
  GenericInterface <Interface>       gInterface;
  GenericInterface <my_interface>    gmy_interface;
  GenericInterface <myInterfaceType> gmyInterface;

  std::cout << "calling gInterface.print()    --> "; 
  gInterface.print();

  std::cout << "calling gmy_interface.print() --> "; 
  gmy_interface.print();

  std::cout << "calling gmyInterface.print()  --> ";
  gmyInterface.print();

  return 0;
}
