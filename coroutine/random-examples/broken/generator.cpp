// https://www.modernescpp.com/index.php/multithreading-in-c-17-and-c-20

// g++-10 -fcoroutines -std=c++2a generator.cpp -o generator -ltbb -pthread && ./generator

#include <iostream>
#include <coroutine>

/*
The generator function's return value generator<double> is an iterator for the type of its template argument. 
In this programming example it is using the template class coro_exp::generator<> which the implementation of is provided. 
However, the C++20 standard is specifying std::generator<>1, but it is not available in the Clang++ (9.0.0) experimental implementation yet.
*/

#if 0
std::generator<int> getInts(int first, int last) // g++ error: ‘generator’ does not name a type
{
  for (auto i= first; i <= last; ++i) {
    co_yield i;
  }
}

int main(int argc,char*argv[])
{
  for (auto i: getInts(5, 10)){
    std::cout << i << " ";                      // 5 6 7 8 9 10
}
#else
// std::generate<int> genForNumbers(int begin, int inc= 1) // error: ‘std::generate’ in namespace ‘std’ does not name a template type
std::generator<int> genForNumbers(int begin, int inc= 1) // error: ‘generator’ in namespace ‘std’ does not name a template type; did you mean ‘generate’?  
{ 
  for(int i= begin;; i += inc) {    
    co_yield i;
  }  
}

int main(int argc,char*argv[])
{  
  auto numbers= genForNumbers(-10);    
  for(int i= 1; i <= 20; ++i) 
    std::cout<< numbers<< " ";    
  for(auto n: genForNumbers(0,5)) 
    std::cout<< n << " ";  
}
#endif
