#include <iostream>
#include <thread>
using namespace std;

int main()
{
 cout << std::thread::hardware_concurrency() << "\n";
 return 0;
}

