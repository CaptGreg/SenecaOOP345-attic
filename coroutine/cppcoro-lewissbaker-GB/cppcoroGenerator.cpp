// https://www.modernescpp.com/index.php/c-20-coroutines-with-cppcoro

// cppcoroGenerator.cpp

#include <iostream>
#include <cppcoro/generator.hpp>

cppcoro::generator<char> hello() {
    co_yield 'h';                  
    co_yield 'e';                   
    co_yield 'l';                   
    co_yield 'l';                   
    co_yield 'o';                   
}

cppcoro::generator<const long long> fibonacci() {
    long long a = 0;
    long long b = 1;
    while (true) {
        co_yield b;                 // (2)
        auto tmp = a;
        a = b;
        b += tmp;
    }
}

int main() {

    std::cout << std::endl;
    
    for (auto c: hello()) std::cout << c; 
    
    std::cout << "\n\n";
    
    for (auto i: fibonacci()) {  // (1)
        if (i > 1'000'000) break;
        std::cout << i << " ";
    }
    
    std::cout << "\n\n";
    
}
