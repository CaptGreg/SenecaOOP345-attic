//  this 64-bit 5GB program runs on 4GB mem computer using virtual memory (swap space)

#ifdef __cplusplus
        #include <iostream>
        using namespace std;

        #define P(T) cout << "sizeof(" #T ")=" << sizeof(T) << "\n";
#else
        #include <stdio.h>
        #define P(T) printf("sizeof(" #T ")=%lu\n", (long) sizeof(T) );
#endif


// char big_buf[5L * 1000L * 1000L * 1000L];   //  5 Gigbytes, see what the compiler generates in 32-bit mode
char big_buf[5LL * 1000LL * 1000LL * 1000LL];  //  5 Gigbytes guaranteed in 32-bit mode


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
