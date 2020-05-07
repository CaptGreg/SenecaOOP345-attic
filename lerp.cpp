// c++20

// https://en.cppreference.com/w/cpp/numeric/lerp

#include <iostream>
#include <cmath>
 
int main()
{
    float a=10.0f, b=20.0f;
 
    std::cout << "a=" << a << '\n'
              << "b=" << b << '\n'
              << "mid point=" << std::lerp(a,b,0.5f) << '\n'
              << std::boolalpha << (a == std::lerp(a,b,0.0f)) << '\n';
}
