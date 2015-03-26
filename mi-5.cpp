#include <iostream>
using namespace std;

class Square
  {
    protected:
     int l;
    public:
     void set_values (int x)
      { l=x;}
  };

class CShow 
  {
    public:
     void show(int i);
  };

void CShow::show (int i) 
 {
   cout << "The area of the square is::" << i << endl;
 }

class Area: public Square, public CShow
  {
   public:
    int area()
      { return (l *l); }
  };

int main (int argc, char**argv)
{
  cout << endl << argv[0] << endl << endl;
  Area r;
  r.set_values (5);
  r.show(r.area());
  return 0;
}
