 // g++ -std=c++11 -Wa,-adhln -g ctor.cpp -o ctor

/// Generate assembly list mixed with the source code
/// Just add these gcc compile options:
/// -Wa,-adhln -g
/// The command:
/// $ gcc -Wa,-adhln -g source_code.c > assembly_list.s
/// The options:
/// -g: Produce debugging information
/// -Wa,option: Pass option as an option to the assembler
/// -adhln:
/// a: turn on listings
/// d: omit debugging directives; n: omit forms processing
/// h: include high-level source
/// l: include assembly


#include <utility> // std::move

#pragma GCC diagnostic ignored "-Wunused-variable"

void test1()
{
  class C {
    int val;
  };

  C ca, cb;
  ca = cb;
  C cc(cb);
  ca = std::move(cb);
  C cd( std::move(ca) );
  C ce;

  struct S {
    int val;
  };

  S sa, sb;
  sa = sb;
  S sc(sb);
  sa = std::move(sb);
  S sd( std::move(sa) );
  S se;
}

void test2()
{
  class X {
    // int val;
  public:
    X(int x) {}
  };

  X a(1), b(2);
  a = b;
  X c(b);
  a = std::move(b);
  X d( std::move(a) );

// #define SHOW_ERRORS
#ifdef SHOW_ERRORS
  X e;
#endif
}

void test3()
{
  class X {
    // int val;
  public:
    X() {}
  };

  X a, b;
  a = b;
  X c(b);
  a = std::move(b);
  X d( std::move(a) );
  X e;
}


int main()
{
  test1();

  test2();

  test3();
}
