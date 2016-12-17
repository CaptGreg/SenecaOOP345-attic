#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char*argv[])
{
  if(argc != 4) {
    cerr << "Usage " << argv[0] << ": in out key\n";
    return 1;
  }
  string in  = argv[1];
  string out = argv[2];
  char   key = argv[3][0];

  fstream i(in, ios::in | ios::binary | ios::ate); // ios::ate open AT-THE_END
  if(i.is_open()) {
    streamsize size = i.tellp();
    cout << "file size = " << size << " bytes\n";
    char *b = new char[size];
    i.seekp(0);  // position the file at the start (rewind the file)
    i.read(b, size);
    cout << i.gcount() << " bytes read\n";
    i.close();

    for(streamsize i = 0; i < size; i++)
      b[i] ^= key;
 
    fstream o(out, ios::out | ios::trunc | ios::binary);
    if(o.is_open()) {
      o.write(b, size);
      if(not o.fail()) 
        cout << size << " bytes written\n";
      o.close();
    } else {
      cerr << "Cannot create file '" << out << "'\n";
    }
    delete [] b;
  } else {
    cerr << "Cannot open file '" << in << "'\n";
  }
}

