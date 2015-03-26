#include <iostream>
#include <fstream>

using namespace std;

void WriteFile(const char* fn)
{
  fstream of(fn, ios::out | ios::trunc);
  if(of) {
    of << "line1\n";
    of << "line2\n";
    of << "line3\n";
    of.close();
  }
}

void ReadCS(const char* fn) {
  cout << "ReadCS:\n";
  std::ifstream file(fn);
  if (!file)
    throw std::string("*** Failed to open file ") + std::string(fn) + std::string(" ***"); 
  while (file) {
    string s;
    file >> s;
    cout << s << "\n";
  }
  file.close();
}

void ReadJD(const char* fn) {
  cout << "ReadJD:\n";
  std::ifstream file(fn);
  if (!file)
    throw std::string("*** Failed to open file ") + std::string(fn) + std::string(" ***");
  while (!file.eof()) {
    string s;
    file >> s;
    cout << s << "\n";
  }
  file.close();
}

int main(int argc,char**argv)
{
  const char* fn="testof.dat";

  try {
    WriteFile(fn);
    ReadCS(fn);
    ReadJD(fn);
  } catch (const string& e) {
    cerr << "Threw: " << e << "\n";
  }
}

