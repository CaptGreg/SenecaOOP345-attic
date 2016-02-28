#include <iostream>
#include <iomanip>
using namespace std;

#include <typeinfo>
#ifdef __MSVC__
  #include <intrin.h>
#else                            // GB assume clang++/g++
  #include <mmintrin.h>
  #include <emmintrin.h>
#endif


template<typename T>
void print(T& i)  { cout << i << " is an l-value, typeid(t).name() returns: " << typeid(i).name() << "\n"; }

#define VECTOR_SIZE         4
typedef float v4sf __attribute__ ((vector_size(sizeof(float)*VECTOR_SIZE)));   // 128-bit SSE
typedef int   v4si __attribute__ ((vector_size(sizeof(int32_t)*VECTOR_SIZE))); // 128-bit MMX
// or ...     v4si __attribute__ ((vector_size(sizeof(int)*VECTOR_SIZE)));     // 128-bit MMX

int main(int argc, char**argv)
{
#define TID(var)  cout << setw(30) << "typeid(" #var ").name()" << " = " << typeid(var).name() << "\n";

  TID(void*)
  TID(size_t)
  TID(streampos) // defined in <iostream> or <fstream>
  TID(int)
  TID(int*)
  TID(char)
  TID(char*)
  TID(long)
  TID(long*)
  TID(long long)
  TID(long long*)
  TID(float)
  TID(float*)
  TID(double)
  TID(double*)
  TID(long double)
  TID(long double*)
  TID(short)
  TID(short*)
  TID(bool)
  TID(bool*)
  TID(size_t)
  TID(size_t*)
  TID(int8_t)
  TID(int8_t*)
  TID(int16_t)
  TID(int16_t*)
  TID(int32_t)
  TID(int32_t*)
  TID(int64_t)
  TID(int64_t*)
  // TID(int128_t)  // 2014 not yet, maybe later...
  
  TID( v4sf )       //  g++ -Ofast -msse3 -ftree-vectorize -ftree-vectorizer-verbose=2
  TID( v4si )       //  g++ -Ofast -msse3 -ftree-vectorize -ftree-vectorizer-verbose=2

  TID( __m64 )      // TIDTIDE
  TID( __m128 )     // SSE
  // TID( __m256 )     // AVX
  // TID( __m512 )  // AVX-512
}
