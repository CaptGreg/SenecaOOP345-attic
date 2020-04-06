// Bartek's C++17 in Detail pdf
// as usual for Bartek, incomplete either syntax errors or missing functions

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void DisplayDirTree(const fs::path& pathToShow, int level)
{
  if (fs::exists(pathToShow) && fs::is_directory(pathToShow)) {
    auto lead = std::string(level * 3, ' ');
    for(int i = 0; i < level; i++)
      lead[3*i + 1] = '|';

    for (const auto& entry : fs::directory_iterator(pathToShow)) {
      auto filename = entry.path().filename();

      if (fs::is_directory(entry.status())) {
        std::cout << lead;
        std::cout << "[+] " << filename << "\n";
        DisplayDirTree(entry, level + 1);

      } else if (fs::is_regular_file(entry.status())) {
        // DisplayFileInfo(entry, lead, filename);   // function is missing
        auto err = std::error_code();
        std::cout << lead
                  << " " << fs::path(entry).filename() 
                  << " " << fs::file_size(fs::path(entry), err) 
                  << "\n";

      } else {
        std::cout << lead << " [?]" << filename << "\n";
      }
    }
  }
}

void DisplayDirectoryTree(const fs::path& pathToShow)
{
  DisplayDirTree(pathToShow, 0);
}

int main(int argc, char* argv[])
{
  const char* file = argv[0];

  fs::path pathToShow(file);
  auto err = std::error_code();

  std::cout << file << ":\n"
  << "  exists()        = " << fs::exists(pathToShow) << "\n"
  << "  root_name()     = " << pathToShow.root_name() << "\n"
  << "  root_path()     = " << pathToShow.root_path() << "\n"
  << "  relative_path() = " << pathToShow.relative_path() << "\n"
  << "  parent_path()   = " << pathToShow.parent_path() << "\n"
  << "  filename()      = " << pathToShow.filename() << "\n"
  << "  stem()          = " << pathToShow.stem() << "\n"
  << "  extension()     = " << pathToShow.extension() << "\n"
  << "  file_size()     = " << fs::file_size(pathToShow, err)  << "\n"
  ;

  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n";

  pathToShow = fs::path("."); 
  DisplayDirectoryTree(pathToShow);

  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n";

  for(auto& p: fs::recursive_directory_iterator("."))
      std::cout << p << "\n";

  std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++\n";
}
