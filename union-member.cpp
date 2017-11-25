// can unions have member functions?

#include <iostream>
using namespace std;

template<typename T>
void printBits(T& t)
{
  for(int bit = 8*sizeof(T)-1; bit >= 0; bit--)
    cout << (bool) (t & (1 << bit));
}

union x_u {
  int x;
  int y;
  float z;
  x_u()             {cout<<"ctor called\n";}
  ~x_u()            {cout<<"dtor called\n";}
  void  X(int xx)   { x = xx; }
  void  Y(int yy)   { y = yy; }
  void  Z(float zz) { z = zz; }
  int   X()         { return x; }
  int   Y()         { return y; }
  float Z()         { return z; }
  void  P()         { printBits(x) ; cout << " x=" << x << " y=" << y << " z=" << z << "\n";}
};

x_u instance;

int main()
{
  cout << "main starting\n";
  atexit([] {cout << "atexit callback called\n";});

  instance.X(1);
  instance.P();

  instance.Y(2);
  instance.P();

  instance.Z(3.);
  instance.P();

  cout << "main finished\n";
  return 0;
}
