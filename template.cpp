#include <iostream>
using namespace std;

// define class with in-line member function implementation
template <class T>
class X
{
	T t_;
public:
	T t() {return t_;}
	void t(T t) { t_ = t;}
};

template <class T, int SIZE>
class Xarray
{
	T t_[SIZE];
public:
	T t(int i) {return t_[i];}
	void t(T t,int i) { t_[i] = t;}
};





// ++++++++++++++++++++++++++++++++++++++++++++++++++++++




// define class and seperate member functions
template <class T>
class Y
{
	T t_;
public:
	T t();
	void t(T t);
};

// member function implementation
template<class T>
T Y<T>::t() {return t_;}

// member function implementation
template<class T>
void Y<T>::t(T t) { t_ = t;}




// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main(int argc, char**argv)
{
  X<int>  xint;
  xint.t(44);
  cout << xint.t() << "\n";
  X<char> xchar;
  xchar.t('a');
  cout << xchar.t() << "\n";

  Y<double> ydouble;
  ydouble.t(42.7);
  cout << ydouble.t() << "\n";
  Y<char>   ychar;
  ychar.t('q');
  cout << ychar.t() << "\n";


  const size_t size = 3*1000; // *1000;   // Runs with 3000, fails with 3000000
  Xarray<long,size> xa;

  for(size_t i = 0; i < size; i++) {
    // cout << "i=" << i << "\n";
    xa.t((long) i*1000, i);  // BE CAREFUL!!  Index is second parameter
  }
}

