#include <iostream>
#include <thread>


int main(int argc, char**argv)
{
  std::cout << std::thread::hardware_concurrency() << " hardware cores\n";
}
