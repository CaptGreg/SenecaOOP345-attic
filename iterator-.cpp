 // Iterators - Vectors
 // iterator.cpp

 #include <vector>
 #include <iostream>

 int main() {
     std::vector<double> prices;  // initially empty
     std::vector<double>::iterator iter;

     prices.push_back(10.); // add 10.
     prices.push_back(20.); // add 20.
     prices.push_back(32.); // add 32.

     std::cout << "Forward: ";                            // GB
     for(iter = prices.begin(); iter != prices.end(); iter++)      // GB
         std::cout << *iter << "  ";                         // GB
     std::cout << "\n";                                   // GB

     std::cout << "Reverse but DO NOT PRINT FIRST ENTRY!: ";// GB
     for(iter = prices.end() - 1; iter != prices.begin(); iter--)
         std::cout << *iter << "  ";
     std::cout << "\n";
 }
