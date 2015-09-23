/*
g++ -std=c++11 -ggdb 2d-array.cpp -o 2d-array && ./2d-array
array2D ++++++++++++++ width x height: 16384,2160 array access 533526
array2D ++++++++++++++ width x height: 16384,2160 array access 416049
array2D ++++++++++++++ width x height: 16384,2160 array access 415838
array2D ++++++++++++++ width x height: 16384,2160 array access 415904
array2D ++++++++++++++ width x height: 16384,2160 array access 415191

vector2D ++++++++++++++ width x height: 16384,2160 vector setup 162629
vector2D ++++++++++++++ width x height: 16384,2160 vector access 190605
vector2D ++++++++++++++ width x height: 16384,2160 vector access 190981
vector2D ++++++++++++++ width x height: 16384,2160 vector access 192398
vector2D ++++++++++++++ width x height: 16384,2160 vector access 189058
vector2D ++++++++++++++ width x height: 16384,2160 vector access 191022

valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 203303
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 203838
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 203093
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 202712
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 203540

valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix setup 122277
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 352723
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 353484
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 353854
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 353543
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 353792

g++ -std=c++11 -Ofast 2d-array.cpp -o 2d-array && ./2d-array
array2D ++++++++++++++ width x height: 16384,2160 array access 131134
array2D ++++++++++++++ width x height: 16384,2160 array access 36898
array2D ++++++++++++++ width x height: 16384,2160 array access 36831
array2D ++++++++++++++ width x height: 16384,2160 array access 33876
array2D ++++++++++++++ width x height: 16384,2160 array access 37630

vector2D ++++++++++++++ width x height: 16384,2160 vector setup 73420
vector2D ++++++++++++++ width x height: 16384,2160 vector access 34971
vector2D ++++++++++++++ width x height: 16384,2160 vector access 38926
vector2D ++++++++++++++ width x height: 16384,2160 vector access 37297
vector2D ++++++++++++++ width x height: 16384,2160 vector access 35519
vector2D ++++++++++++++ width x height: 16384,2160 vector access 34243

valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 37509
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 38734
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 35077
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 37236
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 37755

valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix setup 123846
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 23028
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 22574
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 23029
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 23179
valarray2D ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 22687

*/
#include <array>
#include <valarray>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std;

// const int WIDTH  = 4000;
// const int HEIGHT = 2000;
const int WIDTH  = 16*1024;
const int TIMES = 5;

const int HEIGHT = 2160;

array     <array <float,WIDTH>, HEIGHT>    array2D;
valarray  <float>                          valarray2D (WIDTH*HEIGHT) ;
vector    <vector<float> >                 vector2D;

class Timer { // use C++11 std::chrono features to create a stop-watch timer class
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  Timer() {}
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop () { stop  = std::chrono::high_resolution_clock::now(); }
  // basic form to calculate time differences, illustrate with microseconds
  uint64_t usecs() {
    typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    uint64_t us = duration_get.count();
    return us;
  }
  // Now use a macro to return milli, micro, and nano seconds 
  #define RET(UNITS) uint64_t UNITS##secs() { \
    typedef std::chrono::duration<int,std::UNITS> UNITS##secs_t ; \
    UNITS##secs_t duration_get( std::chrono::duration_cast<UNITS##secs_t>(stop-start) ) ; \
    uint64_t us = duration_get.count(); \
    return us; \
  }
  RET(milli)
  RET(micro)
  RET(nano)
};

