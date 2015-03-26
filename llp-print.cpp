#include <fstream>
#include <iostream>
#include <string>

#include "llp.h"

using namespace std;


int main(int argc, char**argv) 
{
    const char *filename =  "llp.dat";

    LLPersistence llp(filename);

    llp.print();
}
