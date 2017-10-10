 ////////////////////////////////////////////////////////////////////////////
 // This is an alternative solution to the key-value workshop, workshop 4, //
 // from https://cs.senecac.on.ca/~oop345/pages/workshops/w4.html          //
 // This uses a STL std::map.                                              //
 //                                                                        //
 // Read the three code examples in the following order:                   //
 // 1. w4-struct.cpp                                                       //
 // 2. w4-pair.cpp                                                         //
 // 3. w4-map.cpp                                                          //
 ////////////////////////////////////////////////////////////////////////////


// Workshop 4 - Templates
 // w4.cpp

 #include <iostream>
 #include <iomanip>
 #include <string>
 // #include "KVList.h"

#include <map>          // std::map
#include <iterator>     // std::advance, std::distance
#include <utility>      // std::pair, std::make_pair

// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/utility/make_pair/

// http://www.cplusplus.com/reference/map/map/erase/
// http://www.cplusplus.com/reference/map/map/find/
// http://www.cplusplus.com/reference/map/map/insert/
// http://www.cplusplus.com/reference/map/map/size/

// http://www.cplusplus.com/reference/iterator/advance/
// http://www.cplusplus.com/reference/iterator/distance/

// https://stackoverflow.com/documentation/c%2b%2b/681/stdmap#t=20170926121146872317

template <typename K, typename V, int N>
class KVList {
  std::map<K,V> data;
  size_t        count = 0; // C++14
public:
    KVList() // default constructor - adopts a safe empty state
      : count(0) // C++98
    {
      count = 0; // C++98
    }

    size_t size() const // returns the number of entires in the key-value list
    {
      // return data.size();
      return count;
    }

    const K& key(int i) const // returns an unmodifiable reference to the key of element i in the list
    {
      static K def;
      if((size_t) i < count) {
        auto it = data.begin();
        // http://www.cplusplus.com/reference/iterator/advance/
        std::advance (it,i);
        return it->first;
      }
      return def;
    }

    const V& value(int i) const // returns an unmodifiable reference to the value of element i in the list
    {
      static V def;
      if((size_t) i < count) {
        auto it = data.begin();
        // http://www.cplusplus.com/reference/iterator/advance/
        std::advance (it,i);
        return it->second;
      }
      return def;
    }

    KVList& add(const K& kk, const V& vv) // adds a new element to the list if room exists and returns a reference to the current object, does nothing if no room exists
    {
      if(count < N) {
     // http://www.cplusplus.com/reference/map/map/insert/
     // data.insert(std::make_pair(kk,vv));          // insert with make_pair
     // data.insert(std::pair <K,V> (kk,vv));        // insert with make_pair
     // data.insert( { kk, vv } );                   // insert with initializer list
        data[kk] = vv;                               // insert with [] operator
        count++;
      }
      return *this;
    }

    int find(const K& kk) const // returns the index of the first element in the list with a key equal to k - defaults to 0
    {
      auto it = data.find( kk );

      if(it == data.end())    // not found
        return 0;             // SPEC: defaults to 0 (if not found)

      // http://www.cplusplus.com/reference/iterator/distance/
      return std::distance(data.begin(), it);
    }

    KVList& replace(int i, const K& kk, const V& vv) // replaces element i in the list with the key and value received and returns a reference to the current object
    {
      if((size_t) i < count) {
     // http://www.cplusplus.com/reference/map/map/erase/

     // data.erase(kk);                              // erase by key

     // http://www.cplusplus.com/reference/map/map/find/
     // auto it = data.find( kk );                   // erase by iterator
     // if(it != data.end())
       // data.erase(it);

        // http://www.cplusplus.com/reference/iterator/advance/
        auto it = data.begin();                      // erase by position
        std::advance (it,i);
        data.erase( it );
        
     // http://www.cplusplus.com/reference/map/map/insert/
     // data.insert(std::make_pair(kk,vv));          // insert with make_pair
     // data.insert(std::pair <K,V> (kk,vv));        // insert with pair
     // data.insert( { kk, vv } );                   // insert with initializer list
        data[kk] = vv;                               // insert with [] operator
      }
      return *this;
    }
};

 template <typename K, typename V, int N>
 void display(const std::string& msg, const KVList<K, V, N>& list, int w) { 
     std::cout << msg;
     for (size_t /*int*/ i = 0; i < list.size(); i++)
         std::cout << std::setw(w) << list.key(i)
          << " : " << list.value(i) << std::endl;
 }

 int main(int argc, char** argv) {
     if (argc != 1) {
         std::cerr << argv[0] << ": too many arguments\n";
         return 1;
     }

   // int width;
     bool keepreading;
     std::cout << std::fixed << std::setprecision(2);

     std::cout << "\nInventory\n=========\n";
     KVList <std::string, double, 5> inventory;
     std::string str;
     double price;

     keepreading = true;
     do {
          std::cout << "Product : ";
          getline(std::cin, str);
          if (str.compare("quit") == 0) {
              keepreading = false;
          } else {
              std::cout << "Price : ";
              std::cin >> price;
              std::cin.ignore();
              inventory.add(str, price);
          }
     } while(keepreading);
     display("\nPrice List\n----------\n", inventory, 13);

     std::cout << "\nCorrections\n-----------\n";
     keepreading = true;
     do {
          std::cout << "Product : ";
          getline(std::cin, str);
          if (str.compare("quit") == 0) {
              keepreading = false;
          } else {
              int i = inventory.find(str);
              if (i != -1) {
                  std::cout << "Price : ";
                  std::cin >> price;
                  std::cin.ignore();
                  inventory.replace(i, str, price);
              }
          }
     } while(keepreading);
     display("\nPrice List\n----------\n", inventory, 13);

     std::cout << "\nGlossary\n========\n";
     KVList <std::string, std::string, 5> glossary;
     std::string key, definition;

     keepreading = true;
     do {
          std::cout << "Key : ";
          getline(std::cin, key);
          if (key.compare("quit") == 0) {
              keepreading = false;
          } else {
              std::cout << "Definition : ";
              getline(std::cin, definition);
              glossary.add(key, definition);
          }
     } while(keepreading);
     display("\nEntries\n-------\n", glossary, 5);
 }
