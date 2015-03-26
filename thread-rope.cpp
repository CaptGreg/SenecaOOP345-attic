#include <iostream>
#include <vector>
#include <thread>
#include <sstream>

// A rope is made up of many threads.  Each thread is a strand of the rope.

class Rope 
{
  int value;
public:
  Rope() : value(0) { std::cout << "Rope() ctor\n"; }
 ~Rope()            { std::cout << "~Rope() dtor: value=" << value << '\n'; }
  void Set(int v)   { value=v; }

  void Print() 
    { std::stringstream ss; ss << "Print " << value << "\n"; std::cout << ss.str(); }
  void Compute() 
    { std::stringstream ss; ss << "Compute " << value << "\n"; std::cout << ss.str(); }
  void ComputeArg(int arg) 
    { std::stringstream ss; ss << "ComputeArg " << arg << "\n"; std::cout << ss.str(); }
};

int main()
{
   const int                NUM_THREADS = 3;
   Rope                     strands[NUM_THREADS];
   std::vector<std::thread> threadVector;
   std::thread              threadTable[NUM_THREADS];

   int                      arg;
   int                      index;

   for(auto& r  : strands) r.Print();
   arg   = 1000;
   for(auto& r  : strands) r.Set(arg++);
   for(auto& r  : strands) r.Print();

   std::cout << "\nmethod 1 - std::bind class member function\n";
   for(auto& r  : strands) threadVector.push_back(std::thread(std::bind(&Rope::Compute, &r)));  // class member function
   for(auto& th : threadVector)     th.join();
   threadVector.clear();

   std::cout << "\nmethod 2 - lambda\n";
   for(auto& r  : strands) threadVector.push_back(std::thread( [&]() { r.Compute(); } ));  // lambda
   for(auto& th : threadVector)     th.join();
   threadVector.clear();

   std::cout << "\nmethod 3 - std::bind class member function with arg\n";
   arg   = 100;
   for(auto& r  : strands) threadVector.push_back(std::thread(std::bind(&Rope::ComputeArg, &r, arg++)));  // class member function
   for(auto& th : threadVector)     th.join();
   threadVector.clear();

   std::cout << "\nmethod 4 -lambda - call class member function with arg\n";
   arg   = 200;
   for(auto& r  : strands) threadVector.push_back(std::thread([&]() { r.ComputeArg(arg++); }));  // lambda
   for(auto& th : threadVector)     th.join();
   threadVector.clear();

   std::cout << "\nmethod 5 - threadTable without vector\n";

   std::cout << "create Compute threads\n";
   index = 0;
   for(auto& th : threadTable) th = std::thread( std::bind(&Rope::Compute, &strands[index++] ) );
   for(auto& th : threadTable) th.join();

   std::cout << "create ComputeArg threads\n";
   index = 0;
   arg   = 300;
   for(auto& th : threadTable) th = std::thread( std::bind(&Rope::ComputeArg, &strands[index++], arg++ ) );
   for(auto& th : threadTable) th.join();

   std::cout << "\nmain exiting\n";
}
