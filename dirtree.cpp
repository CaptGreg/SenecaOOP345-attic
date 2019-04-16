// https://www.bfilipek.com/2019/04/dir-iterate.html
// using methods from https://en.cppreference.com/w/cpp/filesystem/is_directory

// g++-8 -Wall -std=c++17  dirtree.cpp -o dirtree -lstdc++fs
// 2019-04-15 g++-8.2.0

#include <iostream>

#include <filesystem>            // not found
namespace fs = std::filesystem;  // error: ‘filesystem’ is not a namespace-name

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
