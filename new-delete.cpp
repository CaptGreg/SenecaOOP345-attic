#include <iostream>
#include <exception>
#include <malloc.h>


using namespace std;

#if 1
void* operator new (std::size_t n)
{
  cout << "DIY new allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes)\n";
  void *p = malloc(n);
  cout << "DIY new allocate pointer = " << p << "\n";
  if(p) return p;
  cerr << "DIY new failed to allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes) - throwing std::bad_alloc()\n";
  throw std::bad_alloc();
}
void* operator new[] (std::size_t n)
{
  cout << "DIY new[] allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes)\n";
  // return new(n);
  void *p = malloc(n);
  cout << "DIY new[] allocate pointer = " << p << "\n";
  if(p) return p;
  cerr << "DIY new[] failed to allocate " << n  << " bytes (" << n*1e-9 << " GigaBytes) - throwing std::bad_alloc()\n";
  throw std::bad_alloc();
}

void operator delete(void * p)
{
  cout << "DIY delete: pointer=" << p << "\n";
  free(p);
}
void operator delete[](void * p)
{
  cout << "DIY delete[]: pointer=" << p << "\n";
  // ::delete p;
  free(p);
}
#endif

int main()
{

  cout << "main starting\n";

  atexit( [] { cout << "atexit callback: main completed\n"; } );

  try {
   uint8_t *m = nullptr;

   operator<< (cout, "freeing nullptr\n"); // call 'operator<<' overload function. Same as 'cout << "freeing nullptr\n";'
   free(m);
   operator<< (cout, "double free of nullptr\n");
   free(m);

   uint8_t *cpp = nullptr;

   operator<< (cout, "deleting nullptr\n");
   delete cpp;
   operator<< (cout, "double nullptr delete\n");
   delete [] cpp; // double delete

// cpp = new uint8_t[1000LL * 1000LL * 1000LL * 1000LL]; // 1 terabyte 1e12 bytes
   cpp = new uint8_t[4 * 1000LL * 1000LL * 1000LL];      // 4 gigabytes
 
   operator<< (cout, "delete 4GB\n");
   delete    cpp;

   operator<< (cout, "double delete of 4GB\n");
   delete [] cpp; // double delete

  } catch (std::exception& e) { 
    cerr << e.what() << "\n";
  } catch (decltype(nullptr)) {
    cerr << "threw decltype(nullptr)\n";
  } catch (...) {
    cerr << "threw ...\n";
  }
  cout << "end of main\n";
}
