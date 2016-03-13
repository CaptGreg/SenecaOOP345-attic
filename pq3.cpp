// http://www.cplusplus.com/reference/queue/priority_queue/priority_queue/

// The example does not produce any output, but it constructs different priority_queue objects:
// - First is empty.
// - Second contains the four ints defined for myints, with 60 (the highest) at its top.
// - Third has the same four ints, but because it uses greater instead of the default (which is less), it has 10 as its top element.
// - Fourth and fifth are very similar to first: they are both empty, except that these use mycomparison for comparisons, which is a special stateful comparison function that behaves differently depending on a flag set on construction.

// constructing priority queues
#include <iostream>       // std::cout
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater

class mycomparison
{
  bool reverse;
public:
  mycomparison(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() (const int& lhs, const int&rhs) const
  {
    if (reverse) return (lhs>rhs);
    else return (lhs<rhs);
  }
};

int main ()
{
  int myints[]= {10,60,50,20};

  std::priority_queue<int> first;
  std::priority_queue<int> second (myints,myints+4);
  std::priority_queue<int, std::vector<int>, std::greater<int> >
                            third (myints,myints+4);
  // using mycomparison:
  typedef std::priority_queue<int,std::vector<int>,mycomparison> mypq_type;

  mypq_type fourth;                       // less-than comparison
  mypq_type fifth (mycomparison(true));   // greater-than comparison

  // priority queues do not have iterators so are not canditaes for range-based for loops
  // std::cout << "first :"; for(auto e : first) std::cout << e << " "; std::cout << "\n";
  //  error: no matching function for call to ‘begin(std::priority_queue<int>&)’

  //std::cout << "second:"; for(auto e : second) std::cout << e << " "; std::cout << "\n";
  //std::cout << "third :"; for(auto e : third) std::cout << e << " "; std::cout << "\n";

  return 0;
}
