// thread-pc-simple.cpp - a simple producer consumer threading model
// a single producer thread produces data for a single consumer thread.
// mutex free, spin wait on bufValid flag for buffer empty or available

#include <functional>  // function
#include <iostream>
#include <thread>      // thread, join, sleep_for chrono time units

#include <atomic>
#ifdef WIN32
#include <intrin.h>
#include <Windows.h>
#endif
using namespace std;

#define N 256

class PC {
    volatile long long       buf[N] = {0};
    volatile unsigned  buf_read_pointer = 0;
    volatile unsigned buf_write_pointer = 0;
    bool buf_empty() { 
        return ( buf_read_pointer == buf_write_pointer) ;
    }
    
    void buf_read(long long &x) { 
        while (buf_empty()) pause(); 
        x = buf[buf_read_pointer]; 
        buf_read_pointer = (buf_read_pointer+1) % N;
        
    }
    void buf_write(long long x ) 
    { 
        // if the read pointer is +1 of the write pointer, then the buffer is full. So 
        // stop writing until it is not . 
        while (((buf_read_pointer - buf_write_pointer + N ) %N)==1) pause();
        buf[buf_write_pointer] = x;
        buf_write_pointer = (buf_write_pointer+1 ) % N;
        // a buffer write should never make the buffer empty. This means it is full.
        
    }

    thread    pTid;
    thread    cTid;
    volatile bool      pAlive = false;
    volatile bool      cAlive = false;
    volatile uint64_t  dataWaitTSC = 0;
    volatile uint64_t  emptyWaitTSC = 0;

    typedef uint32_t T;

    volatile T sync;

    static const uint32_t STOP = 0x00000010;


    volatile uint64_t  t1 , t2;
    volatile uint64_t  latency_accum = 0;
    volatile uint32_t  counter = 0;
    volatile uint64_t  frequency = 0;


    static uint64_t RDTSC( ) {
        uint64_t tick;
#if _WIN32
        tick = __rdtsc();

#else

        asm volatile( "rdtsc\n\t"             // returns time-stamp counter in edx:eax
            "shl $32,%%rdx\n\t"     // shift the high dword in edx into rdx
            "or %%rdx, %0"          // or hidword from rdx with lodword in rax. 
            : "=a"( tick )      // output taken from rax
            :
            : "rdx" );
#endif
        return tick;
    }

    static void inline pause( ) { 
#if _WIN32
        _mm_pause();
#else
        asm( "pause" ); 
#endif
    }

    static uint64_t tscTimedFunction( function<void( void )> f ) {

        uint64_t start = RDTSC( );
        f( );
        uint64_t stop = RDTSC( );
        return start < stop ? stop - start : start - stop;  // RDTSC wraps
    }
   
    void producer( ) {
        pAlive = true;
        long long n = 0;

        do {
            emptyWaitTSC += tscTimedFunction( bind(&PC::buf_write, this,  n++));

            if ((sync & STOP)) break;
            t1 = RDTSC( );

        } while (!(sync & STOP) );
        cerr << "Producer Exiting" << endl;
        pAlive = false;
    }

    void consumer( ) {
        cAlive = true;
        long long n=0;
        do {

            dataWaitTSC += tscTimedFunction( bind(&PC::buf_read, this, n) );
            if ((sync & STOP)) break;


            t2 = RDTSC( );
            latency_accum += ( t2 - t1 );
            counter++;


        } while (! (sync & STOP));
        cerr << "Consumer Exiting" << endl;
        cAlive = false;
    }

public:

    void run( int ms ) {                            // run for 'ms' milliseconds

        cout << "Calibrating system timer" << endl;
        t1 = RDTSC( );
        // Sleep( 1000 ); // Windows function
        this_thread::sleep_for(chrono::milliseconds(1000));
        t2 = RDTSC( );
        frequency = t2 - t1;
        cout << "Frequency is " << frequency / 1E9 << " GHz" << endl;

        sync = 0;
        pTid = thread( &PC::producer , this );
        cTid = thread( &PC::consumer , this );

        cerr << "Waiting for pipeline to come up" << endl;
        while (pAlive == false || cAlive == false);  // wait for pipeline to come up
        cerr << "Running for " << ms << "ms" << endl;
        this_thread::sleep_for( chrono::milliseconds( ms ) );
        cerr << "Stopping" << endl;
        int counter0 = counter;
        while (! ( sync & STOP ))
        {
            sync |= STOP;
            cout << sync << ":" << counter - counter0 << "\r";

        }
        cout << "\n";

        cerr << sync << endl;

        while (pAlive == true || cAlive == true) cout << sync << "\r";  // wait for pipeline to come up

        pTid.join( );
        cTid.join( );

        cout << "buf=" << buf << ", " << 1. * counter << "\n";  // prints 6-8 Million on 2016 era I5 laptop
        cout << "producer wasted clocks  = " << emptyWaitTSC << ", " << 1. * emptyWaitTSC << "or " << emptyWaitTSC*1E3 / frequency << "ms\n";
        cout << "consumer wasted clocks  = " << dataWaitTSC << ", " << 1. *  dataWaitTSC << "or " << dataWaitTSC*1E3 / frequency << "ms\n";

        cout << "total pipeline latency = " << latency_accum << " " << latency_accum * 1. / frequency * 1000 << " ms for " << counter << " ops" << endl;
        cout << "average pipeline latency = " << latency_accum * 1. / frequency * 1E9 / counter << " ns/signal" << endl;
    }
}; // class PC

int main( int argc , char*argv [] ) {
    PC pc;
    unsigned time = 1000;

    if (argc>1)
    {
        time = atof( argv[ 1 ] ) * 1000;
    }
    pc.run( time );
    return 0;
}
