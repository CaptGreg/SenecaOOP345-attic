#ifdef __cplusplus
  #include <iostream>
  using namespace std;
#else
  #include <stdio.h>
#endif

int main()
{
  #ifdef __cplusplus
    cout << "CPP code: Running " << 8*sizeof(void*) << "-bit code, sizeof(long double) is " 
    << sizeof(long double) << " BYTES, or " 
    << 8*sizeof(long double) << " bits"
    << "\n";
  #else 
    printf( "C code: Running %d" , (int) (8*sizeof(void*)) );
    printf( "-bit code, sizeof(long double) is %d", (int) (sizeof(long double)) );
    printf( " BYTES, or %d" , (int) (8*sizeof(long double)) );
    printf( " bits");
      printf( "\n");
    
  #endif
}

