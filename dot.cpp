// graphviz resources
//   https://www.youtube.com/watch?v=JXfobuvyFOA
//   http://www.webgraphviz.com/
//   https://en.wikipedia.org/wiki/Graphviz
//   https://everipedia.org/wiki/Graphviz/
//   http://www.graphviz.com/

#include <fstream>   // fstream required for 'fileexists'
#include <iostream>
#include <string>
#include <vector>

// C++11 experimental/filesystem feature works with g++ version 5.4.1 with -std=c++11
#if (10000*__GNUC__ + 100*__GNUC_MINOR__ + __GNUC_PATCHLEVEL__) >= 50401
  #define CPP11
  #include <experimental/filesystem>  // C++11
  // This file requires compiler and library support for the ISO C++ 2011 standard. 
  // This support must be enabled with the -std=c++11 or -std=gnu++11 compiler options.
#endif

using namespace std;

int main(int argc, char*argv[])
{
  if(argc == 1) {
    cerr << "Usage: " << argv[0] << " graphviz-dot-file-list\n";
    return 1;
  }

  vector<string> dotLocations {
      "/usr/bin/dot"                                       // default UNIX
    , "/usr/local/bin/dot"                                 // sometimes a UNIX package is installed in /usr/local
    , "C:/\"Program Files (x86)\"/Graphviz2.38/bin/dot.exe" // a known DOS location for graphviz 2.38 (current package as of Nov 25, 2011)
    , R"foo(C:/"Program Files (x86)"/Graphviz2.38/bin/dot.exe)foo" // a known DOS location for graphviz 2.38 (current package as of Nov 25, 2011)
    , R"foo(C:\"Program Files (x86)"\Graphviz2.38\bin\dot.exe)foo" // a known DOS location for graphviz 2.38 (current package as of Nov 25, 2011)
#ifdef CPP11
    , string("C:")
        + std::experimental::filesystem::path::preferred_separator  // C++11
        + "Program Files (x86)"
        + std::experimental::filesystem::path::preferred_separator  // C++11
        + "Graphviz2.38"
        + std::experimental::filesystem::path::preferred_separator  // C++11
        + "bin"
        + std::experimental::filesystem::path::preferred_separator  // C++11
        + "dot.exe"
#endif
  };

  string dot;
  for(auto& e: dotLocations) {
    auto fileexist = [] (string file) {fstream f(file, ios::in); return f.is_open();};
    if( fileexist(e) ) {
      dot = move(e);
      break;
    }
  }

  if(dot.empty()){
    cerr << R"foo(
    ERROR: Graphviz program "dot" not found!  Package "graphviz" is not installed.

    HOW TO FIX:
      Graphviz reads simple ascii file defining a graph and generates an image (picture) of that graph.

      It has been around for a while.  It is a mature open source package originally written by AT&T in the late 1990's.

      Graph layout is a mathematically complex problem.  Graphviz provides an elegant solution.

      You have likely used graphviz without knowing it.

      If you are using a utility that produces a diagram, such as E-R, UML, crows-foot, call graphs, etc, 
      there is a good chance the utility uses graphviz to generate the picture of the graph.

      It is everywhere.

      See http://www.webgraphviz.com to run graphviz live in a browser using javascript.

      Documentation, samples, binary versions for common operating systems, 
      or the source code can be downloaded from "http://graphviz.org".
    )foo";
    return 2;
  }

  for(int arg = 1; arg < argc; arg++) {
    string cmd = dot;

    string file(argv[arg]);

    cmd += " -Tpng -O " + file; // -O flag - automatically generate the output file name with the proper suffix determined by -T image type flag 

    cout << "Running command -->" << cmd << "<--\n";
    int result = system(cmd.c_str());
    cout  << "It returned '" << result << (result?  "' (command failed)\n": "' (command passed)\n");
  }
  return 0;
}
