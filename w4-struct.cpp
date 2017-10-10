 ////////////////////////////////////////////////////////////////////////////
 // This is an alternative solution to the key-value workshop, workshop 4, //
 // from https://cs.senecac.on.ca/~oop345/pages/workshops/w4.html          //
 // This uses a struct array.                                              //
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

template <typename K, typename V, int N>
class KVList {
  struct {
    K        k;
    V        v;
  } data[N];
  size_t   count = 0; // C++14
public:
    KVList() // default constructor - adopts a safe empty state
      : count(0) // C++98
    {
      count = 0; // C++98
    }

    size_t size() const // returns the number of entires in the key-value list
    {
      return count;
    }

    const K& key(int i) const // returns an unmodifiable reference to the key of element i in the list
    {
      static K def;
      if((size_t) i < count)
        return data[i].k;
      return def;
    }

    const V& value(int i) const // returns an unmodifiable reference to the value of element i in the list
    {
      static V def;
      if((size_t) i < count)
        return data[i].v;
      return def;
    }

    KVList& add(const K& kk, const V& vv) // adds a new element to the list if room exists and returns a reference to the current object, does nothing if no room exists
    {
      if(count < reinterpret_cast<int>(N)) {
        data[count].k = kk;
        data[count].v = vv;
        count++;
      }
      return *this;
    }

    int find(const K& kk) const // returns the index of the first element in the list with a key equal to k - defaults to 0
    {
      for(size_t i = 0; i < count; i++)
        if(kk == data[i].k)
          return i;
      return 0; // SPEC: defaults to 0
    }

    KVList& replace(int i, const K& kk, const V& vv) // replaces element i in the list with the key and value received and returns a reference to the current object
    {
    if((size_t)i < count) {
        data[i].k = kk;
        data[i].v = vv;
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
