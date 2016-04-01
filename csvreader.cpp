#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string& trim(string& s)
{
  while(not s.empty() and s[0] == ' ')
    s.erase(0, 1);

  while(not s.empty() and s[s.size()-1] == ' ')
    s.erase(s.size()-1, 1);

  return s;
}

void csvReader(char* filename, char delim, vector< vector<string> > &csv)
{
  // cout << "filename = " << filename << ", delim=" << delim << "\n";

  ifstream is(filename);
  if(is) {
    string line;
    vector<string> fields;

    while(getline(is, line)) {
      auto cr = line.find('\r');
      if(cr != std::string::npos) line.erase(cr, 1);
      // cout << "read -->" << line << "<--\n";

      string field;
      for(size_t i = 0; i < line.size(); i++) {
         if(line[i] != delim) {
           field += line[i];
         } else {
            trim(field);
            // cout << "field size=" << field.size() << " <<" << field << ">>\n";
            fields.push_back(field);
            field.clear();
         }
      }
      trim(field);
      // cout << "field size=" << field.size() << " <<" << field << ">>\n";
      fields.push_back(field);
      csv.push_back(fields);
      fields.clear();

    }

    is.close();
  } else {
    cerr << "cannot open file " << filename << "\n";
  }
}

int main(int argc, char**argv)
{
  char *filename;
  char delim;

  if(argc != 3) {
    cerr << "usage: " << argv[0] << " file delimiter-char\n";
    return 2;
  }

  filename = argv[1];
  delim    = argv[2][0];

  // cout << "filename = " << filename << ", delim=" << delim << "\n";
  vector< vector<string> > csv;
  csvReader(filename, delim, csv);

  for(size_t line = 0; line < csv.size(); line++) {
    for(size_t field =  0; field < csv[line].size(); field++) {
      cout << csv[line][field];
    }
    cout << "\n";
  }
  return 0;
}
