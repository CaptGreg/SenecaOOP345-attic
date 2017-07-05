#include <iostream>
#include <vector>
#include <typeinfo>
#include <iomanip>
using namespace std;

int main()
{
  vector <int> v {1,2,3,4,5,6,7,8,9,10};
  cout << "v ="; for(auto& e: v) cout << " " << e; cout << "\n";
  cout << "\n";

  cout << "v.size()=" << v.size() << ", v.end()-v.begin()=" << v.end()-v.begin() << "\n";
  cout << "\n";

  auto index = v.end() - v.begin();

  cout << "auto index=v.end()-v.begin()=" << index << "\n";

  cout << "\n";


#define P(VAR)  cout << setw(30) << "typeid(" #VAR ").name()" << " = " << typeid(VAR).name() << "\t(" << sizeof(VAR) <<  " Bytes)\n";

  cout << "on this " << 8*sizeof(void*) << "-bit machine:\n";
  P(index)
  P(long)
  P(unsigned long)
  P(size_t)

  cout << "\n";
  cout <<"Note the typeid of 'index' is long which compiles as a signed " << sizeof(index) << " byte integer under " << 8*sizeof(void*) << "-bit g++\n";
}
