#include <iostream>
#include <chrono>
using namespace std;

// AoS vs SoA are hot topics.  GPUs prefer AoS.

// Let's measure which is faster.

const int N = 100*1000*1000;
struct {
    uint8_t r, g, b;
} AoS[N];

struct {
    uint8_t r[N];
    uint8_t g[N];
    uint8_t b[N];
} SoA;

#if 0
http://stackoverflow.com/questions/8377667/layout-in-memory-of-a-struct-struct-of-arrays-and-array-of-structs-in-c-c/8377717#8377717

These have the following trade-offs:

    AoS tends to be more readable to the programmer as each "object" is kept together.
    AoS may have better cache locality if all the members of the struct are accessed together.
    SoA could potentially be more efficient since grouping same datatypes together sometimes exposes vectorization.
    In many cases SoA uses less memory because padding is only between arrays rather than between every struct.
#endif

int main()
{
  class Timer { // C++11 chrono stop-watch timer class. Needs "#include <chrono>".
    chrono::time_point<chrono::high_resolution_clock> start;
    chrono::time_point<chrono::high_resolution_clock> stop;
  public:
   void Start() { start  = chrono::high_resolution_clock::now(); }
   void Stop()  { stop   = chrono::high_resolution_clock::now(); }
   uint64_t nsecs() {
      typedef chrono::duration<int,nano> nanosecs_t ;
      nanosecs_t duration_get( chrono::duration_cast<nanosecs_t>(stop-start) ) ;
      uint64_t ns = duration_get.count();
      return ns;
   }
  };

  Timer t;
  #define P(X) cout << "sizeof(" << #X << ")=" << sizeof(X) << "\n";

  P(SoA)
  t.Start();
  for(int i = 0 ; i < N; i++) {
     SoA.r[i] = SoA.g[i] = SoA.b[i] = i; 
  }
  t.Stop();
  uint64_t tSoA = t.nsecs();
  cout << "SoA " << tSoA << " ns\n";

  P(AoS)
  t.Start();
  for(int i = 0 ; i < N; i++) {
     AoS[i].r = AoS[i].g = AoS[i].b = i; 
  }
  t.Stop();
  uint64_t tAoS = t.nsecs();
  cout << "AoS " << tAoS << " ns\n";

  cout << "Ratio AoS/SoA = " << double(tAoS) / double(tSoA) << "\n";
}
