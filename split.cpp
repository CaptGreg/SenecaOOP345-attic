#include <algorithm>
#include <iostream>
#include <numeric>
#include <string_view>
#include <string>
#include <vector>

#include <fstream>
#include <chrono>
#include <string>

class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
  void Start() { start = std::chrono::high_resolution_clock::now(); }
  void Stop()  { stop  = std::chrono::high_resolution_clock::now(); }
  uint64_t millisecs()
  {
   typedef std::chrono::duration<int,std::milli> millisecs_t ;
    millisecs_t duration_get( std::chrono::duration_cast<millisecs_t>(stop-start) ) ;
    return duration_get.count();
  }
  uint64_t microsecs()
  {
   typedef std::chrono::duration<int,std::micro> microsecs_t ;
    microsecs_t duration_get( std::chrono::duration_cast<microsecs_t>(stop-start) ) ;
    return duration_get.count();
  }
  uint64_t nanosecs()
  {
   typedef std::chrono::duration<int,std::nano> nanosecs_t ;
    nanosecs_t duration_get( std::chrono::duration_cast<nanosecs_t>(stop-start) ) ;
    return duration_get.count();
  }
};

// hacked from slide 11 of the slides from https://www.bfilipek.com/2018/10/strings17talk.html#the-slides
template<typename T>
std::vector<T> split(const T s, const T d)
{
  std::vector<T> o;

  auto first = s.cbegin();

  while(first != s.cend()) {
    const auto second = std::find_first_of(first, std::cend(s), std::cbegin(d), std::cend(d));
    // NOTE: d is a delimiting string, not a list of delimiter characters
    // the entire string must be matched

    if(first != second)
      o.emplace_back(s.substr(std::distance(s.cbegin(), first), std::distance(s.cbegin(), second)));

    if(second == s.cend())
      break;

    first = std::next(second);
  }

  return o;
}

int main(int argc, char* argv[])
{
  Timer t;
  std::string sparty ="Now is the time for all good men to come to the aid of the party";
  t.Start();
  std::vector<std::string> stringWords = split(sparty, std::string(" "));
  t.Stop();
  auto tString = t.nanosecs();
  std::cout << "split string word count = " << stringWords.size()  << ", " << tString << " ns.\n";

  std::string_view svparty = sparty;
  t.Start();
  std::vector<std::string_view> string_viewWords = split(svparty, std::string_view(" "));
  t.Stop();
  auto tString_view = t.nanosecs();
  std::cout << "split string_view word count = " << string_viewWords.size()  << ", " << tString_view << " ns.\n";

  std::cout << "split string is about " << tString / tString_view << " x slower than split string_view\n";

  auto splitlambda = [] ( auto s , char delimiter_char = ',' )
  {
    return accumulate(s.begin(), s.end(),  std::vector<decltype(s)>(1),
      [=](auto acc, char c) {
          // if (c == delimiter_char){  acc.push_back(string());   }
          if (c == delimiter_char){  acc.push_back(decltype(s)());   }
          else                    {  acc.back( ) += c;          } // String_view is read-only. (cannot append)
          return acc;
      } );
  };
  t.Start();
  std::vector<std::string> stringLambaWords = splitlambda(sparty, ' ');
  t.Stop();
  auto tAccumulate = t.nanosecs();
  std::cout << "accumulate-string word count = " << stringLambaWords.size()  << ", " << tAccumulate << " ns.\n";
  std::cout << "accumulate-string is about " << tAccumulate / tString_view << " x slower than split string_view.\n";

  // t.Start();
  // std::vector<std::string_view> string_viewLambaWords = splitlambda(svparty, ' ');
  // t.Stop();
  // std::cout << "string_viewLambaWords word count = " << string_viewLambaWords.size()  << ", " << t.nanosecs() << " ns.\n";
}

/*
  g++ -std=c++17  split.cpp -o split && ./split
  split string word count = 16, 7795 ns.
  split string_view word count = 16, 2495 ns.
  split string is about 3 x slower than split string_view
  accumulate-string word count = 16, 41957 ns.
  accumulate-string is about 16 x slower than split string_view.

  g++ -std=c++17 -Ofast split.cpp -o split && ./split
  split string word count = 16, 8136 ns.
  split string_view word count = 16, 2014 ns.
  split string is about 4 x slower than split string_view
  accumulate-string word count = 16, 33704 ns.
  accumulate-string is about 16 x slower than split string_view.
*/
