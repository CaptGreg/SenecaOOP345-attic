#include <iostream>
#include <fstream>        // streampos

// AVX: http://en.wikipedia.org/wiki/Advanced_Vector_Extensions
// AVX-512: http://en.wikipedia.org/wiki/AVX-512


// #include <inttype>
// #include <mmintrin.h>      // 64-bit  MME - Multi-Media SIMD Extensions.       MM0 -MM15
// #include <emmintrin.h>     // 128-bit SSE - Streaming SIMD Extensions.         XMM0-XMM15
// #include <avxintrin.h>     // 256-bit AVX - Advanced Vector SIMD Extensions.   YMM0-YMM15
// #include <avx2intrin.h>    // 512-bit AVX2 - Advanced Vector SIMD Extensions2. ZMM0-ZMM15

// #define __MMX__   (built-in)
// #define __SSE__   (built-in)
// #define __SSE2__  (built-in)
#define __SSE3__
#define __SSSE3__
#define __AVX__
#define __AVX2__
// #define __LZCNT__   // http://en.wikipedia.org/wiki/Bit_Manipulation_Instruction_Sets
// #define __BMI__     // http://en.wikipedia.org/wiki/Bit_Manipulation_Instruction_Sets
// #define __BMI2__    // http://en.wikipedia.org/wiki/Bit_Manipulation_Instruction_Sets
// #define __FMA__     // http://en.wikipedia.org/wiki/FMA_instruction_set
// #define __F16C__    // http://en.wikipedia.org/wiki/Bit_Manipulation_Instruction_Sets
// #define __RTM__     // 
// #include <immintrin.h>

using namespace std;

int main(int argc, char**argv)
{
#define M(var)  cout << "sizeof(" #var ")\t= " << sizeof(var) << "\n";

  M(void*)
  M(size_t)
  M(streampos)
  M(int)
  M(char)
  M(long)
  M(long long)
  M(float)
  M(double)
  M(long double)
  M(short)
  M(bool)
  M(size_t)
  M(int8_t)
  M(int16_t)
  M(int32_t)
  M(int64_t)
  // M(int128_t)  // 2014 not yet, maybe later...

  
  // M( __m64 )      // MME
  // M( __m128 )     // SSE
  // M( __m256 )     // AVX
  // M( __m512 )  // AVX-512
}
