// !!!  compile flags -mmmx + -msse flags for mmx + sse intrinsics
// intrinsic list https://github.com/emoon/ps2dev_tools/blob/master/osx/ps2dev/ee/info/gcc.info-3

// g++ -Wall -std=c++11  -ggdb -mmmx -msse  vectorization.cpp -o vectorization && ./vectorization
// clang++ complains about the __builtin_XXX functions


// GCC vectorization has been supported since 2009, experimentally since 2006/7

// To enable native vectorization, x86/SSE, PPC/ALTIVEC, ARM/NEON
// use, g++ -Ofast -msse3 -ftree-vectorize -ftree-vectorizer-verbose=2

// https://gcc.gnu.org/wiki/VectorizationTasks

// examples from
// https://gcc.gnu.org/projects/tree-ssa/vectorization.html

// format code with
// bcpp -bcl  -s  -i 2 vectorization.cpp > x ; mv x vectorization.cpp
// bcpp flags
// bcpp    -bcl = K&R style
// bcpp    -s   =  spaces (not tabs)
// bcpp    -i 2 = indent 2 spaces

#include <iostream>

#include <cstdlib>
#include <cmath>
#ifdef __MSVC__
  #include <intrin.h>
#else                            // GB assume clang++/g++
  #include <mmintrin.h>
  #include <emmintrin.h>
#endif

// https://gcc.gnu.org/onlinedocs/gcc-3.4.0/gcc/Vector-Extensions.html
// V4XX where XX is:
//   QI An integer that is as wide as the smallest addressable unit, usually 8 bits.
//   HI An integer, twice as wide as a QI mode integer, usually 16 bits.
//   SI An integer, four times as wide as a QI mode integer, usually 32 bits.
//   DI An integer, eight times as wide as a QI mode integer, usually 64 bits.
//   SF A floating point value, as wide as a SI mode integer, usually 32 bits.
//   DF A floating point value, as wide as a DI mode integer, usually 64 bits.

typedef int V4SI __attribute__ ((mode(V4SI)));
// warning: specifying vector types with __attribute__ ((mode)) is deprecated [-Wattributes]
// warning: use __attribute__ ((vector_size)) instead [-Wattributes]

// vector of four single floats/int32_t
#define VECTOR_SIZE         4
typedef float v4sf __attribute__ ((vector_size(sizeof(float)*VECTOR_SIZE)));   // 128-bit SSE
typedef int   v4si __attribute__ ((vector_size(sizeof(int32_t)*VECTOR_SIZE))); // 128-bit MMX
// or ...     v4si __attribute__ ((vector_size(sizeof(int)*VECTOR_SIZE)));     // 128-bit MMX

// Currently, GCC will allow using the following operators on these types: +, -, *, /, unary minus, ^, |, &, ~.

typedef union f4vector
{
  v4sf    v;
  float   f[VECTOR_SIZE];
} f4vector;

void printf4vector(v4sf* e) // GB wrote this
{
  f4vector* f4 = reinterpret_cast< f4vector* >(e);

  std::cout << f4->f[0];
  for( int i = 1; i < VECTOR_SIZE; i++)
    std::cout << ", " << f4->f[i];
  std::cout << "\n";
}

typedef union i4vector
{
  v4si    v;
  int     i[VECTOR_SIZE];
} i4vector;

// typedef int v4si __attribute__ ((mode(V4SI)));

void printi4vector(v4si* e) // GB wrote this
{
  i4vector* i4 = reinterpret_cast< i4vector* >(e);

  std::cout << i4->i[0];
  for( int i = 1; i < VECTOR_SIZE; i++)
    std::cout << ", " << i4->i[i];
  std::cout << "\n";
}


