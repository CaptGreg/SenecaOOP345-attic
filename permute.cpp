// g++ -std=c++11 -Ofast -Wall permute.cpp -o permute

#include <vector>
#include <string>
#include <iostream>
#include <chrono>     // platform independent C++11 std::chrono time functions
#include <algorithm>  // shuffle <-- include not required

// appears GNU C++11 knows about time_t, time, srand, and rand
// use platform independent C++11 std::chrono time functions
// #include <cstdlib>  // rand, srand
// #include <ctime>    // time_t, time

// use rand(3)
// int rand(void);
// void srand(unsigned int seed);

// alternatives to rand(3)
// random(3)
// long int random(void);
// void srandom(unsigned int seed);

using namespace std;

int main(int argc, char**argv)
{
  // read and store input in vector 'lines'.
  vector<string> lines;

  bool flagDOS = false;
  string s;
  while (getline(cin, s)) {  // read file
    auto pos =  s.find('\r'); 
    if(pos != string::npos) {
      s.erase(pos);
      flagDOS = true;
    }
    // lines.push_back(move(s));
    lines.emplace_back(s);
  }

  // write each line out in a random order

  // seed the random number generator with the current time
  // seconds since epoch, January 1, 1970 (midnight UTC/GMT), not counting leap seconds 
  // (in ISO 8601: 1970-01-01T00:00:00Z).
  // time_t t = time(0LL);    
  // srand((unsigned) t); // srandom( (unsigned) t);
  // initialize the random number generator with a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  srand(seed);
  // srandom( seed );

  const char *lineEnding = flagDOS? "\r\n": "\n";

#define METHOD1
// time cat *.cpp | ./permute | wc
//   52052  210732 1634684
// real    0m1.590s
// user    0m0.368s
// sys     0m0.080s


// #define METHOD2
// time cat *.cpp | permute | wc
//   52053  210729 1634692
// real    0m8.743s
// user    0m8.764s
// sys     0m0.012s


// #define METHOD3
//   52055  210735 1634728
// real    0m8.765s
// user    0m8.780s
// sys     0m0.016s



// METHOD 1 is faster than METHOD 2 or METHOD 3

#ifdef METHOD1
  for(size_t l = 0; l < lines.size(); l++)
    lines[l].swap(lines[rand()%lines.size()]);  // with luck, string swap is fast, maybe even a move operation

  for(size_t l = 0; l < lines.size(); l++)
    cout << lines[l] << lineEnding;
#endif

#ifdef METHOD2
  while(not lines.empty()) {
    int l = rand() % lines.size(); // or i = random() % lines.size();
    cout << lines[l] << lineEnding;
    lines.erase(lines.begin()+l);
  }
#endif

#ifdef METHOD3
  // http://www.cplusplus.com/reference/algorithm/shuffle/
  shuffle (lines.begin(), lines.end(), std::default_random_engine(seed));

  for(size_t l = 0; l < lines.size(); l++)
    cout << lines[l] << lineEnding;
#endif


}
