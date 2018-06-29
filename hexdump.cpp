// How does getline handle files containing mixed text file line teminator?

// Apple text files use a single '0x0A' \r, DOS uses \r\n '0x0D0A' pair, and UNIX uses a single '0x0A' \n.

// 1. Write a file with mixed line separators.
// 2. Read back with getline.
// 3. Dump the file line by line.

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>  // isprint

// using namespace std;

void hexdump(std::ostream& os, const void* b, const size_t count)
{
    std::ios_base::fmtflags fmtfl =  os.flags(); // save <iomanip> state

    os << std::hex << std::setfill('0');
    for(size_t address = 0; count > address; address += 16 ) {
        const unsigned char* line = reinterpret_cast<const unsigned char*> ( b ) + address;

        size_t thisLineBytes = count - address;
        if(thisLineBytes > 16) thisLineBytes = 16;

        // Show the address
        os << std::setw(8) << address;

        // Show the hex codes
        for( size_t i = 0; i < 16; i++ ) {
            if( i % 8 == 0 ) os << ' ';
            if( i < thisLineBytes )
                os << ' ' << std::setw(2) << line[i];
            else 
                os << "   ";
        }

        // Show printable characters
        os << "  ";
        for( size_t i = 0; i < thisLineBytes; i++) {
            // if( i % 8 == 0 ) os << ' ';
            os << ( std::isprint(line[i]) ? line[i]: '.' ); 
        }

        os << "\n";
    }

    os.flags(fmtfl); // restore <iomanip> state
}
void hexdump(const void *b, const size_t count)
{
  hexdump(std::cout,  b, count);
}

void hexdumpString(std::ostream& os, const std::string& b)
{
    std::ios_base::fmtflags fmtfl =  os.flags(); // save <iomanip> state

    os << std::hex << std::setfill('0');
    for(size_t address = 0; b.size() > address; address += 16 ) {
        const unsigned char* line = reinterpret_cast<const unsigned char*> (b.c_str()) + address;

        size_t thisLineBytes = b.size() - address;
        if(thisLineBytes > 16) thisLineBytes = 16;

        // Show the address
        os << std::setw(8) << address;

        // Show the hex codes
        for( size_t i = 0; i < 16; i++ ) {
            if( i % 8 == 0 ) os << ' ';
            if( i < thisLineBytes )
                os << ' ' << std::setw(2) << line[i];
            else 
                os << "   ";
        }

        // Show printable characters
        os << "  ";
        for( size_t i = 0; i < thisLineBytes; i++) {
            // if( i % 8 == 0 ) os << ' ';
            os << ( std::isprint(line[i]) ? line[i]: '.' ); 
        }

        os << "\n";
    }

    os.flags(fmtfl); // restore <iomanip> state
}


#include <fstream>
#include <string>

using namespace std;

int main(int argc, char**argv)
{
  const string sTable[]={ 
    "[1] Unix LF '0x0a'\n",
    "[2] DOS CRLF '0x0d0a'\r\n",
    "[3] Apple CR '0x0d' sometimes messes up getline\r",
    "[4] another Unix LF '0x0a' line\n",
    "[5] and another Unix LF '0x0a' line\n",
    "[6] this line has no new-line"
  };

  string file = string(argv[0]) + string(".dat");
  fstream fs;

  // write file
  fs.open(file, ios::out);
  if(fs.is_open()) {
    for(auto& e : sTable)
      fs.write(e.c_str(), e.size());
    fs.close();
  } else {
    cerr << "cannot create file \"" << file << "\"\n";
  }

  // read and dump file
  fs.open(file, ios::in | ios::ate);
  if(fs.is_open()) {
    size_t size = fs.tellp();
      char* b = new char[size];
        fs.seekp(0);
        fs.read(b, size);
        hexdump(cout, b, size);
      delete b;
    fs.close();
  } else {
    cerr << "cannot open file \"" << file << "\"\n";
  }
  
  // dump memory data written to file
  string s; for(auto& e : sTable) s += e;
  hexdump(cout, s.c_str(), s.size()); cout << "\n\n";
  
  fs.open(file, ios::in);
  if(fs.is_open()) {
    string s;
    for(int line = 0; getline(fs, s); line++) {
      cout << "\n";
     
      cout << "+++++++++++++++++++ line " << line 
	   << " wrote: " << sTable[line].size() << " bytes "
	   // << "\n-->" <<sTable[line] << "<--"
	   << "\n";
      hexdump(cout, sTable[line].c_str(), sTable[line].size());
      cout << "\n";

      cout << "++++++ getline returned " << s.size() << " bytes"
	   // << "\n -->" << s << "<--\n\n";
	   << "\n";
      hexdump(cout, s.c_str(), s.size());
      cout << "\n";
    }
      fs.close();
  } else {
    cerr << "cannot open file \"" << file << "\"\n";
  }
}
