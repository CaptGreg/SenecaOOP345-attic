// GB hacked a bit from http://www.cplusplus.com/forum/unices/3548/

#include <algorithm>
#include <string>
#include <vector>

#include <dirent.h>
#include <sys/types.h>
#include <errno.h>  // GB

// read_directory()
//   Return an ASCII-sorted vector of filename entries in a given directory.
//   If no path is specified, the current working directory is used.
//
//   Always check the value of the global 'errno' variable after using this
//   function to see if anything went wrong. (It will be zero if all is well.)
//
std::vector <std::string> read_directory( const std::string& path = std::string() )
{
  std::vector <std::string> result;
  errno = 0;
  DIR* dp = opendir( path.empty()? ".": path.c_str() );
  if(errno) { perror("read_directory: opendir failed"); return result; } // GB
  if( dp ) {
    while(true) {
      errno = 0;
      dirent* de = readdir( dp );
      if(errno) { perror("read_directory: readdir failed"); return result; } // GB
      if (de == NULL) break;
      result.push_back( std::string( de->d_name ) );
    }
    closedir( dp );
    std::sort( result.begin(), result.end() );
  }
  return result;
}

#include <iostream>
int main(int argc,char*argv[]) // GB - driver
{

  std::string folder =  argc>1? argv[1]: ".";

  std::vector<std::string> files( move( read_directory( folder ) ) );

  for(const auto file: files)
    std::cout << file << "\n";
  
}
