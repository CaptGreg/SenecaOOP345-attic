#include <fstream>
#include <iostream>
#include <string>
#include <cstring>	// strlen

#include "llp.h"

using namespace std;


int main(int argc, char**argv) 
{
    const char *filename =  "llp.dat";

    LLPersistence llp(filename);

    char *p;

    p = const_cast<char *> ("one");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("two");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("three");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("four");
    llp.push( (void*) p, strlen(p) );

    p = const_cast<char *> ("five");
    llp.push( (void*) p, strlen(p) );

    // push args
    for(int i = 0; i < argc; i++)
        llp.push( (void*) argv[i], strlen(argv[i]) );

    llp.print();
}
