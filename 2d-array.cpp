/*
g++ -std=c++11 -ggdb 2d-array.cpp -o 2d-array && ./2d-array

FILE '2d-array.cpp' compiled Sep 22 2015 at 22:07:22 by: g++ compiler 4.9.2 generating 64-bit code
array2D ++++++++++++++ width x height: 16384,2160 array access 453001
array2D ++++++++++++++ width x height: 16384,2160 array access 427582
array2D ++++++++++++++ width x height: 16384,2160 array access 426664
array2D ++++++++++++++ width x height: 16384,2160 array access 427655
array2D ++++++++++++++ width x height: 16384,2160 array access 427558

vector2D ++++++++++++++ width x height: 16384,2160 vector setup 166595
vector2D ++++++++++++++ width x height: 16384,2160 vector access 330530
vector2D ++++++++++++++ width x height: 16384,2160 vector access 330174
vector2D ++++++++++++++ width x height: 16384,2160 vector access 329944
vector2D ++++++++++++++ width x height: 16384,2160 vector access 330467
vector2D ++++++++++++++ width x height: 16384,2160 vector access 330307

valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 193431
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 193701
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 193066
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 193625
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 193290

valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix setup 49279
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 343332
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 343352
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 342973
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 343619
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 342933

1 2 3 
4 5 6 


g++ -std=c++11 -Ofast 2d-array.cpp -o 2d-array && ./2d-array

FILE '2d-array.cpp' compiled Sep 22 2015 at 22:07:52 by: g++ compiler 4.9.2 generating 64-bit code
array2D ++++++++++++++ width x height: 16384,2160 array access 52389
array2D ++++++++++++++ width x height: 16384,2160 array access 31420
array2D ++++++++++++++ width x height: 16384,2160 array access 29584
array2D ++++++++++++++ width x height: 16384,2160 array access 29487
array2D ++++++++++++++ width x height: 16384,2160 array access 29708

vector2D ++++++++++++++ width x height: 16384,2160 vector setup 65709
vector2D ++++++++++++++ width x height: 16384,2160 vector access 30324
vector2D ++++++++++++++ width x height: 16384,2160 vector access 32114
vector2D ++++++++++++++ width x height: 16384,2160 vector access 31364
vector2D ++++++++++++++ width x height: 16384,2160 vector access 32062
vector2D ++++++++++++++ width x height: 16384,2160 vector access 30409

valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 29377
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 31299
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 28803
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 28446
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 28827

valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix setup 48596
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 21655
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 21575
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 21461
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 21256
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 22077

1 2 3 
4 5 6 

clang++ -Wall -std=c++11  -ggdb -fopenmp 2d-array.cpp -o 2d-array -lrt -pthread && ./2d-array
2d-array.cpp:241:57: warning: field 'm_height' will be initialized after field 'm_storage' [-Wreorder]
  matrix(size_t width, size_t height): m_stride(width), m_height(height), m_storage(width*height) {  }
                                                        ^
2d-array.cpp:255:17: note: in instantiation of member function 'matrix<float>::matrix' requested here
  matrix<float> vam(WIDTH,HEIGHT);
                ^
1 warning generated.

FILE '2d-array.cpp' compiled Sep 22 2015 at 22:10:10 by: clang++ compiler 4.2.1 Compatible Clang 3.3 (tags/RELEASE_33/final) generating 64-bit code
array2D ++++++++++++++ width x height: 16384,2160 array access 471517
array2D ++++++++++++++ width x height: 16384,2160 array access 449310
array2D ++++++++++++++ width x height: 16384,2160 array access 449394
array2D ++++++++++++++ width x height: 16384,2160 array access 448968
array2D ++++++++++++++ width x height: 16384,2160 array access 449666

vector2D ++++++++++++++ width x height: 16384,2160 vector setup 172810
vector2D ++++++++++++++ width x height: 16384,2160 vector access 359400
vector2D ++++++++++++++ width x height: 16384,2160 vector access 356228
vector2D ++++++++++++++ width x height: 16384,2160 vector access 356079
vector2D ++++++++++++++ width x height: 16384,2160 vector access 355977
vector2D ++++++++++++++ width x height: 16384,2160 vector access 356433

valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 285596
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 284831
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 285210
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 284552
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 284412

valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix setup 49434
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 406483
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 405797
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 406064
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 406266
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 406310

1 2 3 
4 5 6 


clang++ -Wall -std=c++11  -Ofast -fopenmp 2d-array.cpp -o 2d-array -lrt -pthread && ./2d-array
2d-array.cpp:241:57: warning: field 'm_height' will be initialized after field 'm_storage' [-Wreorder]
  matrix(size_t width, size_t height): m_stride(width), m_height(height), m_storage(width*height) {  }
                                                        ^
2d-array.cpp:255:17: note: in instantiation of member function 'matrix<float>::matrix' requested here
  matrix<float> vam(WIDTH,HEIGHT);
                ^
1 warning generated.

FILE '2d-array.cpp' compiled Sep 22 2015 at 22:09:30 by: clang++ compiler 4.2.1 Compatible Clang 3.3 (tags/RELEASE_33/final) generating 64-bit code
array2D ++++++++++++++ width x height: 16384,2160 array access 132040
array2D ++++++++++++++ width x height: 16384,2160 array access 104371
array2D ++++++++++++++ width x height: 16384,2160 array access 104315
array2D ++++++++++++++ width x height: 16384,2160 array access 104184
array2D ++++++++++++++ width x height: 16384,2160 array access 104199

vector2D ++++++++++++++ width x height: 16384,2160 vector setup 64812
vector2D ++++++++++++++ width x height: 16384,2160 vector access 76989
vector2D ++++++++++++++ width x height: 16384,2160 vector access 76920
vector2D ++++++++++++++ width x height: 16384,2160 vector access 76641
vector2D ++++++++++++++ width x height: 16384,2160 vector access 76815
vector2D ++++++++++++++ width x height: 16384,2160 vector access 76629

valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 55165
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 55042
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 55604
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 54628
valarray2D ++++++++++++++ width x height: 16384,2160 valarray access 55519

valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix setup 0
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 0
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 0
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 0
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 0
valarray2D matrix ++++++++++++++ width x height: 16384,2160 valarray matrix slice access 0

1 2 3 
4 5 6 

*/

