#include <iostream>
#include <chrono>
using namespace std;

// AoS : Array of Structures
// struct { uint8_t r, g, b; } AoS[N];

// SoA : Structure of Arrays
// struct { uint8_t r[N]; uint8_t g[N]; uint8_t b[N]; } SoA;

// AoS vs SoA are hot topics.  GPUs prefer AoS.

// Let's measure which is faster.
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

const int N = 100*1000*1000+1;
struct {
    uint8_t r, g, b;
} AoS[N];

struct {
    uint8_t r[N];
    uint8_t g[N];
    uint8_t b[N];
} SoA;


int main()
{
  Timer t;

  cout << "\n";

  #define P(X) cout << "sizeof(" << #X << ")=" << sizeof(X) << "\n";

  cout << "struct { uint8_t r, g, b; } AoS[N];\n";
  P(AoS)
  t.Start();
  for(int i = 0 ; i < N; i++) {
     AoS[i].r = AoS[i].g = AoS[i].b = i; 
  }
  t.Stop();
  uint64_t tAoS = t.nsecs();
  cout << "AoS " << tAoS << " ns\n";

  cout << "\n";

  cout << "struct { uint8_t r[N]; uint8_t g[N]; uint8_t b[N]; } SoA;\n";
  P(SoA)
  t.Start();
  for(int i = 0 ; i < N; i++) {
     SoA.r[i] = SoA.g[i] = SoA.b[i] = i; 
  }
  t.Stop();
  uint64_t tSoA = t.nsecs();
  cout << "SoA " << tSoA << " ns\n";

  cout << "\n";

  cout << "Ratio AoS/SoA = " << double(tAoS) / double(tSoA) << "\n";

  cout << "\n";
}
