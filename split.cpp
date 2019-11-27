#include <algorithm>    // find_first_of
#include <chrono>
#include <iostream>
#include <numeric>      // accumulate
#include <string_view>  // older compilers need this
#include <string>
#include <vector>


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
std::vector<T> splitBF(const T s, const T d)
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

template<typename T>
std::vector<T> splitBM(const T s, const T d) // https://en.cppreference.com/w/cpp/utility/functional/boyer_moore_searcher
{
  std::vector<T> o;
  auto first = s.cbegin();
  while(first != s.cend()) {
    // const auto second = std::find_first_of(first, std::cend(s), std::cbegin(d), std::cend(d));
    const auto second = std::search(first, std::cend(s), std::boyer_moore_searcher(std::cbegin(d), std::cend(d)));
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
  std::vector<std::string> stringWords = splitBF(sparty, std::string(" "));
  t.Stop();
  auto tStringBF = t.nanosecs();
  std::cout << "splitBF string word count = " << stringWords.size()  << ", " << tStringBF << " ns.\n";

  t.Start();
  std::vector<std::string> stringWordsBM = splitBM(sparty, std::string(" "));
  t.Stop();
  auto tStringBM = t.nanosecs();
  std::cout << "splitBM string word count = " << stringWordsBM.size()  << ", " << tStringBM << " ns.\n";


  // Greg's shortest effort
  // functional programming style: no side effects.
  auto splitGB = [] ( const std::string& s , const char delimiter = ',' ) { // -> std::vector<std::string> { // g++-9.2 no longer chokes without -> return specification
    std::string token;
    std::vector<std::string> v;
	
    for(auto c:s)
      if(c != delimiter) token += c; 
      else v.emplace_back( move(token) );
    v.emplace_back( move(token) );
    return v;
  };
  t.Start();
  std::vector<std::string> stringWordsGB = splitGB(sparty, ' ');
  t.Stop();
  auto tStringGB = t.nanosecs();
  std::cout << "splitGB string word count = " << stringWordsGB.size()  << ", " << tStringGB << " ns.\n";


  //////////////////////////////////
  std::string_view svparty = sparty;
  //////////////////////////////////

  auto splitGBsv = [] ( const std::string_view& s , const char delimiter = ',' ) { // -> std::vector<std::string> { // g++-9.2 no longer chokes without -> return specification
    std::string token;
    std::vector<std::string_view> v;
	
    for(auto c:s)
      if(c != delimiter) token += c; 
      else v.emplace_back( move(token) );
    v.emplace_back( move(token) );
    return v;
  };
  t.Start();
  std::vector<std::string_view> stringWordsGBsv = splitGBsv(svparty, ' ');
  t.Stop();
  auto tStringGBsv = t.nanosecs();
  std::cout << "splitGBsv string_view word count = " << stringWordsGBsv.size()  << ", " << tStringGBsv << " ns.\n";


  t.Start();
  std::vector<std::string_view> string_viewWords = splitBF(svparty, std::string_view(" "));
  t.Stop();
  auto tString_view = t.nanosecs();
  std::cout << "splitBF string_view word count = " << string_viewWords.size()  << ", " << tString_view << " ns.\n";

  auto splitlambda = [] ( auto s , char delimiter_char = ',' )
  {
    return accumulate(s.begin(), s.end(),  std::vector<decltype(s)>(1),
      [=](auto acc, char c) {
          if (c == delimiter_char){  acc.emplace_back(decltype(s)());   }
          else                    {  acc.back( ) += c;          } // String_view is read-only. (cannot append)
          return acc;
      } );
  };
  t.Start();
  std::vector<std::string> stringLambaWords = splitlambda(sparty, ' ');
  t.Stop();
  auto tsplitlambda = t.nanosecs();
  std::cout << "accumulate-splitlambda string word count = " << stringLambaWords.size()  << ", " << tsplitlambda << " ns.\n";

  auto splitlambdaCount = [] ( auto s , char delimiter_char = ',' )
  {
    uint32_t start = 0;
    uint32_t count = 0;
    return accumulate(s.cbegin(), s.cend(),  std::vector<decltype(s)>(1),
      [&](auto acc, char c) {
          if (c == delimiter_char){  acc.emplace_back(s.substr(start,count)); start += count+1; count=0; }
          else                    {  count++;  }
          return acc;
      } );
  };
  t.Start();
  std::vector<std::string> stringLambaWords2 = splitlambdaCount(sparty, ' ');
  t.Stop();
  auto tsplitlambdaCount = t.nanosecs();
  std::cout << "accumulate-splitlambdaCount string word count = " << stringLambaWords2.size()  << ", " << tsplitlambdaCount << " ns.\n";


  // t.Start();
  // std::vector<std::string_view> string_viewLambaWords = splitlambda(svparty, ' ');
  // t.Stop();
  // std::cout << "string_viewLambaWords word count = " << string_viewLambaWords.size()  << ", " << t.nanosecs() << " ns.\n";
}

/*
  g++-9.2 -Wall -std=c++17 -Ofast split.cpp -o split && ./split
  split string word count = 16, 5551 ns.
  split string GB word count = 16, 912 ns.
  split string_view word count = 16, 762 ns.
  accumulate-string word count = 16, 17203 ns.
  accumulate-string2 word count = 16, 11702 ns.
*/