int main()
{
#ifdef __GNUC__  // either g++ or clang++
  std::cout << "\nFILE '" << __FILE__ << "' compiled " 
            << __DATE__ << " at " << __TIME__ << " by: "
#ifdef __clang__
    << "clang++ compiler " << __VERSION__ 
  #else
    << "g++ compiler " << __VERSION__
  #endif
  << " generating " << 8*sizeof(void*) << "-bit code\n";
#endif

  Timer t;
  for(int test = 0; test<TIMES; test++) {
    t.Start();
    for(int y = 0; y < HEIGHT; y++) {
      // cout << endl << "++++++ " << y << endl;
      for(int x = 0; x < WIDTH; x++) {
	// cout << " " << x;
	array2D  [y][x] = 1e6*y + x;
      }
    }
    t.Stop();
    cout << "array2D ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " array access " << t.microsecs() << "\n";
  }
  cout << endl;

  // Set up sizes. (HEIGHT x WIDTH)
  t.Start();
  vector2D.resize(HEIGHT);
  for (int y = 0; y < HEIGHT; ++y)
    vector2D[y].resize(WIDTH);
  t.Stop();
  cout << "vector2D ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " vector setup " << t.microsecs() << "\n";
  for(int test = 0; test<TIMES; test++) {
    t.Start();
    for(int y = 0; y < HEIGHT; y++) {
      for(int x = 0; x < WIDTH; x++) {
	vector2D [y][x] = 1e6*y + x;
      }
    }
    t.Stop();
    cout << "vector2D ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " vector access " << t.microsecs() << "\n";
  }
  cout << endl;

  for(int test = 0; test<TIMES; test++) {
    t.Start();
    int i = 0;
    for(int y = 0; y < HEIGHT; y++) {
      for(int x = 0; x < WIDTH; x++) {
	valarray2D [i++] = 1e6*y + x;
      }
    }
    t.Stop();
    cout << "valarray2D ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " valarray access " << t.microsecs() << "\n";
  }
  cout << endl;


/*
Valarray slice selector
This class represents a valarray slice selector. It does not contain nor refers to any element - it only describes a selection of elements to be used as an index in valarray::operator[].

A valarray slice is defined by a starting index, a size, and a stride.

The starting index (start) is the index of the first element in the selection.
The size (size) is the number of elements in the selection.
The stride (stride) is the span that separates the elements selected.

Therefore, a slice with a stride higher than 1 does not select contiguous elements in the valarray; For example, slice(3,4,5) selects the elements 3, 8, 13 and 18.

// slice_array example
#include <iostream>     // std::cout
#include <cstddef>      // std::size_t
#include <valarray>     // std::valarray, std::slice

int main ()
{
  std::valarray<int> foo (9);
  for (int i=0; i<9; ++i) foo[i]=i;         //  0  1  2  3  4  5  6  7  8
                                            //     |     |     |
  std::slice myslice=std::slice(1,3,2);     //     v     v     v
  foo[myslice] *= std::valarray<int>(10,3); //  0 10  2 30  4 50  6  7  8
                                            //  |        |        |
  foo[std::slice (0,3,3)] = 99;             //  v        v        v
                                            // 99 10  2 99  4 50 99  7  8
  std::cout << "foo:";
  for (std::size_t n=0; n<foo.size(); n++)
	  std::cout << ' ' << foo[n];
  std::cout << '\n';

  return 0;
}

	
Edit & Run


Output:


99 10 2 99 4 50 99 7 8

*/
  void valarrayMatrixTest();
  valarrayMatrixTest();
}

template <class T> class matrix
{ // http://stackoverflow.com/questions/2187648/how-can-i-use-a-stdvalarray-to-store-manipulate-a-contiguous-2d-array
    std::valarray<T> m_storage;
    size_t m_stride;
    size_t m_height;
public:
    matrix(size_t width, size_t height): m_stride(width), m_height(height), m_storage(width*height) {  }
    T &operator() (size_t row, size_t column) {
     // return m_storage[std::slice(column, m_height, m_stride)][row];    // column major
     // return m_storage[std::slice(row,    m_stride, m_height)][column]; // row major
     // return m_storage[row    * m_stride + column];    // row major   (on exit:double free or corruption)
        return m_storage[column * m_height + row];       // column major (works!)
    }
};

void valarrayMatrixTest()
{
  Timer t;
  t.Start();
  matrix<float> vam(WIDTH,HEIGHT);
  t.Stop();
  cout << "valarray2D ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " valarray matrix setup " << t.microsecs() << "\n";
  for(int test = 0; test<TIMES; test++) {
    t.Start();
    for(int y = 0; y < HEIGHT; y++) {
      for(int x = 0; x < WIDTH; x++) {
	vam(x,y) = 1e6*y + x;
     // vam[x,y] = 1e6*y + x;
      }
    }
    t.Stop();
    cout << "valarray2D ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " valarray matrix slice access " << t.microsecs() << "\n";
  }
  cout << endl;

}
