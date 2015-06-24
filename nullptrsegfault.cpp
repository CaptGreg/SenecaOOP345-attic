#include <exception>
#include <iostream>
using namespace std;

int main()
{
  try {
    char *p = new char[1000000000LL];
    delete [] p;
    delete [] p;         // want this segfault or throw (double delete)
    p = nullptr;
    delete [] p;         // want this segfault to throw

  } catch(exception& e) {
    cout << "threw exception: " << e.what() << "\n";
  } catch(...) {
    cout << "threw ...\n";
  }
}
