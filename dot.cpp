#include <iostream>
#include <string>
using namespace std;

int main(int argc, char*argv[])
{
  for(int arg = 1; arg < argc; arg++) {
    string cmd;
#ifdef __unix
    cmd = "dot";
#else
    cmd = "C:/\"Program Files (x86)\"/Graphviz2.38/bin/dot.exe";
#endif
    string file(argv[arg]);

    cmd += " -Tpng " + file + " > " + file + ".png";
    cout << cmd << "\n";
    cout << "cmd returned " << system(cmd.c_str()) << "\n";
  }
}