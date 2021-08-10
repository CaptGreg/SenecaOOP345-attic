// https://www.modernescpp.com/index.php/multithreading-in-c-17-and-c-20

// g++-10 -fcoroutines -std=c++2a synchronize.cpp -o synchronize -ltbb -pthread
// g++-10           synchronize.cpp:31:3: error: ‘synchronized’ was not declared in this scope
// g++-10              31 |   synchronized{


/*
 * With transaction memory, the well-established idea of transactions will be applied in software.
 * Transactional memory
 * 
 * Transactional memory idea is based on transactions from the database theory. 
 * A transaction is an action which provides the properties Atomicity, Consistency, Isolation and Durability (ACID). 
 * Except for durability, all properties will hold for transactional memory in C++. 
 * C++ will have transactional memory in two flavours. One is called synchronised blocks and the other atomic blocks. 
 * Both have in common that they will be executed in total order and behave as they were protected by a global lock. 
 * In contrary to synchronised blocks, atomic blocks can not execute transaction-unsafe code.
 * 
 * Therefore, you can invoke std::cout in a synchronised block but not in an atomic block.
 * 
 * The synchronized keyword guarantees that the execution of the synchronised block will not overlap. 
 * That means in particular that there is a single, total order between all synchronised blocks. 
 * To say it the other way around. 
 * The end of each synchronised block synchronizes-with the start of the next synchronised block.
*/


#include <iostream>
#include <thread>
#include <vector>

int func() { 
  static int i = 0; 
  synchronized{ 
    std::cout << "Not interleaved \n"; 
    ++i; 
    return i;  
  } 
}
 
int main(){
  std::vector<std::thread> v(10); 
  for(auto& t: v) 
    t = std::thread([]{ for(int n = 0; n < 10; ++n) func(); });
}
