#include <iostream>

using namespace std;

int calculateIndex(char msg[], int value) {
   cout << msg << " : " << value << endl;
}

int main(int argc, char ** argv)
{
   int i = 2; cout << "i = " << i << "\n";

   cout << "\nWhat happens with array indicies?\n";
   double trouble[10] = { 0,1,2,3,4,5,6,7,8,9 };
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";

   i = 2; cout << "i = " << i << "\n";

   trouble[calculateIndex("\"i\" on the left side of \"=\"", i)] = trouble[calculateIndex("\"i++\" on the right side of \"=\"", i++)];
   cout << "After 'trouble[i] = trouble[i++];', i = " << i << "\n";
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";
   cout << "It assigned trouble[3]=trouble[2] and i=3\n";

   cout << "\nreset trouble.\n";
   for(int i = 0; i < 10; i++) trouble[i] = i;
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";

   i = 2; cout << "i = " << i << "\n";
   trouble[calculateIndex("\"i++\" on the left side of \"=\"", i++)] = trouble[calculateIndex("\"i++\" on the right side of \"=\"", i++)];
   cout << "After 'trouble[i++] = trouble[i++];', i = " << i << "\n";
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";
   cout << "It assigned trouble[3]=trouble[2] and i=4\n";
}
