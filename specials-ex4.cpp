// destructors
#include <iostream>
#include <string>
using namespace std;

class Example4 {
    string* ptr;
  public:
    // constructors:
    Example4() : ptr(new string) {}
    Example4 (const string& str) : ptr(new string(str)) {}
    // destructor:
    ~Example4 () {delete ptr;}
    // access content:
    const string& content() const {return *ptr;}
};

int main () {
  Example4 foo;
  Example4 bar ("Example");

  cout << "bar's content: " << bar.content() << '\n';
  cout << "sizeof(foo): " << sizeof(foo) << '\n';
  cout << "sizeof(bar): " << sizeof(bar) << '\n';
  cout << "sizeof(void*): " << sizeof(void*) << '\n';
  cout << "This is a " << 8*sizeof(void*) << "-bit machine\n";
  return 0;
}
