// How much memroy can we allocate?
// Anecdotal experience suggests under 2GB with the 32-bit Microsoft C++ compiler
// Let's measure it.

#include <iostream>
#include <iomanip>   // setprecision
#include <exception> // std::exception
using namespace std;

int app(int argc, char* argv[])
{
 size_t lo =  0;
 size_t hi = ~0;
 size_t mid;
 do {
   mid = (lo + hi) / 2;
   try {
     // cout << lo << "," << mid << "," << hi << "\n";

     uint8_t* b= new uint8_t[mid];
     delete [] b;
     cout << mid << " byte allocation sucessful\n"; 

     lo = mid;
   } catch(exception& e) {
     cerr << mid << " bytes - " << e.what() << "\n"; 
     hi = mid;
   }
 } while(lo != hi);

 cout << "This " << 8 * sizeof(void*) << "-bit program successfully allocated " 
      << mid << " bytes, " << setprecision(2) << 1.e-9 * mid << " GBytes\n";

  return 0;
}

int main(int argc, char* argv[])
{
  return app(argc, argv);
}
