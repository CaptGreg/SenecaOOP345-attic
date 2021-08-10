// https://godbolt.org/z/s6a9do

#include <iostream>
// #include <https://raw.githubusercontent.com/atulmehra/moderncpp-code-examples/master/cpp20_code_examples/coroutines/include/templatized_generator.h>
#include "templatized_generator.h"

Couroutine_Example_Generator<int> f() { for (int i = 0; i < 10; ++i) co_yield i; }

int main ()
{
    for (auto i = f(); i.move_next(); )
    {
        std::cout << i.current_value() << ' '; // 0 1 2 3 4 5 6 7 8 9
    }

    return 0;
}
