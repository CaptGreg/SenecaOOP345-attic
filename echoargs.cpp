#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
   cout << "number of args = " << argc << "\n";

   for(int arg = 0; arg < argc; arg++) {
       cout << "argv[" << arg << "] -->" << argv[arg]  << "<--\n";
   }

   cout << "Interpreting args as numbers, we have:\n";
   for(int arg = 0; arg < argc; arg++) {
       cout << "argv[" << arg << "] -->" << argv[arg] << "<--"  
            << ", as integer " << atoi(argv[arg]) 
            << ", as double "  << atof(argv[arg]) 
            << "\n";
   }
}
