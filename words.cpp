// c++ example from python doc http://www.aleax.it/Python/accu03_Python_Intro_alex.pdf

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
int main()
{
  std::map<std::string, std::vector<int> > word_lineNumbers_table;
  std::string line;
  int n = 0;
  while(getline(std::cin, line)) {
    n++;
    std::istringstream sline(line);
    std::string word;
    while(sline >> word) {
      word_lineNumbers_table[word].push_back(n);
    }
  }
  
#if 1
  std::cout << "\n\niterators\n";
  for(
		std::map<std::string, std::vector<int> >::iterator        // C++98
		// auto                                                   // C++11
		iterWord = word_lineNumbers_table.begin(); iterWord != word_lineNumbers_table.end(); ++iterWord) {
			std::cout << iterWord->first << " -->";
			for(
				std::vector<int>::iterator                            // C++98
				// auto                                               // C++11
				iterLineNumber = iterWord->second.begin() ; iterLineNumber != iterWord->second.end(); ++iterLineNumber) {
					std::cout << ' ' << *iterLineNumber;
			}
			std::cout << "\n";
		}
#endif

#if 0
  std::cout << "\n\nrange-based for\n";
  for(auto word : word_lineNumbers_table) {
    std::cout << word.first << " -->";
    for(auto lineNumber : word.second) {
      std::cout << ' ' << lineNumber;
    }
    std::cout << "\n";
  }
#endif
  return 0;
}
