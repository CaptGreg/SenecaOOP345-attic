#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
   int i = 2; cout << "i = " << i << "\n";

   i = --i; // everyone agrees with this one!
   cout << "After 'i = --i;', i = " << i << "\n";

   i = 2; cout << "i = " << i << "\n";
   i = i++; // this generates some discussion and is a classic JAVA final exam question.
   cout << "After 'i = i++;', i = " << i << "\n";

   cout << "NOTE: 'i++ = i++;'  does not compile - error: expression is not assignable\n";
   // i++ = i++;  //  error: expression is not assignable
   // cout << "After 'i++ = i++;', i = " << i << "\n";
  
   cout << "same error with 'y++ = i++;'\n";
   // int y;
   // y++ = i++; // error: expression is not assignable

   cout << "\nWhat happens with array indicies?\n";
   double trouble[10] = { 0,1,2,3,4,5,6,7,8,9 };
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";

   i = 2; cout << "i = " << i << "\n";

   trouble[i] = trouble[i++];
   cout << "After 'trouble[i] = trouble[i++];', i = " << i << "\n";
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";
   cout << "It assigned trouble[3]=trouble[2] and i=3\n";

   cout << "\nreset trouble.\n";
   for(int i = 0; i < 10; i++) trouble[i] = i;
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";

   i = 2; cout << "i = " << i << "\n";
   trouble[i++] = trouble[i++];
   cout << "After 'trouble[i++] = trouble[i++];', i = " << i << "\n";
   cout << "double trouble:"; for(auto e : trouble) cout << e; cout << "\n";
   cout << "It assigned trouble[3]=trouble[2] and i=4\n";

   cout << "\nDoes a char array make any difference?\n";
   char broiled[10] = { 'a','b','c','d','e','f','g','i','j','k' };
   cout << "char broiled:"; for(auto e : broiled) cout << e; cout << "\n";

   i = 2; cout << "i = " << i << "\n";

   broiled[i] = broiled[i++];
   cout << "After 'broiled[i] = broiled[i++];', i = " << i << "\n";
   cout << "char broiled:"; for(auto e : broiled) cout << e; cout << "\n";

   broiled[i++] = broiled[i++];
   cout << "After 'broiled[i++] = broiled[i++];', i = " << i << "\n";
   cout << "char broiled:"; for(auto e : broiled) cout << e; cout << "\n";

   cout << "\nSo there you go.\n";
   cout << "'i=i++', 'array[i]=array[i++]', array[i++]=array[i++] behave differently\n";
   cout << "  ('i++=i++' does not compile - error: expression is not assignable)\n";

   // i = 3++;  // will this compile?  *** NO! ***
   // i++ = 3;
   i = 2; cout << "i = 2;\n";
   (++i)++;  cout << "after (++i)++; i=" << i << "\n";  // prints 4

   // i = 2; cout << "i = 2;\n";
   // (i++)++;  cout << "after (i++)++; i=" << i << "\n";
}
