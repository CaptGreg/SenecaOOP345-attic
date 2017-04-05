#include <deque>
#include <list>
using namespace std;

class X {
public:
  X(const X& x) {}
};

class Y {
public:
  Y(const Y& y) = default;
};

class Z {
public:
  Z(const Z& z) = delete;
};


int main ()
{
  {
    deque<X> xDeque1;
    deque<X> xDeque2(xDeque1);

    deque<Y> yDeque1;
    deque<Y> yDeque2(yDeque1);

    deque<Z> zDeque1;
#ifdef SHOW_ERRORS
    deque<Z> zDeque2(zDeque1);
    // error: use of deleted function ‘Z::Z(const Z&)’
    // note: declared here
    // Z(const Z& z) = delete;
#endif
  }

  {
    list<X> xList1;
    list<X> xList2(xList1);

    list<Y> yList1;
    list<Y> yList2(yList1);

    list<Z> zList1;
#ifdef SHOW_ERRORS
    list<Z> zList2(zList1);
    //  error: use of deleted function ‘Z::Z(const Z&)’
    // note: declared here
    // Z(const Z& z) = delete;
#endif
  }
}
