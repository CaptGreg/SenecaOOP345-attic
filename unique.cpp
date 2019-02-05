#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

using namespace std;

template <class T>
void RemoveDuplicatesInVector(std::vector<T> & vec)
{
    // set<T> values;
    unordered_set<T> values;
    /*
      Sets are containers that store unique elements following a specific order.

      In a set, the value of an element also identifies it (the value is itself the key, of type T), 
      and each value must be unique. The value of the elements in a set cannot be modified once in 
      the container (the elements are always const), but they can be inserted or removed from the container.

      Internally, the elements in a set are always sorted following a specific strict weak ordering 
      criterion indicated by its internal comparison object (of type Compare).

      set containers are generally slower than unordered_set containers to access individual elements 
      by their key, but they allow the direct iteration on subsets based on their order.

      Sets are typically implemented as binary search trees.

      set::insert --- The single element versions (1) return a pair, with its member pair::first set 
      to an iterator pointing to either the newly inserted element or to the equivalent element already 
      in the set. The pair::second element in the pair is set to true if a new element was inserted or 
      false if an equivalent element already existed.
    */
    vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const T & value) { return !values.insert(value).second; }), vec.end());
    cout << "unordered set: ";
    for (int i : values)
      std::cout << i << " ";
    std::cout << "\n";

}

int main() 
{
    // remove duplicate elements
    std::vector<int> v{1,2,3,1,2,3,3,4,5,4,5,6,7};
    std::sort(v.begin(), v.end()); // 1 1 2 2 3 3 3 4 4 5 5 6 7 
      // auto last = std::unique(v.begin(), v.end());
      // v now holds {1 2 3 4 5 6 7 x x x x x x}, where 'x' is indeterminate
      // v.erase(last, v.end()); 

    // OR IN ONE LINE:
    v.erase(std::unique(v.begin(), v.end()), v.end()); 

    cout << "sort-unique: "; 
    for (int i : v)
      std::cout << i << " ";
    std::cout << "\n";

    cout << "w: ";
    std::vector<int> w{1,2,3,1,2,3,3,4,5,4,5,6,7};
    for (int i : w)
      std::cout << i << " ";
    std::cout << "\n";

    RemoveDuplicatesInVector(w);
    for (int i : w)
      std::cout << i << " ";
    std::cout << "\n";
}
