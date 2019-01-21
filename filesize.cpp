#include <iostream>                // cout
#include <fstream>                 // ifstream
#include <experimental/filesystem> // link with -lstdc++fs library 
// Verified Jan 2019 with g++ 7.3.0 (Ubuntu 7.3.0-27ubuntu1~18.04)

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
        const auto fsize = std::experimental::filesystem::file_size(argv[arg]); 
        cout <<  "std::filesystem::file_size(" << argv[arg] << ") = " << fsize << "\n";
    } catch(std::experimental::filesystem::filesystem_error& ex) {
        std::cout << ex.what() << '\n';
        // filesystem error: cannot get file size: No such file or directory [xxxyyyxxx]
    }
  }
}
