#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm> // find_if
#include <cctype>    // isspace

using namespace std;

// man fgets:
// char *fgets(char *s, int size, FILE *stream);
//       fgets() reads in at most one less than size characters from stream and 
//       stores them into the buffer pointed to by s.  
//
//       Reading stops after an EOF or a newline.  
//
//       If a newline is read, it is stored into the buffer.  
//       A terminating null byte ('\0') is stored after the last character in the buffer.
//
//       fgets() return s on success, and NULL on error or when end of file occurs while 
//       no characters have been read.


string& trim(string& str) // sigh, no built-in trim
{
  // A C++11 trim from 
  // http://www.codeproject.com/Articles/10880/A-trim-implementation-for-std-string

  str.erase(str.begin(), find_if(str.begin(), str.end(),
    [](char& ch)->bool { return !isspace(ch); }));

  str.erase(find_if(str.rbegin(), str.rend(),
    [](char& ch)->bool { return !isspace(ch); }).base(), str.end());

  return str;
}

string getStdoutFromCommand(string cmd)
{
  // setup
  string data;
  char buffer[2048];

  // do it
  FILE* stream = popen(cmd.c_str(), "r");
  if(stream) {
    while ( fgets(buffer, sizeof(buffer), stream) ) {
      data.append(buffer);
    }
    pclose(stream);
  }

  // done
  return trim(data);
}

bool printStdoutFromCommand(string cmd)  // returns true if failed
{
  bool ret = true;
  FILE* stream = popen(cmd.c_str(), "r");
  if (stream) {
    char buffer[2048];
    while ( fgets(buffer, sizeof(buffer), stream) ) {
      cout << buffer;
    }
    pclose(stream);
    ret = false;
  } else {
    cerr << "FAIL!\n";
  }
  return ret;
}

int main(int argc, char**argv)
{
  const char* cmd = "echo 1 && sleep 1 && echo 2 && sleep 1 && echo 3 && sleep 1 && echo done!";
  cout << "cmd=" << cmd << "\n" 
       << (printStdoutFromCommand(cmd)? "Failure": "Success.\n");

  cout << "Again with function that assembles all output into a space-trimmed string:\n" 
	  << getStdoutFromCommand(cmd) << "\n";

  return 0;
}  