int main()
{
  union f4vector a, b, c;

  a.f[0] = 1.1; a.f[1] =  2; a.f[2] = 3;  a.f[3] = 4.0;
  b.f[0] = 5.0; b.f[1] =  6; b.f[2] = 7;  b.f[3] = 8.0;
  c.f[0] = 9.1; c.f[1] = 10; c.f[2] = 11; c.f[3] = 12.3;

  v4sf tmp = __builtin_ia32_mulps (a.v, b.v);
  v4sf foo = __builtin_ia32_addps(tmp, c.v);

  std::cout << "calculate foo = (a * b) + c:\n";
  std::cout << "a   = "; printf4vector(&a.v);
  std::cout << "b   = "; printf4vector(&b.v);
  std::cout << "c   = "; printf4vector(&c.v);
  std::cout << "foo = "; printf4vector(&foo);

  v4si A ={1,2,3,4};
  v4si B ={5,6,7,8};
  v4si C;

  std::cout << "A     : "; printi4vector(&A);
  std::cout << "B     : "; printi4vector(&B);

  C = A + B;
  std::cout << "C=A+B : "; printi4vector(&C);
  C = A - B;
  std::cout << "C=A-B : "; printi4vector(&C);
  C = B - A;
  std::cout << "C=B-A : "; printi4vector(&C);
  C = A * B;
  std::cout << "C=A*B : "; printi4vector(&C);
  C = B / A;
  std::cout << "C=B/A : "; printi4vector(&C);
  C = A / B;
  std::cout << "C=A/B : "; printi4vector(&C);

  v4sf fA ={1,2,3,4.5};
  v4sf fB ={5,6,7,8.5};
  v4sf fC;

  std::cout << "fA       : "; printf4vector(&fA);
  std::cout << "fB       : "; printf4vector(&fB);

  fC = fA + fB;
  std::cout << "fC=fA+fB : "; printf4vector(&fC);
  fC = fA - fB;
  std::cout << "fC=fA-fB : "; printf4vector(&fC);
  fC = fB - fA;
  std::cout << "fC=fB-fA : "; printf4vector(&fC);
  fC = fA * fB;
  std::cout << "fC=fA*fB : "; printf4vector(&fC);
  fC = fA * fA;
  std::cout << "fC=fA*fA : "; printf4vector(&fC);
  fC = fB * fB;
  std::cout << "fC=fB*fB : "; printf4vector(&fC);
  fC = fA / fB;
  std::cout << "fC=fA/fB : "; printf4vector(&fC);
  fC = fB / fA;
  std::cout << "fC=fB/fA : "; printf4vector(&fC);

  /* ... */
  extern void vectorcode();
  vectorcode(); // seems to execute --- no segfault.  How about that?
}


//////////////////////////////////////////////////////////////
// GB The following examples are from                       //
// https://gcc.gnu.org/projects/tree-ssa/vectorization.html //
// GB cleaned up a bit to make it compile                   //
// GB may not execute, most likely won't, does not segfault //
//////////////////////////////////////////////////////////////

