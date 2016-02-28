 // Iterators - Insertion and Removal
 // iterator_list.cpp

 #include <list>
 #include <iostream>

 int main() {
     std::list<double> prices;  // initially empty

     prices.push_back(10.5);   // add 10.5
     prices.push_back(20.5);   // add 20.5
     prices.push_back(30.5);   // add 30.5
     for(auto e : prices) std::cout << e << " "; std::cout << "\n";   // GB

     prices.insert(--prices.end(), 12.5); // same as push_back
     for(auto e : prices) std::cout << e << " "; std::cout << "\n";   // GB

     prices.erase(++prices.begin()); // deletes the second entry
     for(auto e : prices) std::cout << e << " "; std::cout << "\n";   // GB

     for(auto i = prices.begin(); i != prices.end(); i++)
         std::cout << *i << "  ";
     std::cout << "\n";
 }
