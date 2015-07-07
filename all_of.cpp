// http://www.cplusplus.com/reference/algorithm/all_of/
// GB modified to illustrate iterators and bind

// all_of example
#include <iostream>     // std::cout
#include <algorithm>    // std::all_of
#include <array>        // std::array
#include <functional>   // std::bind
#include <typeinfo>     // typeid

#define TYPEID(X) std::cout << "typeid(" #X ").name()=" << std::typeid(X).name() << "\n";

int main () {
  std::array<int,8> foo = {3,5,7,11,13,17,19,23};

  std::cout << "Do it with std::all_of (a loop)\n";
  if ( std::all_of(foo.begin(), foo.end(), [](int i){return i%2;}) )
    std::cout << "All the elements are odd numbers.\n";

  std::cout << "Do it with std::all_of (a loop), printing each value(x) passed to f\n";
  auto f0 =  [](int x) -> bool {return x%2;};
  auto f =   [](int x) -> bool  {bool r  = x%2; std::cout <<"x= " << x << ",r=" << r << " "; return r;};
  if ( std::all_of(foo.begin(), foo.end(), f) )
    std::cout << "All the elements are odd numbers.\n";

  std::cout << "Now do it with a for loop\n";
  bool all_odd = true;
  for(auto it = foo.begin(); it != foo.end(); it++) {
    if(f(*it) == 0) {
      all_odd = false;
      break;
    }
  }
  if(all_odd)
    std::cout << "All the elements are odd numbers.\n";

  std::cout << "Now do it with a for loop using bind\n";
  all_odd = true;
  for(auto it = foo.begin(); it != foo.end(); it++) {
    auto b = std::bind(f,*it);
    if(b() == 0) {
      all_odd = false;
      break;
    }
  }
  if(all_odd)
    std::cout << "All the elements are odd numbers.\n";


  // TYPEID(std::all_of, foo.begin(), foo.end(), f);
  // TYPEID(std::bind(std::all_of, foo.begin(), foo.end(), f));  // syntax error

  std::cout << "bind... = "
            << "std::bind(std::all_of, foo.begin(), foo.end(), f)"
            // << std::bind(std::all_of, foo.begin(), foo.end(), f) // syntax error
            << " does not compile"
            << "\n";

  return 0;
}
