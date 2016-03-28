// g++ -std=c++11 -Ofast -Wall permute.cpp -o permute

#include <vector>
#include <string>
#include <iostream>
// appears GNU C++11 knows about time_t, time, srand, and rand
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
  time_t t = time(0LL);    
  srand((unsigned) t); // srandom( (unsigned) t);

  const char *lineEnding = flagDOS? "\r\n": "\n";

#define METHOD1
// time cat *.cpp | ./permute | wc
//   45317  183617 1427830
// real	0m0.104s
// user	0m0.120s
// sys	0m0.004s

// #define METHOD2
// time cat *.cpp | permute | wc
//   45317  183617 1427830
// real	0m3.494s
// user	0m3.580s
// sys	0m0.020s


// METHOD 1 is about 35 x faster than METHOD 2.

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
}
