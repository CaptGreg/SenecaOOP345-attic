// http://en.cppreference.com/w/cpp/thread/sleep_for

#include <iostream>
#include <chrono>
#include <thread>
 
int main()
{
    std::cout << "Hello waiter" << "\n";
    std::chrono::milliseconds dura( 2000 );
    std::this_thread::sleep_for( dura );
    std::cout << "Waited 2000 ms\n";
}
