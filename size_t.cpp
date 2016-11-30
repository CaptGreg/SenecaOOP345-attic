#include <iostream>
using namespace std;

int main()
{
  cout << "for dogs and aliens\n";
  cout  << 9 * 1024 * 1024 * 1024 << "\n";
  cout  << 9LL * 1024LL * 1024LL * 1024LL << "\n";

  cout << "for humans\n";
  cout  << 9 * 1000 * 1000 * 1000 << "\n";
  cout  << 9LL * 1000LL * 1000LL * 1000LL << "\n";

  size_t thousand = 1000;
  size_t billion = thousand * thousand * thousand;

  cout << "using size_t:\n";
  cout  <<  9* billion << "\n";
}
