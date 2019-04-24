// g++-8 -Wall -std=c++17  filesize.cpp -o filesize -lstdc++fs && ./filesize filesize.cpp

#include <iostream>                // cout
#include <fstream>                 // ifstream

// g++-7: #include <experimental/filesystem> + std::experimental::filesystem
// g++-8: #include <filesystem> + std::filesystem

// #include <experimental/filesystem> // link with -lstdc++fs library 
#include <filesystem> // link with -lstdc++fs library 

using namespace std;

int64_t filesize(char*f)
{
  ifstream i(f, ios::in | ios::binary | ios::ate);
  return i.good()? uint64_t(i.tellg()): -1;
}

int main(int argc, char**argv)
{
  for(int arg = 1; arg < argc; arg++) {
    cout << "DIY filesize(" << argv[arg] << ") = " << filesize(argv[arg]) << "\n";
    try {
        // const auto fsize = std::experimental::filesystem::file_size(argv[arg]); 
        const auto fsize = std::filesystem::file_size(argv[arg]); 

        // cout <<  "std::filesystem::file_size(" << argv[arg] << ") = " << fsize << "\n";
        cout <<  "std::file_size(" << argv[arg] << ") = " << fsize << "\n";
    } 
    // catch(std::experimental::filesystem::filesystem_error& ex) 
    catch(std::filesystem::filesystem_error& ex) 
    {
        std::cout << ex.what() << '\n';
        // filesystem error: cannot get file size: No such file or directory [xxxyyyxxx]
    }
  }
}
