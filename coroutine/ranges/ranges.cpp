// https://www.incredibuild.com/ja/blog/what-you-need-to-do-to-move-on-to-c-20-the-complete-list
// g++-10 -std=c++2a ranges.cpp -o ranges && ./ranges

/*
Ranges

Ranges make a huge difference in how we work with collections of data since they give users the power to lazily filter and transform data through a pipeline. 
Ranges also make iterator pairs largely unnecessary, reducing the need to write error-prone code. 
The familiar “|” operator is used to pass data from one part of the pipeline to the next, making it easy to compose different pipelines from a common set of primitive functions.

The following is an example of using ranges to lazily evaluate a data pipeline over a collection in g++ 10:
*/

#include <iostream>
#include <ranges>
#include <vector>

void main1() 
{
  using std::views::filter,
        std::views::transform,
        std::views::reverse;

  // Some data for us to work on
  std::vector<int> numbers = { 6, 5, 4, 3, 2, 1 };

  // Lambda function that will provide filtering
  auto is_even = [](int n) { return n % 2 == 0; };

  // Process our dataset
  auto results = numbers 
               | filter(is_even)
               | transform([](int n){ return n++; })
               | reverse;

  // Use lazy evaluation to print out the results
  for (auto v: results) {
    std::cout << v << " ";   // Output: 2 4 6
  }
  std::cout << "\n";
}


// https://en.cppreference.com/w/cpp/ranges/transform_view
#include <algorithm>
#include <array>
#include <cstdio>
#include <ranges>
#include <string>
 
class Transcoder {
    static constexpr std::array r{
        0x02,-0x02,-0x42, 0x05, 0x04, 0x05, 0x04,-0x02,
        0x00,-0x0a, 0x06,-0x04, 0x00, 0x0c, 0x03,-0x06,
    };
    decltype(r.size()) p{}, q{};
public:
    char operator() (int x) {
        ! (p|q)
        ? (99 == x ? x += r[p++]  : x -= r[q++])
        : (0 < p && p < r.size()) ? x += r[p++]
        : (0 < q && q < r.size()) ? x -= r[q++]
        : (0)
        ;
        return x;
    }
};
 
void main2()
{
    auto show = [](const char x) { std::putchar(x); };
 
    std::string in{ "cppreference.com\n" };
    std::ranges::for_each(in, show);
 
    std::string out;
    std::ranges::copy(
        std::ranges::views::transform(in, Transcoder{}),
        std::back_inserter(out));
    std::ranges::for_each(out, show);
 
    auto view = std::ranges::transform_view{ out, Transcoder{} };
    std::ranges::for_each(view, show);
    std::ranges::for_each(view, show);
}

// https://en.cppreference.com/w/cpp/ranges/split_view/deduction_guides
#include <ranges>
#include <string_view>
#include <type_traits>
using std::operator""sv;
 
void main3() 
{
    std::ranges::split_view w1{"a::b::c"sv, "::"sv};
    // type of w1 is split_view<string_view, string_view>:
    static_assert(std::is_same_v<
        decltype(w1),
        std::ranges::split_view<
            std::basic_string_view<char, std::char_traits<char>>,
            std::basic_string_view<char, std::char_traits<char>>>>);
 
    std::ranges::split_view w2{"x,y,z"sv, ','};
    // type of w2 is split_view<string_view, single_view<char>>:
    static_assert(std::is_same_v<
        decltype(w2),
        std::ranges::split_view<
            std::basic_string_view<char, std::char_traits<char>>,
            std::ranges::single_view<char>>>);
}

// https://en.cppreference.com/w/cpp/ranges/filter_view
#include <iostream>
#include <ranges>
#include <vector>
 
void main4 () 
{
    std::vector<int> ints{0, 1, 2, 3, 4, 5};
    auto even = [](int i) { return 0 == i % 2; };
    auto square = [](int i) { return i * i; };
 
    for (int i : ints | std::views::filter(even) | std::views::transform(square)) {
        std::cout << i << ' ';
    }
    std::cout << "\n";
}

// https://en.cppreference.com/w/cpp/ranges/take_while_view
#include <ranges>
#include <iostream>
 
void main5()
{
    for (int year : std::views::iota(2017)
                  | std::views::take_while([](int y) { return y <= 2020; })) {
        std::cout << year << ' ';
    }
    std::cout << '\n';
 
    const char idea[] {"Today is yesterday's tomorrow!.."};
    for (char x : std::ranges::take_while_view(idea, [](char c) { return c != '.'; })) {
        std::cout << x;
    }
    std::cout << '\n';
}

// no www.cplusplus.com ranges hits

int main()
{
  std::cout << "++++++++++++++++++++++++++++++ main1\n";
  main1();
  std::cout << "++++++++++++++++++++++++++++++ main2\n";
  main2();
  std::cout << "++++++++++++++++++++++++++++++ main3\n";
  main3();
  std::cout << "++++++++++++++++++++++++++++++ main4\n";
  main4();
  std::cout << "++++++++++++++++++++++++++++++ main5\n";
  main5();
}
