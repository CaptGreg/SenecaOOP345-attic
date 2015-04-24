// valarray assignment example
#include <iostream>     // std::cout
// #include <cstddef>      // std::size_t, include works with g++, clang++ error
// error: no member named 'max_align_t' in the global namespace
// using ::max_align_t;

#include <valarray>     // std::valarray, std::slice
#include <cmath>        // std::sqrt(double)

// Valarray class
// A valarray object is designed to hold an array of values, and easily perform 
// mathematical operations on them. It also allows special mechanisms to refer 
// to subsets of elements in the arrays (see its operator[] overload).

// Most mathematical operations can be applied directly to valarray objects, 
// including arithmetical and comparison operators, affecting all its elements.

// The valarray specification allows for libraries to implement it with several 
// efficiency optimizations, such as parallelization of certain operations, memory 
// recycling or support for copy-on-reference / copy-on-write optimizations. 
// Implementations may even replace valarray as the return type for standard f
// unctions described below, provided they behave as, and can be converted to, valarray objects.

// Valarray slice selector
// This class represents a valarray slice selector. 
// It does not contain nor refers to any element 
// - it only describes a selection of elements to be 
// used as an index in valarray::operator[].

// A valarray slice is defined by a starting index, a size, and a stride.

// The starting index (start) is the index of the first element in the selection.
// The size (size) is the number of elements in the selection.
// The stride (stride) is the span that separates the elements selected.

// Therefore, a slice with a stride higher than 1 does not select contiguous 
// elements in the valarray; For example, slice(3,4,5) selects the elements 3, 8, 13 and 18.

int main1 ()
{                                //    foo:      bar:

  std::valarray<int> foo (4);    //  0 0 0 0
  std::valarray<int> bar (2,4);  //  0 0 0 0   2 2 2 2

  foo = bar;                     //  2 2 2 2   2 2 2 2
  bar = 5;                       //  2 2 2 2   5 5 5 5
  foo = bar[std::slice (0,4,1)]; //  5 5 5 5   5 5 5 5

  std::cout << "foo sums " << foo.sum() << '\n';

  return 0;
}
int main2 ()
{
  int init[]= {10,20,30,40};
                                     //     foo:           bar:

  std::valarray<int> foo (init, 4);  //  10 20 30 40
  std::valarray<int> bar (25,4);     //  10 20 30 40    25 25 25 25

  bar += foo;                        //  10 20 30 40    35 45 55 65

  foo = bar + 10;                    //  45 55 65 75    35 45 55 65

  foo -= 10;                         //  35 45 55 65    35 45 55 65

  std::valarray<bool> comp = (foo==bar);

  if ( comp.min() == true )
    std::cout << "foo and bar are equal.\n";
  else
    std::cout << "foo and bar are not equal.\n";

  return 0;
}
int increment (int x) {return ++x;}

int main3 ()
{
  int init[]={10,20,30,40,50};
  std::valarray<int> foo (init,5);

  std::valarray<int> bar = foo.apply(increment);

  std::cout << "foo contains:";
  for (std::size_t n=0; n<bar.size(); n++)
	  std::cout << ' ' << bar[n];
  std::cout << '\n';

  return 0;
}

int main4 ()
{
  std::valarray<int> foo (14);
  for (int i=0; i<14; ++i) foo[i]=i;

  std::size_t start=1;
  std::size_t lengths[]= {2,3};
  std::size_t strides[]= {7,2};

  std::gslice mygslice (start,
                        std::valarray<std::size_t>(lengths,2),
                        std::valarray<std::size_t>(strides,2));
  foo[mygslice] = 0;

  std::cout << "foo:\n";
  for (std::size_t n=0; n<foo.size(); n++)
	  std::cout << ' ' << foo[n];
  std::cout << '\n';

  return 0;
}
int main5 ()
{
  std::valarray<int> foo (8);
  for (int i=0; i<8; ++i) foo[i]=i;             //  0  1  2  3  4  5  6  7

  std::size_t sel[] = {3,5,6};
  std::valarray<std::size_t> selection (sel,3); //           *     *  *

  foo[selection] *= std::valarray<int>(10,3);   //  0  1  2 30  4 50 60 7
  foo[selection] = 0;                           //  0  1  2  0  4  0  0 7

  std::cout << "foo:";
  for (std::size_t i=0; i<foo.size(); ++i)
    std::cout << ' ' << foo[i];
  std::cout << '\n';

  return 0;
}

int main6 ()
{
  double val[] = {9.0, 25.0, 100.0};
  std::valarray<double> foo (val,3);

  std::valarray<double> bar = sqrt (foo);

  std::cout << "foo:";
  for (std::size_t i=0; i<foo.size(); ++i)
    std::cout << ' ' << foo[i];
  std::cout << '\n';

  std::cout << "bar:";
  for (std::size_t i=0; i<bar.size(); ++i)
    std::cout << ' ' << bar[i];
  std::cout << '\n';

  return 0;
}

int main()
{
  main1();
  main2();
  main3();
  main4();
  main5();
  main6();
}
