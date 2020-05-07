// c++20

// https://en.cppreference.com/w/cpp/numeric/midpoint

#include <cstdint>
#include <limits>
#include <numeric>
#include <iostream>
 
int main()
{
    std::uint32_t a = std::numeric_limits<std::uint32_t>::max();
    std::uint32_t b = std::numeric_limits<std::uint32_t>::max() - 2;
 
    std::cout << "a: " << a << '\n'
              << "b: " << b << '\n'
              << "Incorrect (overflow and wrapping): " << (a + b) / 2 << '\n'
              << "Correct: " << ((a >> 1) + (b >> 1)) << " (right shift and add)\n" // GB
              << "Correct: " << std::midpoint(a, b) << " (std::midpoint)\n\n";
 
 
    auto on_pointers = [](int i, int j) {
        char const* text = "0123456789";
        char const* p = text + i;
        char const* q = text + j;
        std::cout << "std::midpoint('" << *p << "', '" << *q << "'): '"
                  << *std::midpoint(p, q) << "'\n";
    };
 
    on_pointers(2, 4);
    on_pointers(2, 5);
    on_pointers(5, 2);
    on_pointers(2, 6);
}
