// http://www.thegeekstuff.com/2016/06/friend-class-cpp/

#include <iostream>
using namespace std;

class CKomplexNumber
{
private:
  double dX,dY;

public:
  CKomplexNumber(const double x, const double y) {dX=x;dY=y;}
  CKomplexNumber() {}
  CKomplexNumber operator+(const CKomplexNumber& z)
  {
    CKomplexNumber temp=*this;
    temp.dX += z.dX; temp.dY += z.dY;
    return temp;
  }

  friend ostream& operator<<(ostream& out, const CKomplexNumber z);
  friend istream& operator>>(istream& in, CKomplexNumber& z);
};

ostream& operator<<(ostream& out, const CKomplexNumber z)
{
  cout<<"Complex number is ";
  out<<z.dX<<" + "<<z.dY<<"i"<<endl;  // Friend permits access to private variables dX and dY
  return out;
}

istream& operator>>(istream& in, CKomplexNumber& z)
{
  cout<<"Input real and imaginary part"<<endl;
  in>>z.dX>>z.dY;                      // Friend permits access to private variables dX and dY
  return in;
}

int main(void)
{
  CKomplexNumber Z1;
  cout<<"First complex number: ";
  cin>>Z1;
  cout<<Z1;

  CKomplexNumber Z2;
  cout<<"Second complex number: ";
  cin>>Z2;
  cout<<Z2;

  CKomplexNumber Z3;
  cout<<"Third complex number: ";
  cin>>Z3;
  cout<<Z3;

  CKomplexNumber Zr(0,0);
  Zr = Z1 + Z2 + Z3;
  cout<<Zr;

  cout<< "expression sum is: " << Z1 + Z2 + Z3;

  return EXIT_SUCCESS;
}
