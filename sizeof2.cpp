//  this 64-bit 3GB program runs on 4GB mem computer using virtual memory (swap space)

#ifdef __cplusplus
        #include <iostream>
        using namespace std;

        #define P(QUACK) cout << "CPP: sizeof(" #QUACK ")=" << sizeof(QUACK) << "\n";
#else
        #include <stdio.h>
        #define P(ARF) printf("C: sizeof(" #ARF ")=%lu\n", (long) sizeof(ARF) );
#endif


// char big_buf[5L * 1000L * 1000L * 1000L];   //  5 Gigbytes, see what the compiler generates in 32-bit mode
char big_buf[3LL * 1000LL * 1000LL * 1000LL];  //  3 Gigbytes guaranteed in 32-bit mode


int main(int argc, char ** argv)
{
        P(void*)
        P(size_t)
        P(long long)
        P(long)
        P(int)
        P(big_buf)

	return 0;
}
