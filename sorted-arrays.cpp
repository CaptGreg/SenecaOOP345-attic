// http://asawicki.info/news_1353_my_algorithms_for_sorted_arrays.html

#include <algorithm>
#include <iterator>
#include <iostream>
#include <map>
#include <string>

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// The Beauty of Sorted Arrays

// A one-dimmensional array (also called vector) is very simple data structure. We were taught that inserting or deleting elements in the middle of the array is slow because we have to move (by rewriting) all following elements. That's why books about C++ teach that for sorted collections - those who provide fast search operation - it's better to use containers like std::set or std::map.

// But those who code games in C++ and care about performance know that today's processors (CPU) are very fast in terms of doing calculations and processing lots of data, while access to the RAM memory becomes more and more kind of a bottleneck. According to great presentation Pitfalls of Object Oriented Programming by Tony Albrecht from Sony, a cachce miss (access to some data from the memory which are not currently in CPU cache) costs about 400 cycles! Dynamic memory allocation (like operator new in C++) is also a slow operation. That's why data structures that use separate nodes linked with pointers (like std::list, std::set, std::map) are slower than std::vector and it's often better to keep a sorted collection of elements in a vector, even if you have hundreds or thousands of elements and must sometimes insert or delete them from the middle.

// Unfortunately, C++ STL doesn't provide a "std::ordered_vector" container that whould keep items sorted while allocating single chunk of continuous memory. We have to do it by ourselves, like this:

// First, we have to choose a way to compare elements. There are multiple solutions (read about comparators in C++), but the simplest one is to just overload operator < in your element structure.

void TheBeautyofSortedArrays ()
{
  struct Item
  {
    int Id;
    uint32_t Color;
    std::string Name;

    Item() { }
    Item(int id, uint32_t color, const std::string name)
      : Id(id), Color(color), Name(name) { }

    bool operator < (const Item &rhs) const { return Id < rhs.Id; }
  };

  typedef std::vector<Item> ItemVector;

  ItemVector vec;
  Item item;

  // Here is how we insert an item into sorted vector:

  item = Item(1, 0xFF0000, "Red");
  ItemVector::iterator itToInsert = std::lower_bound(vec.begin(), vec.end(), item);
  vec.insert(itToInsert, item);

  // Unfortunately we have to deal with iterators instead of simple indices.

  // To check if the vector contains element with particular Id:

  item = Item(1, 0, std::string());
  bool contains = std::binary_search(vec.begin(), vec.end(), item);

  // Here come another nuisances. We have to create full Item object just to use its Id field for comparison purpose. I'll show tomorrow how to overcome this problem.

  // It's also annoying that binary_search algorithm returns bool instead of iterator that would show us where the found item is. To find an item and determine its iterator (and from this its index) we have to use lower_bound algorithm. This algorithm is very universal but also hard to understand. It quickly searches a sorted collection and returns the position of the first element that has a value greater than or equivalent to a specified value. It was perfect for determining a place to insert new item into, but to find existing element, we have to use it with this if:

  item = Item(1, 0, std::string());
  ItemVector::iterator findIt = std::lower_bound(vec.begin(), vec.end(), item);
  if (findIt != vec.end() && findIt->Id == item.Id)
  {
    size_t foundIndex = std::distance(vec.begin(), findIt);
  }
  else
  {
    // Not found.
  }

  // And finally, to remove an item from the vector while having index, we do this trick:

  size_t indexToDelete = 0;
  vec.erase(vec.begin() + indexToDelete);

  // Well, C++ STL is very general, universal, powerful and... very complicated, while standard libraries of other programming languages also have some curiosities in their array classes.

  // For example, ActionScript 3 have single method to insert and remove items:

  // function splice(startIndex:int, deleteCount:uint, ...  values):Array

  // To delete some items, you pass non-zero deleteCount. To insert some items, you pass them as values parameter.

  // In .NET, the System.Collections.Generic.List<T> class (which is actually an array, the name is misnomer) has very smart BinarySearch method. It returns the "zero-based index of item (...), if item is found; otherwise, a negative number that is the bitwise complement of the index of the next element that is larger than item or, if there is no larger element, the bitwise complement of Count.". It can be used to both find existing item in the array or determine an index for a new item, like:

  // int index = myList->BinarySearch("abc");
  // if (index < 0)
    // myList->Insert(~index, "abc");

} // TheBeautyofSortedArrays ()

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// My Algorithms for Sorted Arrays

// Yesterday I wrote about The Beauty of Sorted Arrays. I believe the C++ STL is not perfect and so I've written some additional code that I needed for the purpose of handling such data structure. It can be found in my CommonLib library.

// I once wanted to have a sorting algorithm, just like std::sort, but using insertion sort. The std::sort is of course great, has average O(n log n) complexity and AFAIK internally uses introsort algorithm (an improved version of quicksort). But if we know that the collection is already almost sorted, insertion sort will work faster. So here is my code:

