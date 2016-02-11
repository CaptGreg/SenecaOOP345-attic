#include <fstream>
#include <iostream>
#include <string>

#include "llp.h"

using namespace std;


int main(int argc, char**argv) 
{
    const char *filename =  "llp.dat";

    LLPersistence llp(filename);

    void *data;
    size_t length;

    // pop 
    llp.pop( data, length);

    if(length) {
        cout << "print: " << length << " bytes [";
        cout.write((char*) data,length);
        cout << "]" << "\n";
    } else {
        cout << "list is empty!\n";
    }
}
