// http://en.wikipedia.org/wiki/Template_metaprogramming

#include <iostream>
#include <future>
#include <functional>   // bind
using namespace std;

template <int n>
struct factorial {
	enum { value = n * factorial<n - 1>::value };
};
 
template <>
struct factorial<0> {
	enum { value = 1 };
};

int factorialInt(int n) { return n == 1? 1: n * factorialInt(n-1); }

template <class T> // function template
T factorialT(T n) { return n == 1? 1: n * factorialT<T>(n-1); }

int main(int argc, char**argv)
{
  // recursively expands the function at compile time
  cout << "factorial<0>::value = " << factorial<0>::value << "\n";
  cout << "factorial<4>::value = " << factorial<4>::value<< "\n";

  cout << "recursive function factorialInt(4) = " << factorialInt(4)<< "\n";

  // async of factorialInt works
  future<int> ffactorialInt = async(factorialInt, 4);
  cout << "async recursive function factorialInt(4) = " << ffactorialInt.get()<< "\n";

  cout << "function template recursive function factorialT<int>(4) = " << factorialT(4)<< "\n";

  // async of factorialT function template does not compile
  // future<int> ffactorialT = async(factorialT<int>, 4);
  // future<int> ffactorialT = async(factorialT, 4);
  future<int> ffactorialT = async([]()->int{return factorialT(4);});
  cout << "async recursive function factorialT(4) = " << ffactorialT.get()<< "\n";

  // auto bfactorialT =  bind(factorialT, 4);
  auto bfactorialT =  bind(factorialT<int>, 4);
  future<int> fbfactorialT = async(bfactorialT);
  cout << "async bind recursive function factorialT(4) = " << fbfactorialT.get()<< "\n";
}