// Basic version that uses operator <
template<class Iterator>
void InsertionSort(Iterator b, Iterator e)
{
  if (b == e) return;
  for (Iterator j = b + 1; j < e; j++)
  {
    typename std::iterator_traits<Iterator>::value_type key;
    key = *j;
    Iterator i = j;
    while (i > b && key < *(i-1))
    {
      *i = *(i-1);
      i--;
    } 
    *i = key;
  }
}

// Overloaded version with explicit comparator
template<class Iterator, typename Compare>
void InsertionSort(Iterator b, Iterator e, Compare Comp)
{
  if (b == e) return;
  for (Iterator j = b + 1; j < e; j++)
  {
    typename std::iterator_traits<Iterator>::value_type key;
    key = *j;
    Iterator i = j;
    while (i > b && Comp(key, *(i-1)))
    {
      *i = *(i-1);
      i--;
    } 
    *i = key;
  }
}

// Another problem is finding an item in the ordered array by some key. I can see two solutions in C++, both unsatisfactory. First one is to write your own comparator or overload operator < for your item structure to compare items by key, like this:

struct Item
{
  int Id;
  uint32_t Color;
  std::string Name;

  bool operator < (const Item &rhs) const { return Id < rhs.Id; }
};

// The problem is that C++ comparator always compares two full items, so to use this comparing feature for finding item in the array, we must construct and pass full Item object, despite we use only the Id field.

std::vector<Item> vec;
void somefunction( )
{
  Item item;
  item.Id = 1;
  bool contains = std::binary_search(vec.begin(), vec.end(), item);

  // Second solution is to use std::map container. It has separate key and value type so you can search by key:

  std::map<int, Item> m;
  std::map<int, Item>::iterator foundIt = m.find(1);
  if (foundIt != m.end())
    std::cout << "Found ID=" << foundIt->first
      << ", Name=" << foundIt->second.Name << std::endl;

  // But the problem is that the key field is separate from value type so the Id field should not be included in the Item structure because it's redundant. We should instead use std::pair<int, Item> as the type of items contained in the map, which is alised to std::map<int, Item>::value_type.
}

// I wanted to have more convenient way to quickly search an ordered array by key, so I've coded these two function templates:

template <typename IterT, typename KeyT, typename CmpT>
IterT FirstNotLessIndex(IterT beg, IterT end, const KeyT &key, CmpT cmp)
{
  unsigned down = 0, up = (end - beg);
  while(down < up)
  {
    unsigned mid = (down + up) / 2;
    int res = cmp(key, *(beg + mid));
    if(res > 0)
      down = mid + 1;
    else
      up = mid;
  }
  return beg + down;
}

template <typename IterT, typename KeyT, typename CmpT>
IterT BinarySearch(IterT beg, IterT end, const KeyT &key, CmpT cmp)
{
  IterT it = FirstNotLessIndex<IterT, KeyT, CmpT>(beg, end, key, cmp);
  if (it == end || cmp(key, *it) != 0)
    return end;
  return it;
}

// First one performs binary search to determine the iterator for a first element not less (greater or equal) than the specified key. It's perfect for determining the place for inserting new item into the array. Second function uses first one to check whether an item with exact matching key exists in the array and returns iterator pointing to it, or end iterator if not found.

// As you can see, both functions take the type of the key as a template parameter, which may be different than full item type pointed by iterator. They also take a comparator functor which is always called with parameters (key, item) and should return int. The comparator should return a number less, equal or greater than zero according to the result of the comparison - just like strcmp, memcmp and other functions from old C.

// Now my example can be written like this:

struct Item2
{
  int Id;
  uint32_t Color;
  std::string Name;

  static int Cmp(int id, const Item2 &rhs) { return id - rhs.Id; }

  // Having such universal comparison function, it's easy to convert it to all comparison operators:
  bool operator <  (const Item2 &rhs) const { return Cmp(Id, rhs) <  0; }
  bool operator >  (const Item2 &rhs) const { return Cmp(Id, rhs) >  0; }
  bool operator <= (const Item2 &rhs) const { return Cmp(Id, rhs) <= 0; }
  bool operator >= (const Item2 &rhs) const { return Cmp(Id, rhs) <= 0; }
  bool operator == (const Item2 &rhs) const { return Cmp(Id, rhs) == 0; }
  bool operator != (const Item2 &rhs) const { return Cmp(Id, rhs) != 0; }

};
void someFuntion2 ()
{
  // bool contains = BinarySearch(vec.begin(), vec.end(), 1, &Item2::Cmp) != vec.end();
}

// Opposite conversion is also possible. If you have a type with operator < accessible (like float or your own type with overloaded operator), you can convert it to a comparator returning int with this function:

template <typename T>
inline int UniversalCmp(const T &a, const T &b)
{
  if (a < b) return -1;
  if (b < a) return  1;
  return 0;
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main()
{
}
