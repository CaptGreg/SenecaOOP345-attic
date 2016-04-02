#include <vector>
#include <string>
#include <iostream>
#include <fstream>

// using namespace std;

std::string& trim(std::string& s, char trimChar = ' ');
std::string& trim(std::string& s, char trimChar)
{
  while(not s.empty() and s[0] == trimChar)
    s.erase(0, 1);

  while(not s.empty() and s[s.size()-1] == trimChar)
    s.erase(s.size()-1, 1);

  return s;
}

#define PROCESS_DOUBLE_QUOTES
void csvReader(char* filename, char delim, std::vector< std::vector<std::string> > &csv)
{
  std::ifstream is(filename);
  if(is) {
    std::string line;
    std::vector<std::string> fields;

    while(getline(is, line)) {
      auto cr = line.find('\r');
      if(cr != std::string::npos) line.erase(cr, 1);

      std::string field;
      for(size_t i = 0; i < line.size(); i++) {
      #ifdef PROCESS_DOUBLE_QUOTES
         if(line[i] == '"') {
           field += line[i];        // copy 1st "
           for( i++ ; i < line.size() ; i++ ) {
             field += line[i];
             if(line[i] == '"')     // found 2nd "
               break;
           }
         }
         else 
       #endif
         if(line[i] != delim) {
           field += line[i];
         } else {
            trim(field);
            fields.push_back(field);
            field.clear();
         }
      }
      trim(field);
      fields.push_back(field);
      csv.push_back(fields);
      fields.clear();
    }
    is.close();
  } else {
    std::cerr << "cannot open file " << filename << "\n";
  }
}

int main(int argc, char**argv)
{
  char *filename;
  char delim;

  if(argc != 3) {
    std::cerr << "usage: " << argv[0] << " file delimiter-char\n";
    return 2;
  }

  filename = argv[1];
  delim    = argv[2][0];

  std::vector< std::vector<std::string> > csv;
  csvReader(filename, delim, csv);

  for(size_t line = 0; line < csv.size(); line++) {

    std::cout << csv[line].size() << " fields:";
    for(size_t field =  0; field < csv[line].size(); field++) {
      std::cout << " <<" << csv[line][field] << ">>";
    }
    std::cout << "\n";
  }
  return 0;
}
