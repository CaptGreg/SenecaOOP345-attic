#include <utility>      // std::pair, std::make_pair
#include <cstddef>      // size_t
#include <vector>
#include <algorithm>    // find
#include <iostream>

struct VEC : std::vector<int> {
  VEC()                          : std::vector<int> ()               {}
  VEC(size_t s)                  : std::vector<int> (s)              {}
  VEC(size_t s,int initialValue) : std::vector<int> (s,initialValue) {}
};

int val = 99;
const size_t s = 10000;
VEC V1;
VEC V2(s);
VEC V3(s, val);

int main()
{
  V2[1000] = V3[2000];
  V2[5000] = V3[4000];
  V2[9999] = V3[6000];

  for(auto i = V2.begin(); (i=find(i, V2.end(), val)) != V2.end(); ++i) {
    std::cout << "for-loop found '" << val << "' at offset " << std::distance(V2.begin(), i) << "\n";
  }
// pair::pair example
// http://www.cplusplus.com/reference/utility/pair/pair/

  std::pair <std::string,double> product1;                     // default constructor
  std::pair <std::string,double> product2 ("tomatoes",2.30);   // value init
  std::pair <std::string,double> product3 (product2);          // copy constructor

  product1 = std::make_pair(std::string("lightbulbs"),0.99);   // using make_pair (move)

  product2.first = "shoes";                  // the type of first is string
  product2.second = 39.90;                   // the type of second is double

  std::cout << "The price of " << product1.first << " is $" << product1.second << '\n';
  std::cout << "The price of " << product2.first << " is $" << product2.second << '\n';
  std::cout << "The price of " << product3.first << " is $" << product3.second << '\n';

  // try it again with derived struct

  struct P : std::pair <std::string,double> {
     P()                       : std::pair <std::string,double> ()    {}
     P(std::string s,double d) : std::pair <std::string,double> (s,d) {}
     P(const P& rhs) { *this = rhs; }     // copy ctor
     const P& operator=(const P& rhs) {   // assignment operator
       if(this != &rhs) {
         this->first  = rhs.first; 
	 this->second = rhs.second;
       } 
       return *this;
     }
     const P& operator=(const P::pair& rhs) {   // assignment operator
       if(this != &rhs) {
         this->first  = rhs.first; 
	 this->second = rhs.second;
       } 
       return *this;
       
     }
  };

  P p1;                     // default constructor
  std::cout << "struct p1 The price of " << p1.first << " is $" << p1.second << '\n';

  P p2 ("tomatoes",2.30);   // value init
  std::cout << "struct p2 The price of " << p2.first << " is $" << p2.second << '\n';

  P p3 (p2);                // copy constructor
  std::cout << "struct p3 The price of " << p3.first << " is $" << p3.second << '\n';

  p1 = std::make_pair(std::string("lightbulbs"),0.99);   // using make_pair (move)
  std::cout << "struct p1 The price of " << p2.first << " is $" << p2.second << '\n';

  p2.first = "shoes";                  // the type of first is string
  p2.second = 39.90;                   // the type of second is double
  std::cout << "struct p2 The price of " << p2.first << " is $" << p2.second << '\n';

  std::cout << "struct p1 The price of " << p1.first << " is $" << p1.second << '\n';
  std::cout << "struct p2 The price of " << p2.first << " is $" << p2.second << '\n';
  std::cout << "struct p3 The price of " << p3.first << " is $" << p3.second << '\n';

  return 0;
}
