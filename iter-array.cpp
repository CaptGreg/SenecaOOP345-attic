#include <iostream>
#include <algorithm>  /// misc algorithms
using namespace std;

int a[] {1,2,3,4,5};

int main()
{
  cout << "pointer\n";
  for(auto it = a; it != a +(sizeof(a)/sizeof(*a)); it++)
    cout << *it << " ";
  cout << "\n\n";

  cout << "iterator\n";
  for(auto it = begin(a); it != end(a); it++)
    cout << *it << " ";
  cout << "\n\n";

  cout << "for_each\n";
  for_each(begin(a), end(a), // sequence 
    [] (int& e) { cout << e << " "; });
  cout << "\n\n";

  cout << "tranform\n";
  transform(
    begin(a), end(a),   // input sequence
    begin(a),           // output sequence
    [] (int& e) { // transform function
      e += e;
      cout << e << " ";
      return e;
    });
  cout << "\n\n";

}
