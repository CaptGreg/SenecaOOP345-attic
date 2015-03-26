#include <iostream>
#include <string>
using namespace std;

#include <sys/types.h>    // stat
#include <sys/stat.h>     // stat
#include <chrono>         // std::strftime + std::localtime

#include <dirent.h>

int main(int argc, char**argv)
{

  string path("./");
  if(argc > 1) path = string(argv[1]) + "/";

// http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c

  DIR *dir = opendir(path.c_str());
  class dirent *ent;
  class stat sb;
  while ((ent = readdir(dir)) != NULL) {
      const string file_name = ent->d_name;
      const string full_file_name = path + file_name;

      if (file_name[0] == '.')
          continue;

      if (stat(full_file_name.c_str(), &sb) == -1)
          continue;

      const bool is_directory = (sb.st_mode & S_IFDIR) != 0;

      if (is_directory)
          continue;

      cout << full_file_name << "\n";
      cout << "   file size:           " << sb.st_size << " bytes\n";

      auto ptime = [] (const string text, const std::time_t* t) {
        char b[100];
        if (std::strftime(b, sizeof(b), "%F %T", std::localtime(t))) {
          cout << text << b << "\n";
        }
      };
      ptime("   status change:       ", &sb.st_ctime);
      ptime("   file access:         ", &sb.st_atime);
      ptime("   file modification:   ", &sb.st_mtime);
  }
  closedir(dir);

  return 0;
}  
