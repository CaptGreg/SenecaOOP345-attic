// https://www.modernescpp.com/index.php/coroutines

// greedyGenerator.cpp, a pure C++11 program (auto, range-based for), no c++20 coroutines

// g++-10 -fcoroutines -std=c++2a greedyGenerator.cpp -o greedyGenerator -ltbb -pthread && ./greedyGenerator 
// --- compiles

#include <iostream>
#include <vector>

std::vector<int> getNumbers(int begin, int end, int inc= 1)
{
  std::vector<int> numbers;
  for (int i= begin; i < end; i += inc){
    numbers.push_back(i);
  }
  return numbers;
}

int main(){
  std::cout << "\n";
  auto numbers= getNumbers(-10, 11);
  for (auto n: numbers) 
    std::cout << n << " ";
  std::cout << "\n\n";
  for (auto n: getNumbers(0,101,5)) 
    std::cout << n << " ";
  std::cout << "\n\n";
}
