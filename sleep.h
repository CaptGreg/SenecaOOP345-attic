// replace system dependent sleep(nap) with c++11 OS independent chono second method
// #include <unistd.h>         // sleep
#include <thread>
#include <chrono>
void sleep(int nap) { std::this_thread::sleep_for(std::chrono::seconds(nap)); }
