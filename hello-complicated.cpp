// http://www.cplusplus.com/forum/lounge/79437/

// GB an application of 
//  1. static_cast
//  2. std::endl
//  3. std::ostream operator<<

#include <iostream>

// http://www.cplusplus.com/reference/ostream/endl/
// ostream& endl (ostream& os);

// http://en.cppreference.com/w/cpp/io/manip/endl
// std::endl
//  
// C++
//  
// Input/output library
//  
// Input/output manipulators
//  
// Defined in header <ostream>
// 		
//   template< class CharT, class Traits >
//   std::basic_ostream<charT,traits>& endl( std::basic_ostream<CharT, Traits>& os );
// 
// Inserts a newline character into the output sequence os and flushes it as if by calling os.put(os.widen('\n')) followed by os.flush().
// 
// This is an output-only I/O manipulator, it may be called with an expression such as out << std::endl for any out of type std::basic_ostream.
// Notes
// 
// This manipulator may be used to produce a line of output immediately, e.g. when displaying output from a long-running process, logging activity of multiple threads or logging activity of a program that may crash unexpectedly. An explicit flush of std::cout is also necessary before a call to std::system, if the spawned process performs any screen I/O (a common example is std::system("pause") on Windows). In most other usual interactive I/O scenarios, std::endl is redundant when used with std::cout because any input from std::cin, output to std::cerr, or program termination forces a call to std::cout.flush(). Use of std::endl in place of '\n', encouraged by some sources, may significantly degrade output performance.
// 
// In many implementations, standard output is line-buffered, and writing '\n' causes a flush anyway, unless std::cout.sync_with_stdio(false) was executed. In those situations, unnecessary endl only degrades the performance of file output, not standard output.
// 
// The code samples on this wiki follow Bjarne Stroustrup in flushing the standard output only where necessary.
// 
// When an incomplete line of output needs to be flushed, the std::flush manipulator may be used.
// 
// When every character of output needs to be flushed, the std::unitbuf manipulator may be used.
// Parameters
// os 	- 	reference to output stream
// Return value
// 
// os (reference to the stream after manipulation) 

// http://www.cplusplus.com/reference/ostream/ostream/operator<</
// arithmetic types (1)	
//   ostream& operator<< (bool val);
//   ostream& operator<< (short val);
//   ostream& operator<< (unsigned short val);
//   ostream& operator<< (int val);
//   ostream& operator<< (unsigned int val);
//   ostream& operator<< (long val);
//   ostream& operator<< (unsigned long val);
//   ostream& operator<< (long long val);
//   ostream& operator<< (unsigned long long val);
//   ostream& operator<< (float val);
//   ostream& operator<< (double val);
//   ostream& operator<< (long double val);
//   ostream& operator<< (void* val);
// stream buffers (2)	
//   ostream& operator<< (streambuf* sb );
// manipulators (3)	
//   ostream& operator<< (ostream& (*pf)(ostream&));
//   ostream& operator<< (ios& (*pf)(ios&));
//   ostream& operator<< (ios_base& (*pf)(ios_base&));

// http://www.cplusplus.com/reference/ostream/ostream/operator-free/
// single character (1)	
//   ostream& operator<< (ostream& os, char c);
//   ostream& operator<< (ostream& os, signed char c);
//   ostream& operator<< (ostream& os, unsigned char c);
// character sequence (2)	
//   ostream& operator<< (ostream& os, const char* s);
//   ostream& operator<< (ostream& os, const signed char* s);
//   ostream& operator<< (ostream& os, const unsigned char* s);
// rvalue insertion (3)	
//   template<class charT, class traits, class T>
//   basic_ostream<charT,traits>&
//     operator<< (basic_ostream<charT,traits>&& os, const T& val);

int main()
{
   bool b = static_cast<bool> (std::endl (operator<< (std::cout, "#1 Hello, World!") ) );
   int  i = static_cast<int>  (b);

   std::endl (operator<< (std::cout, "#2 Hello, World!") );

   std::endl ( std::endl (operator<< (std::cout, "#3 Hello, World!") ) );

   operator<< (std::cout, "#4 Hello, World!\n");

   return static_cast<int> (static_cast<bool> (std::endl (operator<< (std::cout, "#5 Hello, World!") )) );
}
