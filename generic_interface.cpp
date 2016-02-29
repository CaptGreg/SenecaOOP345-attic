// GB Feb 29, 2016
//    1. added function code
//    2. adopted boost code test  to C++11 environment
// C++11 code from
// https://gist.github.com/pfultz2/d0eea79bec9f7353339a
// http://ideone.com/zTzwIL shows how to use it with boost.  GB adopted to C++11 template

// referenced from
// https://isocpp.org/blog/2016/02/selecting-by-interface-an-idea-almost-to-good-to-be-true
// https://meetingcpp.com/index.php/br/items/selecting-by-interface-an-idea-almost-to-good-to-be-true.html

// see http://ideone.com/zTzwIL for sample use

#include <string>
#include <iostream>

// Update - how to do it in C++11 only
// Paul Fultz II pointed out at twitter, that decltype and declval can do the same, 
// simply by using auto and return type deduction:

template<class Interface>
struct GenericInterface
{
  template<class T = Interface>
  // auto query_interface() const -> decltype(std::declval<T>().interface(), std::string()) // comma operator, second arg ignored
  auto query_interface() const -> decltype( std::declval<T>().interface() )
  { return i.interface(); }

  template<class T = Interface>
  // auto query_interface() const -> decltype(std::declval<T>().get_my_interface(), std::string()) // comma operator, second arg ignored
  auto query_interface() const -> decltype( std::declval<T>().get_my_interface() )
  { return i.get_my_interface(); }

  template<class T = Interface>
  // auto query_interface() const -> decltype(std::declval<T>().myInterface(), std::string()) // comma operator, second arg ignored
  auto query_interface() const -> decltype( std::declval<T>().myInterface() )
  { return i.myInterface(); } 

  template<class T = Interface>  // GB function object, member function name is operator()
  auto query_interface() const -> decltype( std::declval<T>().operator() () )
  { return i(); } 

  void print() { std::cout << query_interface() << "\n"; } 
  
  void iSet(const Interface& ii) { i = ii; } // GB - used to update 'i'

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

  // GB function object
  std::cout << "\nFUNCTION OBJECTS\n";
  struct FO {  // a rule-of-three function object
    int i;

    FO() : i(0) { std::cout << "\nFO(), i=" << i << "\n"; }

    FO(int ii) : i(ii) { std::cout << "\nFO(" << ii << "), i=" << i << "\n"; }

    ~FO() { std::cout << "\n~FO(), i=" << i << "\n"; }

    FO& operator= (const FO& rhs) 
    { 
      std::cout << "\noperator= FO(const FO&), i=" << i << "," << rhs.i << "\n"; 
      if(this!=&rhs) { i = rhs.i; }
      return *this;
    }

    FO(const FO& rhs) { i=rhs.i; std::cout << "\nFO(const FO&), i=" << i << "\n"; }

    void Set(int ii) { i = ii; }

    std::string operator() ()      const { return std::to_string(i) + " function object operator()"; } 
  };

  GenericInterface <FO>             gfo;
  std::cout << "calling gfo.print()  --> ";
  gfo.print(); 

  FO mfo(99);
  GenericInterface <FO>             gmfo;
  std::cout << "gmfo.print()  --> ";
  gmfo.print(); 
  std::cout << "update the GenericInterface with a non-default function object.\n";
  gmfo.iSet(mfo);
  std::cout << "gmfo.print()  --> ";
  gmfo.print(); 
  std::cout << "change the function object value: call mfo.Set(123)\n";
  mfo.Set(123);
  std::cout << "gmfo.print()  --> ";
  gmfo.print(); 
  std::cout << "the gmfo instance has a copy of the old value\n";
  std::cout << "update the gmfo interface: call gmfo.iSet\n";
  gmfo.iSet(mfo);
  std::cout << "gmfo.print()  --> ";
  gmfo.print(); 

  return 0;
}
