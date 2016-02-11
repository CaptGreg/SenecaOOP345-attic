#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;
 
template <class T>
void printContainer(T& container)
{
   for(typename T::iterator i = container.begin(); i != container.end(); i++) {
      cout << "container value of i = " << *i << "\n";
   }
}
 
template <class T>
void printContainer11(T& container)  // C++11 function template range based for compile error
{
   for(auto it = container.begin(); it != container.end(); it++) { // auto is C++ 11
      cout << "c++11 auto iterator container value of it = " << *it << "\n";
   }

   for(auto e : container)   // error
   {
      cout << "c++11 range-based for container value of e = " << e << "\n";
   }
}

int main()
{
   // create a vector to store double
   vector<double> vec; 
   vector<double>::iterator it;
   // vector<int> vec; 
   double i;

   // display the original size of vec
   cout << "vector size = " << vec.size() << "\n";

   // push 5 values into the vector
   for(i = 0; i < 5; i++){
      vec.push_back(i);
   }

   // display extended size of vec
   cout << "extended vector size = " << vec.size() << "\n";

   // access 5 values from the vector
   for(i = 0; i < 5; i++){
      cout << "value of vec [" << i << "] = " << vec[i] << "\n";
   }
   for(double d : vec) {
      cout << "c++11 range-based for loop value of vec " << d << "\n";
   }
   for(auto e : vec) {
      cout << "c++11 range-based for loop with auto value of vec " << e << "\n";
   }

   // use iterator to access the values
   it = vec.begin();
   while( it != vec.end()) {
      cout << "while loop: value of it = " << *it << "\n";
      it++;
   }
   for(it = vec.begin(); it != vec.end() ;  it++) {
      cout << "for loop: value of it = " << *it << "\n";
   }
   printContainer(vec);
   printContainer11(vec);

   list<string> l; 
   list<string>::iterator il;
   l.push_back( string("one") );
   l.push_back( string("two") );
   l.push_back( string("three") );
   l.push_back( string("four") );
   l.push_back( string("five") );

   cout << "list size = " << l.size() << "\n";

   for(il = l.begin(); il != l.end(); il++) {
      cout << "value of il = " << *il << "\n";
   }
   printContainer(l);
   printContainer11(l);
   return 0;
}
