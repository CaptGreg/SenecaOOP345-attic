// http://www.ganssle.com/tem/tem110.pdf
// Jack Ganssle, jack@ganssle.com 

// see https://raw.githubusercontent.com/dhylands/TimerUART/master/CBUF.h

/*
 * GB 
 *   1. NOTE C-Code, not C++
 *   2. converted unsigned char to uint8_t
 *   3. converted FifoLength macro to inline function
 *   4. converted get and put to return status and accept parameter for data
 *   5. converted 'uint8_t' datatype to 'typedef uint8_t T;'
 *   6. size_t of the index variables must be > FIFO max size
 *      NOTE the index varibles wrap and table access is (var & SIZE-1)
 *           *** no need to clamp index variables ***
 *   7. This code is a nice candiate for our single producer single consumer (P1C1) threaded queue
 *
 * A Fiendishly Clever Circular Buffer (for power of two buffer sizes)
 * Phil Ouellette sent along a circular buffer routine that at first looked 
 * horribly wrong. It doesn’t even look for the end of the buffer to wrap 
 * the pointers!  Then I looked deeper and saw the hidden cleverness. This
 * routine * is very efficient. 
 *
 * Phil tells me the code is derived from a Keil routine 
 * (http://www.keil.com/download/docs/intsio2.zip.asp), 
 * though his implementation is more compact and easier to read. 
 * Keil graciously put their version into the * public domain... 
 * but did caution me to say that they do not provide technical 
 * support for the code!  
 *
 * So the code that follows is my (GB) slight changes to 
 *   1. Phil’s code which is based on Keil’s. 
 *   2. Jack’s code which is based on Phils’s. 
 * Any errors are mine alone. 
 *
 * The nice thing about this implementation is that you never have to 
 * concern yourself with checking for the indexes wrapping or pointing 
 * past the end of the FIFO.  The only limitations are that your FIFO 
 * buffer size is limited to a max of 128 entries and must be a power 
 * of 2.  You can use larger FIFO buffers if you change the FifoHead 
 * and FifoTail to larger variable types (with a corresponding code 
 * space and run time impact).   The code uses an AND to wrap the 
 * pointers around. The modulo operator might be faster on some 
 * processors. FifoTail & (FIFO_SIZE-1) is, in this case, equivalent 
 * to FifoTail % FIFO_SIZE. 
 */


#include <iostream>
using namespace std;
// #include <cstdint.h>   // uint8_t for c-code, not needed for modern C++

/************************************************/ 
// Fifo.h  
// Public header file containing Definitions 
// and function prototypes. 
// FIFO_SIZE Must be a power of 2 (2, 4, 8, 16, 32, 64, or 128). 
#define FIFO_SIZE   64 
enum {FIFO_OK = 0, FIFO_FULL, FIFO_EMPTY}; // FifoPut/FifoGet Succeeded,  FifoPut Failed, FifoGet Failed

// data type
typedef uint8_t T;

// Function Prototypes  
void FifoInit(void); 
int FifoPut(T  element);  
int FifoGet(T* pElement);  
/**************************************************
*/  
// Fifo.c  
// Source level module  
// #include "FIFO.H" 
// Module Level Variables  
T  FifoBuffer[FIFO_SIZE]; // data type T
uint32_t FifoHead = 0;  // test with uint8_t so indices wrap at 255, uint32_t calculations more efficient
uint32_t FifoTail = 0;  // test with uint8_t so indices wrap at 255, uint32_t calculations more efficient, wraps at 2**32-1
inline uint32_t FifoLength() { return (FifoHead - FifoTail) & (FIFO_SIZE-1); }
inline bool     FifoEmpty () { return FifoHead == FifoTail; } // GB
inline void     FifoStats () { cout << "h,t,s=" <<  (uint32_t) FifoHead << " " << (uint32_t) FifoTail << " " << FifoLength(); } 
// Function to initialize of flush the Fifo  
void FifoInit(void) 
{ 
   FifoHead = 0; 
   FifoTail = 0; 
} 
// Function to add an entry to Fifo  
// If Fifo if full then function returns FIFO_FULL 
// else function adds entry, increments FifoHead and returns FIFO_OK.  
int FifoPut(T element) 
{ 
  if (FifoLength() >= FIFO_SIZE) return FIFO_FULL; 
  FifoBuffer[FifoHead++ & (FIFO_SIZE - 1)] = element; 
  return FIFO_OK; 
} 
// Function to get an entry from Fifo  
// If Fifo is empty then function returns FIFO_EMPTY  
// else function returns oldest entry in *pElement,
// increments FifoTail, and returns FIFO_OK.  
int FifoGet(T* pElement) 
{ 
  if (FifoEmpty()) return FIFO_EMPTY; 
  *pElement=FifoBuffer[FifoTail++ & (FIFO_SIZE-1)]; 
  return FIFO_OK; 
} 

void test()
{
  for(int i = 0; i < 26; i++)
    FifoPut('A' + i);
  for(int i = 0; i < 9; i++)
    FifoPut('0' + i);
  for(int i = 0; i < 26; i++)
    FifoPut('a' + i);

  uint8_t c;
  while(FifoGet(&c) == FIFO_OK)
    cout << c;
  cout << "\n";
}

void test2()
{
  for(int i = 0; i < 26; i++)
    FifoPut('A' + i);

  uint8_t c;
  while(FifoGet(&c) == FIFO_OK) {
    FifoStats();
    cout <<" '" << c << "' ";
  }
  cout << "\n";
}

// FIFO #2
// https://embedjournal.com/implementing-circular-buffer-embedded-c/
// GB convert to C++, using unique_ptr
#include <memory>  // unique_ptr
template <class T>
class Fifo2 {
    const int     maxlen;
    unique_ptr<T> buffer;
    int           head = 0;
    int           tail = 0;
public:
  Fifo2(int l = 128) : maxlen(l),  buffer( unique_ptr<T> ( new T[maxlen] ) ) {}

  bool empty() { return head == tail; }
  bool full() { 
      decltype(head) next = (head + 1) % maxlen;
      return next == tail;
  }

  int push(T&& data)
  {
      decltype(head) next = (head + 1) % maxlen;  // next is where head will point to after this push.

      if (next == tail)  // if head + 1 == tail, circular buffer is full
          return -1;

      buffer.get()[head] = move(data);
      head = next;
      return 0;
  }

  int pop(T& data)
  {
      if (empty())  // if head == tail, we don't have any data
          return -1;

      data = move(buffer.get()[tail]);
      tail = (tail + 1) % maxlen;
      return 0;
  }
}; 

void testFifo2(Fifo2<uint8_t> & f)
{  
  uint8_t c;

  for(int i = 0; i < 26; i++)
    f.push('A'+i);
  for(int i = 0; i < 9; i++)
    f.push('0'+i);
  for(int i = 0; i < 26; i++)
    f.push('a'+i);

  while(f.pop(c) == 0)
    cout << c;
  cout << "\n";
}

int main()
{
  FifoInit(); 

  test(); // repeat to force pointer wrap
  test();
  test();
  test();
  test();
  test();
  test();
  test();
  test();
  test();
  test();

  test();
  test();
  test();
  test();
  test();
  test();
  test();
  test();
  test();
  test();
  test();

  test2(); // noisy (verbose) test
  test2();
  test2();
  test2();
  test2();

  Fifo2<uint8_t> f;
  testFifo2(f); // repeat to force pointer wrap
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
  testFifo2(f);
}
