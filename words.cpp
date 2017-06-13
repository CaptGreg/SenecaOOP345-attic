// c++ example from python doc http://www.aleax.it/Python/accu03_Python_Intro_alex.pdf

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
int main()
{
  std::map<std::string, std::vector<int> > idx;
  std::string line;
  int n = 0;
  while(getline(std::cin, line)) {
    std::istringstream sline(line);
    std::string word;
    while(sline >> word) {
      idx[word].push_back(n);
    }
    n += 1;
  }
  
  for(auto i = idx.begin(); i != idx.end(); ++i)  // C++11
  // for(std::map<std::string, std::vector<int> >::iterator i = idx.begin(); i != idx.end(); ++i) // C++98
  {
    std::cout << i->first << ": ";
    for(auto j = i->second.begin() ; j != i->second.end(); ++j)  // C++11
    // for(std::vector<int>::iterator j = i->second.begin() ; j != i->second.end(); ++j) // C++98
    {
      std::cout << ' ' << *j;
    }
    std::cout << "\n";
  }
  return 0;
}
