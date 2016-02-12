// http://www.cplusplus.com/reference/memory/weak_ptr/expired/
// weak_ptr::expired example
//
// GB:
//  1. print out use_counts
//  2. try it with a simple class
//  3. added try catch
//
//  class example works with new X
//  class example fails with new X[10] 
//     * shared.release() segfaults after Calling ~X() once
#include <iostream>
#include <memory>
#include <exception>

int main () {
 {
  std::shared_ptr<int> shared (new int(10));
  std::weak_ptr<int> weak(shared);

  std::cout << "1. weak " << (weak.expired()?"is":"is not") << " expired\n";
  std::cout << "shared.use_count() = " << shared.use_count() << "\n";
  std::cout << "weak.use_count() = " << weak.use_count() << "\n";
  std::cout << "\n";

  shared.reset();

  std::cout << "2. weak " << (weak.expired()?"is":"is not") << " expired\n";
  std::cout << "shared.use_count() = " << shared.use_count() << "\n";
  std::cout << "weak.use_count() = " << weak.use_count() << "\n";
  std::cout << "\n";

 }

 try {
  class X {
    uint8_t* b;
  public:
    X() : b(new uint8_t[1000000]) { std::cout << "X()\n"; }
    ~X() { delete [] b; std::cout << "~X()\n"; }
  };


  std::shared_ptr<X> shared (new X);
  std::weak_ptr<X> weak(shared);

  std::cout << "3. weak " << (weak.expired()?"is":"is not") << " expired\n";
  std::cout << "shared.use_count() = " << shared.use_count() << "\n";
  std::cout << "weak.use_count() = " << weak.use_count() << "\n";
  std::cout << "\n";

  std::cout << "calling shared.reset();\n";
  shared.reset();
  std::cout << "shared.reset(); returned\n";

  std::cout << "4. weak " << (weak.expired()?"is":"is not") << " expired\n";
  std::cout << "shared.use_count() = " << shared.use_count() << "\n";
  std::cout << "weak.use_count() = " << weak.use_count() << "\n";
  std::cout << "\n";
 } catch(const std::exception& e) {
   std::cout << "it threw " << e.what() << "\n";
 } catch(...) {
   std::cout << "it threw ...\n";
 }

  return 0;
}
