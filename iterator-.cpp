 // Iterators - Vectors
 // iterator.cpp

 #include <vector>
 #include <iostream>

 int main() {
     std::vector<double> prices;  // initially empty
     std::vector<double>::iterator i;

     prices.push_back(10.43); // add 10.43
     prices.push_back(20.54); // add 20.54
     prices.push_back(32.43); // add 32.43
     for(i = prices.end() - 1; i != prices.begin(); i--)
         std::cout << *i << "  ";
     std::cout << std::endl;
 }
