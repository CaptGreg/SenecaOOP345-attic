 // Iterators - Insertion and Removal
 // iterator_deque.cpp

 #include <deque>     // works
 #include <list>      // works
 #include <vector>    // works
 #include <array>  // does not work
 #include <iostream>

 int main() {
     // std::deque<float> p(3, 10.50);
     std::list<double> p(3, 10.50);
     // std::vector<long double> p(3, 10.50);
     // std::array<float> p(3, 10.50);            // does not work
     // std::array<float,3> p;            // does not work

     p.back() = 40.5;   // reset last
     p.erase(p.begin()); // remove first

     for(auto iter = p.begin(); iter != p.end(); iter++)
         std::cout << *iter << "  ";
     std::cout << "\n";

     p.insert(++p.begin(), 15.50);
     p.insert(p.end(), 20.50);

     for(auto iter = p.begin(); iter != p.end(); iter++)
         std::cout << "iterator-based for " << *iter << "  \n";

     for(auto e : p )
         std::cout << "range-based for " << e << "  \n";
     std::cout << "\n";
 }