#include <array>
#include <valarray>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

const int TIMES = 5;   // number of repeats for timing run

// const int WIDTH  = 4000;
// const int HEIGHT = 2000;
const int WIDTH  = 16*1024;
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
  RET(milli) // creates member function 'uint64_t millisecs()' - which returns 'stop-start' in millisecs
  RET(micro) // creates member function 'uint64_t microsecs()' - which returns 'stop-start' in microsecs
  RET(nano)  // creates member function 'uint64_t nanosecs()'  - which returns 'stop-start' in nanosecs
};

int main(int argc, char*argv[])
{
#ifdef __GNUC__  // either g++ or clang++
  std::cout << "\nFILE '" << __FILE__ << "' compiled " 
            << __DATE__ << " at " << __TIME__ << " by: "
  #ifdef __clang__
    << "clang++ compiler "
  #else
    << "g++ compiler "
  #endif
   << __VERSION__;
#endif

  cout << " generating " << 8*sizeof(void*) << "-bit code\n";
  cout << argv[0] << " running on a " << std::thread::hardware_concurrency() << " core machine\n";

  Timer t;
  for(int test = 0; test<TIMES; test++) {
    t.Start();
    for(int y = 0; y < HEIGHT; y++) {
      // cout << "\n" << "++++++ " << y << "\n";
      for(int x = 0; x < WIDTH; x++) {
	// cout << " " << x;
	array2D  [y][x] = 1e6*y + x;
      }
    }
    t.Stop();
    cout << "array2D ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " array access " << t.microsecs() << "\n";
  }
  cout << "\n";

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
  cout << "\n";

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
  cout << "\n";


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

  // http://stackoverflow.com/questions/2187648/how-can-i-use-a-stdvalarray-to-store-manipulate-a-contiguous-2d-array
  typedef valarray<valarray<int> > va2d;

  int data[][3] = { {1, 2, 3}, {4, 5, 6} };
  va2d mat(valarray<int>(3), 2);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 3; ++j) { mat[ i ][ j ] = data[ i ][ j ]; }
  }
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 3; ++j) { cout << mat[ i ][ j ] << " "; }
    cout << "\n";
  }
}

template <class T> class matrix
{ // http://stackoverflow.com/questions/2187648/how-can-i-use-a-stdvalarray-to-store-manipulate-a-contiguous-2d-array
  std::valarray<T> m_storage;
  size_t m_stride;
  size_t m_height;
public:
  matrix(size_t width, size_t height): m_stride(width), m_height(height), m_storage(width*height) {  }
  T &operator() (size_t row, size_t column) {
            // A valarray slice is defined by a starting index, a size, and a stride.
  // return m_storage[std::slice(column, m_height, m_stride)][row];    // column major : syntax error
  // return m_storage[std::slice(row,    m_stride, m_height)][column]; // row major    : syntax error
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
  cout << "valarray2D matrix ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " valarray matrix setup " << t.microsecs() << "\n";
  for(int test = 0; test<TIMES; test++) {
    t.Start();
    for(int y = 0; y < HEIGHT; y++) {
      for(int x = 0; x < WIDTH; x++) {
	vam(x,y) = 1e6*y + x;
     // vam[x,y] = 1e6*y + x;
      }
    }
    t.Stop();
    cout << "valarray2D matrix ++++++++++++++ width x height: " << WIDTH << "," << HEIGHT << " valarray matrix slice access " << t.microsecs() << "\n";
  }
  cout << "\n";

}
