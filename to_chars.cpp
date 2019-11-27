// tested with
// clang++-8 -Wall -std=c++17  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  to_chars.cpp -o to_chars -lrt -pthread -ltbb -ltbb && ./to_chars
// g++-9.2   -Wall -std=c++17  -ggdb -fopenmp -fmax-errors=1 -Wfatal-errors  to_chars.cpp -o to_chars -lrt -pthread -ltbb -ltbb && ./to_chars


#include <array>
#include <cerrno>      // errno
#include <charconv>    // C++17 ‘to_chars’
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

int main()
{
  /*
  The new conversion routines are:

    * non-throwing
    * non-allocating
    * no locale support
    * memory safety
    * error reporting gives additional information about the conversion outcome
    * bound checked
    * explicit round-trip guarantees - you can use to_chars and from_chars to convert the number back and forth.
      It will give you the exact binary representations. 
      This is not guaranteed by other routines like printf/sscanf/itoa, etc.
  */

  {
    // https://www.bfilipek.com/2019/11/tochars.html
    std::string str { "xxxxxxxx" };
    const int value = 1986;
    std::to_chars(str.data(), str.data() + str.size(), value);
    std::cout << str << "\n";
    // str is "1986xxxx"
  }


  {
    // https://www.bfilipek.com/2018/12/fromchars.html
    const std::string str { "12345678901234" };
    // int value = 0;
    uint64_t value = 0;
    // std::cout << "from_chars=" << std::from_chars(str.data(),str.data() + str.size(), value) << "\n";
    std::from_chars(str.data(),str.data() + str.size(), value);
    std::cout << value << "\n";
    // error checking ommited...
  }

  {
    // https://en.cppreference.com/w/cpp/utility/to_chars
    std::array<char, 1> str;

    std::cout << "call 1\n";
    if(auto [p1, ec1] = std::to_chars(str.data(), str.data() + str.size(), 42);
      ec1 == std::errc())
    std::cout << std::string_view(str.data(), p1 - str.data());

    // how to pick up 'ec' error?
    // std::cout << std::errc() << "\n";

    std::cout << "call 2\n";
    auto [p2, ec2] = std::to_chars(str.data(), str.data() + str.size(), 42);
    if(p2 == str.data()) 
    {
      std::error_code ec2a (errno,std::generic_category());
      std::error_condition ok;
      if (ec2a != ok) std::cout << "Error: " << ec2a.message() << '\n';
    }

    std::cout << "call 3\n";
    // to_chars returns
      struct to_chars_result {
        char* ptr;
        std::errc ec; // a typedef
      };
    // let auto figure it out
    auto tcr = std::to_chars(str.data(), str.data() + str.size(), 42);
    if(tcr.ptr == str.data()) {
      // std::cout << ec << "\n";
      // std::cout << reinterpret_cast<int>(tcr.ec) << "\n"; // error: reinterpret_cast from 'std::errc' to 'int' is not allowed
      // tcr.ec = std::errc();
      // std::cout << tcr.ec << "\n";
      // std::cout << std::errc() << "\n";
    }
  }

  return EXIT_SUCCESS;
}
