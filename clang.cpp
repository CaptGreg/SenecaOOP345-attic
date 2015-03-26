// http://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang/

// clang -cc1 -fdump-record-layouts clang.cpp
class Base {
protected:
  int foo;
public:
  int method(int p) {
    return foo + p;
  }
};

struct Point {
  double cx, cy;
};

class Derived : public Base {
public:
  int method(int p) {
    return foo + bar + p;
  }
protected:
  int bar, baz;
  Point a_point;
  char c;
};

int main(int argc, char** argv) {
  return sizeof(Derived);
}