void vectorcode ()
{
  const int N = 512;
  const int M = 1024;
  const int K = 32;

  {
    // example1:

    int a[N], b[N], c[N];
    // void foo ()
    {
      int i;

      for (i=0; i<N; i++) {
        a[i] = b[i] + c[i];
      }
    }
  }

  {
    // example2:

    int a[N], b[N], c[N];
    // void foo (int n, int x)
    int n;
    int x;
    {
      int i;

      /* feature: support for unknown loop bound  */
      /* feature: support for loop invariants  */
      for (i=0; i<n; i++) {
        b[i] = x;
      }

      /* feature: general loop exit condition  */
      /* feature: support for bitwise operations  */
      i = 0;
      while (n--) {
        a[i] = b[i]&c[i]; i++;
      }
    }
  }

  {
    // example3:

    typedef int aint __attribute__ ((__aligned__(16)));
    // void foo (int n, aint * __restrict__ p, aint * __restrict q)
    int n; aint * __restrict__ p; aint * __restrict q;
    {

      /* feature: support for (aligned) pointer accesses.  */
      while (n--) {
        *p++ = *q++;
      }
    }
  }

  {
    // example4:

    typedef int aint __attribute__ ((__aligned__(16)));
    int a[N], b[N], c[N];
    // void foo (int n, aint * __restrict__ p, aint * __restrict__ q)
    int n; aint * __restrict__ p; aint * __restrict__ q;
    {
      int i,j, MAX =  1234;

      /* feature: support for (aligned) pointer accesses  */
      /* feature: support for constants  */
      while (n--) {
        *p++ = *q++ + 5;
      }

      /* feature: support for read accesses with a compile time known misalignment  */
      for (i=0; i<n; i++) {
        a[i] = b[i+1] + c[i+3];
      }

      /* feature: support for if-conversion  */
      for (i=0; i<n; i++) {
        j = a[i];
        b[i] = (j > MAX ? MAX : 0);
      }
    }
  }

  {
    // example5:

    struct a
    {
      int ca[N];
    } s;
    // void foo (x)
    int x;
    {
      int i;

      for (i = 0; i < N; i++) {
        /* feature: support for alignable struct access  */
        s.ca[i] = 5;
      }
    }
  }

  {
    // example6 (gfortran):

    // DIMENSION A(1000000), B(1000000), C(1000000)
    // READ*, X, Y
    // A = LOG(X); B = LOG(Y); C = A + B
    // PRINT*, C(500000)
    // END
  }

  {
    // example7:

    int a[N], b[N];
    // void foo (int x)
    int x;
    {
      int i;

      /* feature: support for read accesses with an unknown misalignment  */
      for (i=0; i<N; i++) {
        a[i] = b[i+x];
      }
    }
  }

  {
    // example8:

    int a[M][N];
    // void foo (int x)
    int x;
    {
      int i,j;

      /* feature: support for multidimensional arrays  */
      for (i=0; i<M; i++) {
        for (j=0; j<N; j++) {
          a[i][j] = x;
        }
      }
    }
  }

  {
    // example9:

    unsigned int ub[N], uc[N];
    // void foo ()
    {
      int i;

      /* feature: support summation reduction.
         note: in case of floats use -funsafe-math-optimizations  */
      unsigned int udiff = 0;
      for (i = 0; i < N; i++) {
        udiff += (ub[i] - uc[i]);
      }
    }
  }

  {
    // example10:

    /* feature: support data-types of different sizes.
       Currently only a single vector-size per target is supported;
       it can accommodate n elements such that n = vector-size/element-size
       (e.g, 4 ints, 8 shorts, or 16 chars for a vector of size 16 bytes).
       A combination of data-types of different sizes in the same loop
       requires special handling. This support is now present in mainline,
       and also includes support for type conversions.  */

    short *sa, *sb, *sc;
    int *ia, *ib, *ic;
    int i;
    for (i = 0; i < N; i++) {
      ia[i] = ib[i] + ic[i];
      sa[i] = sb[i] + sc[i];
    }

    for (i = 0; i < N; i++) {
      ia[i] = (int) sb[i];
    }
  }

  {
    // example11:

    /* feature: support strided accesses - the data elements
       that are to be operated upon in parallel are not consecutive - they
       are accessed with a stride > 1 (in the example, the stride is 2):  */

    int i;
    float a[N/2], b[N], c[N], d[N/2];
    for (i = 0; i < N/2; i++) {
      a[i] = b[2*i+1] * c[2*i+1] - b[2*i] * c[2*i];
      d[i] = b[2*i] * c[2*i+1] + b[2*i+1] * c[2*i];
    }
  }

  {
    // example12: induction:

    int i;
    int a[N];
    for (i = 0; i < N; i++) {
      a[i] = i;
    }
  }

  {
    // void foo()
    {
      // example13: outer-loop:

      int i,j;
      int diff;
      int a[M][N];
      int b[M][N];
      int out[M];
      for (i = 0; i < M; i++) {
        diff = 0;
        for (j = 0; j < N; j+=8) {
          diff += (a[i][j] - b[i][j]);
        }
        out[i] = diff;
      }
    }
  }

  {
    // example14: double reduction:

    int i,j,k;
    int in[M][N];
    int coeff[M][N];
    int out[K];
    int sum;
    for (k = 0; k < K; k++) {
      sum = 0;
      for (j = 0; j < M; j++)
        for (i = 0; i < N; i++)
          sum += in[i+k][j] * coeff[i][j];

      out[k] = sum;
    }
  }

  {
    // example15: condition in nested loop:

    int i,j;
    int a[N+1];
    int c[N];
    int x_in[M];
    int x_out[M];
    for (j = 0; j < M; j++) {
      int x = x_in[j];
      int curr_a = a[0];

      for (i = 0; i < N; i++) {
        int next_a = a[i+1];
        curr_a = x > c[i] ? curr_a : next_a;
      }

      x_out[j] = curr_a;
    }
  }

  {
    // example16: load permutation in loop-aware SLP:
    int i;

    int  in[3*N];
    int  out[3*N];
    int* pInput  = in;
    int* pOutput = out;
    int  M00, M01, M02;
    int  M10, M11, M12;
    int  M20, M21, M22;
    for (i = 0; i < N; i++) {
      int a = *pInput++;
      int b = *pInput++;
      int c = *pInput++;

      *pOutput++ = M00 * a + M01 * b + M02 * c;
      *pOutput++ = M10 * a + M11 * b + M12 * c;
      *pOutput++ = M20 * a + M21 * b + M22 * c;
    }
  }

  {
    // example17: basic block SLP:

    // void foo ()
    {
      unsigned int in[N];
      unsigned int out[N];
      unsigned int *pin =  in;
      unsigned int *pout = out;

      *pout++ = *pin++;
      *pout++ = *pin++;
      *pout++ = *pin++;
      *pout++ = *pin++;
    }
  }

  {
    // example18: Simple reduction in SLP:

    int sum1 = 0;
    int sum2 = 0;
    int a[128];
    // void foo (void)
    {
      int i;

      for (i = 0; i < 64; i++) {
        sum1 += a[2*i];
        sum2 += a[2*i+1]; // max index = 63*2+1 = 127, need vector of 128 
      }
    }
  }

  {
    // example19: Reduction chain in SLP:

    int sum = 0;
    int a[128];
    // void foo (void)
    {
      int i;

      for (i = 0; i < 64; i++) {
        sum += a[2*i];
        sum += a[2*i+1]; // max index = 63*2+1 = 127, need vector of 128 
      }
    }
  }

  {
    // example20: Basic block SLP with multiple types, loads with different offsets, misaligned load, and not-affine accesses:

    // void foo (int * __restrict__ dst, short * __restrict__ src,
    // int h, int stride, short A, short B)
    int *   __restrict__   dst;
    short * __restrict__   src;
    int h;
    int stride;
    short A;
    short B;
    {
      int i;
      for (i = 0; i < h; i++) {
        dst[0] += A*src[0] + B*src[1];
        dst[1] += A*src[1] + B*src[2];
        dst[2] += A*src[2] + B*src[3];
        dst[3] += A*src[3] + B*src[4];
        dst[4] += A*src[4] + B*src[5];
        dst[5] += A*src[5] + B*src[6];
        dst[6] += A*src[6] + B*src[7];
        dst[7] += A*src[7] + B*src[8];
        dst += stride;
        src += stride;
      }
    }
  }

  {
    // example21: Backward access:

    // int foo (int *b, int n)
    int *b; int n;
    {
      int i, a = 0;

      for (i = n-1; i >= 0; i--)
        a += b[i];

      // return a;
    }
  }

  #if 0
  {
    // example22: Alignment hints:

    // void foo (int *out1, int *in1, int *in2, int n)
    int *out1; int *in1; int *in2; int n;
    {
      int i;

      // GB looks like __builtin_assume_aligned is a C99 gcc extension

      // https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
      //  Built-in Function: void *__builtin_assume_aligned (const void *exp, size_t align, ...)

      // This function returns its first argument, and allows the compiler to assume that the
      // returned pointer is at least align bytes aligned. This built-in can have either two
      // or three arguments, if it has three, the third argument should have integer type,
      // and if it is nonzero means misalignment offset. For example:

      // void *x = __builtin_assume_aligned (arg, 16);

      // means that the compiler can assume x, set to arg, is at least 16-byte aligned, while:

      // void *x = __builtin_assume_aligned (arg, 32, 8);

      // means that the compiler can assume for x, set to arg, that (char *) x - 8 is 32-byte aligned.

      int* out1 = __builtin_assume_aligned (out1, 32, 16);
      int* in1  = __builtin_assume_aligned (in1,  32, 16);
      int* in2  = __builtin_assume_aligned (in2,  32,  0);

      for (i = 0; i < n; i++)
        out1[i] = in1[i] * in2[i];
    }
  }
  #endif

  {
    // example23: Widening shift:

    // void foo (unsigned short *src, unsigned int *dst)
    unsigned short *src; unsigned int *dst;
    {
      int i;

      for (i = 0; i < N; i++)
        *dst++ = *src++ << 7;
    }
  }

  {
    // example24: Condition with mixed types:

    float a[N], b[N];
    int c[N];

    //void foo (short x, short y)
    short x; short y;
    {
      int i;
      for (i = 0; i < N; i++)
        c[i] = a[i] < b[i] ? x : y;
    }
  }

  {
    // example25: Loop with bool:

    float a[N], b[N], c[N], d[N];
    int j[N];

    // void foo (void)
    {
      int i;
      bool x, y;
      for (i = 0; i < N; i++) {
        x = (a[i] < b[i]);
        y = (c[i] < d[i]);
        j[i] = x & y;
      }
    }
  }
} // void vectorcode ()
