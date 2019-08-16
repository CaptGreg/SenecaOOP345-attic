// https://www.bfilipek.com/2019/04/dir-iterate.html
// using methods from https://en.cppreference.com/w/cpp/filesystem/is_directory

// 2019-08-16 tested
// g++-9.2   -Wall -std=c++17  dirtree.cpp -o dirtree
// clang++-8 -Wall -std=c++17  dirtree.cpp -o dirtree

#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char*argv[])
{
  const fs::path pathToShow{ argc >= 2 ? argv[1] : fs::current_path() };
  for (const auto& entry : fs::directory_iterator(pathToShow)) {
      auto filenameStr = entry.path().filename().string();
      if ( fs::is_directory(entry.symlink_status()) )  // cached status from directory entry
        std::cout << "dir:  " << filenameStr << '\n';
      else
      if ( fs::is_regular_file(entry.symlink_status()) )  // cached status from directory entry
        std::cout << "file: " << filenameStr << '\n';
      else
          std::cout << "??    " << filenameStr << '\n';
  }
}
