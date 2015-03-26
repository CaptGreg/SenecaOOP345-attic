// GB derived from http://stackoverflow.com/questions/27604128/c-stdfunction-like-template-syntax
#include <functional>
std::function<void(int)> f1;
std::function<int(std::string, std::string)> f2;

#include <iostream>
using namespace std;

template <typename FType>
struct Functor
{
   Functor(FType* fptr) : fptr(fptr) {}

   template <typename ...Args>
   void call(Args... args) { fptr(args...); }
   template <typename ...Args>
   void operator() (Args... args) { fptr(args...); }

private:
   FType* fptr;
};

void foo(int x, char y, bool z) { cout << "x=" <<  x << ", y=" <<  y << ", z=" <<  z << "\n"; }
void bar(char x, int y, bool z) { cout << "x=" <<  x << ", y=" <<  y << ", z=" <<  z << "\n"; }

int main()
{
   Functor<void(int, char, bool)> f(&foo);
   f.call(1, 'f', true);
   f(1, 'f', true);

   Functor<void(char, int, bool)> b(&bar);
   b.call('b', 2, false);
   b('b', 2, false);
}
