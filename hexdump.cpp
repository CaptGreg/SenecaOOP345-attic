#include <cctype>  // isprint
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>  // unique_ptr

// using namespace std;

void hexdump(std::ostream& os, const void* b, const size_t count)
{
    for(size_t address = 0; count > address; address += 16 ) {
        const unsigned char* line = reinterpret_cast<const unsigned char*> ( b ) + address;

        size_t thisLineBytes = count - address;
        if(thisLineBytes > 16) thisLineBytes = 16;

        // Show the address
        os << std::setfill('0') << std::setw(8) << address;

        // Show the hex codes
        for( size_t i = 0; i < 16; i++ ) {
            if( i % 8 == 0 ) os << ' ';
            if( i < thisLineBytes ) // 'in the data', so print
                os << ' ' << std::hex << ((line[i]>>4)&0xf) << std::hex << (line[i]&0xf) ;
            else  // pad
                os << "   ";
        }

        // Show printable characters
        os << "  ";
        for( size_t i = 0; i < thisLineBytes; i++) {
            // if( i % 8 == 0 ) os << ' ';
            os << (char) ( std::isprint(line[i]) ? line[i]: '.' ); 
        }

        os << "\n";
    }
}

int main(int argc, char**argv)
{
  if(argc == 1) {
    std::cerr << "Usage: " << argv[0] << " file-to-dump [file-to dump ...]\n";
    return EXIT_FAILURE;
  }

  for(int arg = 1; arg < argc; arg++) {
    char* file = argv[arg];

    // open file 'at the end'
    std::fstream in(file, std::ios::in | std::ios::binary | std::ios::ate);
    if(in.is_open()) {
      // how large is file?
      const size_t size = in.tellp();

      // allocate buffer
      std::unique_ptr<char> b ( new char[size] ); // unique_ptr destructor deletes 'b' when 'b' goes out of scope

      // read file
      in.seekp(0, std::ios_base::beg);
      in.read(b.get(), size);

      // all done with file, close it
      in.close();

      // dump file
      hexdump(std::cout, b.get(), size);
    } else {
      std::cerr << "cannot open file \"" << file << "\"\n";
    }
  }
  return EXIT_SUCCESS;
}
