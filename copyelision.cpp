// http://definedbehavior.blogspot.ca/2011/08/value-semantics-copy-elision.html


// elision
// noun
// 1The omission of a sound or syllable when speaking (as in I’m, let’s): the shortening of words by elision [count noun]: conversational elisions
// 1.1 [count noun] An omission of a passage in a book, speech, or film: the movie’s elisions and distortions have been carefully thought out
// 2The process of joining together or merging things, especially abstract ideas: unease at the elision of so many vital questions


#include <iostream>
using namespace std;

typedef double type;

void f( type arg ) {
  std::cout << arg << "\n";
}

type g() {
  return type();
}

void g1( type const & ref ) 
{ 
}

void g2( type arg ) 
{
  type copy(arg);
  // operate on copy // operate on arg
} 

type f2() {
  return type();
}
type g3( type arg ) {
  return arg;
}

int main() {
  type x;
  f( x );

  type const & ref = g();

  type x2 = g3( f2() );
}
