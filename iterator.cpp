// iterator.cpp

// Iterators - Vectors, range-based for walk, hack for range-based for reverse walk

#include <vector>
#include <iostream>

// reference:
// http://en.cppreference.com/w/cpp/language/range-for

// can we use a range-based for to walk in reverse?  --- YES:
// http://www.codesynthesis.com/~boris/blog/2012/05/16/cxx11-range-based-for-loop

// other hacks:
// https://gist.github.com/compnerd/5694186
// http://edmundv.home.xs4all.nl/blog/2014/07/16/reversed-range-based-for-loop/
// http://kbokonseriousstuff.blogspot.ca/2011/09/using-reverse-iterators-with-c11-range.html

template <typename T>
struct reverse_range
{
private:
  T& x;
 
public:
  reverse_range (T& xx): x (xx) {}
 
  auto begin () const -> decltype (this->x.rbegin ()) { return x.rbegin (); }
 
  auto end () const -> decltype (this->x.rend ()) { return x.rend (); }
};
 
template <typename T>
reverse_range<T> reverse_iterate (T& x) { return reverse_range<T> (x); }

template <typename T>
reverse_range<T> reverse_iterate (const T& x) { return reverse_range<T> (x); } // GB 


int main(int argc, char**argv)
{
    ///////////////////
    // STL container //
    ///////////////////

    std::vector<double> v;

    v.push_back(1.1);
    v.push_back(2.2);
    v.push_back(3.3);
    v.push_back(4.4);
    v.push_back(5.5);

    std::cout << argv[0] << " Forward walk:\n";

    std::vector<double>::iterator it;
    for(it = v.begin(); it != v.end(); it++) 
        std::cout << *it << "  ";
    std::cout << "\n";

    for(auto it = begin(v); it != end(v); it++) 
        std::cout << *it << "  ";
    std::cout << "\n";

    std::cout << argv[0] << " Reverse walk:\n";

    std::vector<double>::reverse_iterator rit;
    for(rit = v.rbegin(); rit != v.rend(); rit++)
        std::cout << *rit << "  ";
    std::cout << "\n";

    for(auto rit = v.rbegin(); rit != v.rend(); rit++)
        std::cout << *rit << "  ";
    std::cout << "\n";

    std::cout << argv[0] << " range-based for Forward walk:\n";

    for( auto e : v ) std::cout << e << "  "; std::cout << "\n";

    std::cout << argv[0] << " range-based for Reverse walk:\n";

    // use 'reverse_iterate' hack
    for ( auto e : reverse_iterate (v) )  std::cout << e << "  "; std::cout << "\n";

    ////////////
    // arrays //
    ////////////

    double d[] = { 6.6, 7.7, 8.8, 9.9, 10.};
//  double e[]   { 6.6, 7.7, 8.8, 9.9, 10.};     // alternate declaration without the '='
    std::cout << "double d[] ={ 6.6, 7.7, 8.8, 9.9, 10.};\n";

    std::cout << argv[0] << " range-based for walks arrays:\n";
    for( auto e : d ) std::cout << e << "  "; std::cout << "\n";

    // for(it = begin(d); it != end(d); it++) std::cout << *it << "  "; std::cout << "\n";

    // for(rit = d.rbegin(); rit != d.rend(); rit++) std::cout << *rit << "  "; std::cout << "\n";

    // use 'reverse_iterate' hack
    std::cout << argv[0] << " range-based for reverse walk hack generates syntax error with arrays\n";
    // for ( auto e : reverse_iterate (d) ) std::cout << e << "  "; std::cout << "\n";
}
