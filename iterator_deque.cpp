 // Iterators - Insertion and Removal
 // iterator_deque.cpp

 #include <deque>     // works
 #include <list>      // works
 #include <vector>    // works
 // #include <array>  // does not work
 #include <iostream>

 int main() {
     // std::deque<float> p(3, 10.50);
     // std::list<float> p(3, 10.50);
     std::vector<float> p(3, 10.50);
     // std::array<float> p(3, 10.50);            // does not work

     p.back() = 32.43;   // reset last
     p.erase(p.begin()); // remove first
     for(auto i = p.begin(); i != p.end(); i++)
         std::cout << *i << "  ";
     std::cout << "\n";
     p.insert(++p.begin(), 15.64);
     p.insert(p.end(), 20.31);
     for(auto i = p.begin(); i != p.end(); i++)
         std::cout << "iterator-based for " << *i << "  \n";
     for(auto i : p )
         std::cout << "range-based for " << i << "  \n";
     std::cout << "\n";
 }
