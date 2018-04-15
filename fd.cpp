// https://stackoverflow.com/questions/2746168/how-to-construct-a-c-fstream-from-a-posix-file-descriptor

// #include <cstdio>           // FILE*
#include <ext/stdio_filebuf.h>
#include <iostream>
#include <fstream>
#include <string>
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main()
{
    // 1. write a test file
    ofstream ofs("test.txt", ios::trunc);
    ofs << "Writing to a basic_ofstream object...\n";
    ofs.close();

    // 2. open the file returning a fd, a posix_handle if you prefer
  #if 0
    int fd = fileno(::fopen("test.txt", "r"));   // one line
    // FILE* fp =  ::fopen("test.txt", "r");     // two lines 1 of 2
    // int fd = fileno(fp);                      // two lines 2 of 2
  #else
    int fd = open("test.txt", O_RDONLY);
  #endif
    cout << "fd(posix_handle)=" << fd << "\n";

    // 3. convert the fd to a filebuf, something istream ctor can understand
    __gnu_cxx::stdio_filebuf<char> filebuf(fd, std::ios::in);
    // NO CAN DO WITH STD C++ 'filebuf'
    // filebuf filebuf();
    // filebuf.open(fd, std::ios::in);
    // std::filebuf::open
    //     filebuf* open (const char* filename,  ios_base::openmode mode);
    //     filebuf* open (const string filename, ios_base::openmode mode);

    // 4. attach the fd as a istream stream
    istream is(&filebuf);  // istream constructor

    // 5. print the test file
    string line;
    while(getline(is, line))
      cout << "line: " << line << std::endl;

    return 0;
}
