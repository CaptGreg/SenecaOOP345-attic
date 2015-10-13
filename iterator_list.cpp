 // Iterators - Insertion and Removal
 // iterator_list.cpp

 #include <list>
 #include <iostream>

 int main() {
     std::list<double> prices;  // initially empty

     prices.push_back(10.43);   // add 10.43
     prices.push_back(20.54);   // add 20.54
     prices.push_back(32.43);   // add 32.43
     for(auto e : prices) std::cout << e << " "; std::cout << "\n";   // GB

     prices.insert(--prices.end(), 12.52);
     for(auto e : prices) std::cout << e << " "; std::cout << "\n";   // GB

     prices.erase(++prices.begin());
     for(auto e : prices) std::cout << e << " "; std::cout << "\n";   // GB

     for(auto i = prices.begin(); i != prices.end(); i++)
         std::cout << *i << "  ";
     std::cout << std::endl;
 }
