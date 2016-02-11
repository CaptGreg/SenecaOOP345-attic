#include <iostream>
using namespace std;

  template<typename T>
  // template<class T>  // also works most of the time.  If compiler complains, switch to typename
 //  #define T int
  class Z{
    T z;
  public:
    Z() {}
    Z(T zz) : z(zz) {}
    T getz() { return z; }
    void setz(T zz) { z = zz; }
  };

template<typename T>
T myAbs(T v) { return v > 0? v: -v; }

int main(int argc, char**argv)
{
  class X{
    int x;
  public:
    X() {}
    X(int xx) : x(xx) {}
    int getx() { return x; }
    void setx(int xx) { x = xx; }
  };
  
  X x(123);
  cout << "x.x= " << x.getx() << "\n";

  class Y{
    double y;
  public:
    Y() {}
    Y(double yy) : y(yy) {}
    double gety() { return y; }
    void sety(double yy) { y = yy; }
  };

  
  Y y(12.3);
  cout << "y.y= " << y.gety() << "\n";

  Z<float> z(3.1415962f);
  cout << "z.z= " << z.getz() << "\n";

  float f = -1.2f;
  cout << "myAbs(" << f <<  ")="  << myAbs(f) << "\n";
}
