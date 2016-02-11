#include <iostream>
#include <string>
using namespace std;

#include "lx.h"

size_t count = 0;  // ::count

void Count(string& s)
{
  ::count ++;
}

int main(int argc, char**argv)
{
    List<string> l;
    l.pushTail("one pushTail");
    l.pushTail("two pushTail");
    l.pushTail("three pushTail");
    l.pushHead("zero pushHead");
    l.print();

    l.popTail();
    l.print();

    l.popTail();
    l.print();

    // this works
    l.walk( Count );
    cout << "::count=" << ::count << "\n";

    // these lambda's work (return type '-> void' not required)
    l.walk( [] (string& s) -> void { ::count++; } );
    cout << "::count=" << ::count << "\n";
    l.walk( [] (string& s) -> void { count++; } );
    cout << "::count=" << ::count << "\n";
    l.walk( []  (string& s) -> void { cout << "List walk with a []  lambda expression: " << s << "\n"; } );
    l.walk( [=] (string& s) -> void { cout << "List walk with a [=] lambda expression: " << s << "\n"; } );
    l.walk( [&] (string& s) -> void { cout << "List walk with a [&] lambda expression: " << s << "\n"; } );

    // using -sdt=c++11 flag with g++ (Ubuntu 4.8.2-19ubuntu1) 4.8.2

    // This lambda DOES NOT compile.
    // auto f = [] (string& s) { ::count++; } // This 'auto' statement compiles
    // l.walk( f ); // but generates missing ',' or ';' error on following statement

    size_t count = 0; // local variable count

    // these lambda's DO NOT compile

    // l.walk( [] (string& s) { count++; } ); // error: ‘count’ is not captured

    // l.walk( [&] (string& s) { count++; } );  // error 'count' not captured

    // l.walk( [&count] (string& s) { count++; } ); // error: no matching function for call to ‘List<std::basic_string<char> >::walk(main(int, char**)::__lambda4)’

    // l.walk( [::count] (string& s) { ::count++; } ); // error: expected identifier before ‘::’ token

    // l.walk( [count] (string& s) { count++; } ); // error: no matching function for call to ‘List<std::basic_string<char> >::walk(main(int, char**)::__lambda4)’

    // auto f = [count] (string& s) { count++; } // error: increment of read-only variable ‘count’
    // auto g1 = [&count] (string& s) { count++; }  // generates error: expected ‘,’ or ‘;’ before the following statement

    // auto g2 = [] (string& s) { ::count++; }

    // auto g1 = [&count] (string& s) { count++; } 
    // l.walk( g1 ); // error: expected ‘,’ or ‘;’ before ‘l’
    cout << "::count=" << ::count << "\n";
    cout << "count=" << count << "\n";

    return 0;
}
