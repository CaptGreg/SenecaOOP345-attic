// http://www.yosefk.com/c++fqa/defective.html

#include <map>      // map+multimap
#include <string>
#include <iostream>

#if 0
Maps are associative containers that store elements formed by a combination of a key value and a mapped value, following a specific order.

In a map, the key values are generally used to sort and uniquely identify the elements, while the mapped values store the content associated to this key. The types of key and mapped value may differ, and are grouped together in member type value_type, which is a pair type combining both:

typedef pair<const Key, T> value_type;

Internally, the elements in a map are always sorted by its key following a specific strict weak ordering criterion indicated by its internal comparison object (of type Compare).

map containers are generally slower than unordered_map containers to access individual elements by their key, but they allow the direct iteration on subsets based on their order.

The mapped values in a map can be accessed directly by their corresponding key using the bracket operator ((operator[]).

Maps are typically implemented as binary search trees.
#endif

typedef std::map<std::string,std::string> StringToStringMap;

void printMap(const StringToStringMap& dict) 
{
  // for(StringToStringMap::iterator p=dict.begin(); p!=dict.end(); ++p) {
  // map.cpp: In function ‘void print(const StringToStringMap&)’:
// map.cpp:6:47: error: conversion from ‘std::map<std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char> >::const_iterator {aka std:
// :_Rb_tree_const_iterator<std::pair<const std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char> > >}’ to non-scalar type ‘std::ma
// p<std::__cxx11::basic_string<char>, std::__cxx11::basic_string<char> >::iterator {aka std::_Rb_tree_iterator<std::pair<const std::__cxx11::ba
// sic_string<char>, std::__cxx11::basic_string<char> > >}’ requested

  // C++11 to the rescue.
  // We can't figure out how to declare the itertor but the compiler can.
  // Let the compiler figure it out: Use auto or a range-based for loop.

  std::cout << "for(auto p=dict.begin(); p!=dict.end(); ++p)\n";
  for(auto p=dict.begin(); p!=dict.end(); ++p) {
    std::cout << "  " << p->first << " -> " << p->second << std::endl;
  }
  std::cout << "\n";

  std::cout << "for(auto e : dict)\n";
  for(auto e : dict) {
    std::cout << "  " << e.first << " -> " << e.second << std::endl;
  }
  std::cout << "\n";
}

void Map ()
{
 StringToStringMap dict;

 // use pair
 dict.insert( std::pair<std::string,std::string> (std::string("dog"), std::string("cat")));

 // use make_pair
 dict.insert( std::make_pair(std::string("fish"), std::string("water snake")));
 dict.insert( std::make_pair(std::string("fish"), std::string("sea snake")));
 dict.insert( std::make_pair(std::string("ardvark"), std::string("pig")));

 printMap(dict);
}

#if 0
Multiple-key map
Multimaps are associative containers that store elements formed by a combination of a key value and a mapped value, following a specific order, and where multiple elements can have equivalent keys.

In a multimap, the key values are generally used to sort and uniquely identify the elements, while the mapped values store the content associated to this key. The types of key and mapped value may differ, and are grouped together in member type value_type, which is a pair type combining both:

typedef pair<const Key, T> value_type;

Internally, the elements in a multimap are always sorted by its key following a specific strict weak ordering criterion indicated by its internal comparison object (of type Compare).

multimap containers are generally slower than unordered_multimap containers to access individual elements by their key, but they allow the direct iteration on subsets based on their order.

Multimaps are typically implemented as binary search trees.
#endif

typedef std::multimap<std::string,std::string> StringToStringMultiMap;

void printMultiMap(const StringToStringMultiMap& dict) 
{
  std::cout << "for(auto p=dict.begin(); p!=dict.end(); ++p)\n";
  for(auto p=dict.begin(); p!=dict.end(); ++p) {
    std::cout << "  " << p->first << " -> " << p->second << std::endl;
  }
  std::cout << "\n";

  std::cout << "for(auto e : dict)\n";
  for(auto e : dict) {
    std::cout << "  " << e.first << " -> " << e.second << std::endl;
  }
  std::cout << "\n";
}

void MultiMap ()
{
 StringToStringMultiMap dict;

 // use pair
 dict.insert( std::pair<std::string,std::string> (std::string("dog"), std::string("cat")));

 // use make_pair
 dict.insert( std::make_pair(std::string("fish"), std::string("water snake")));
 dict.insert( std::make_pair(std::string("fish"), std::string("sea snake")));
 dict.insert( std::make_pair(std::string("ardvark"), std::string("pig")));

 printMultiMap(dict);
}

int main()
{
  Map();
  MultiMap();

  return 0;
}
