// Do c++ and c-code do the same thing with 'i = i++'?
// Hope they behave identically, but let's see.

#include <stdio.h>

int main(int argc, char ** argv)
{
   #define SIZE 10
   double trouble[SIZE] = { 0,   1,   2,   3,   4,   5,   6,   7,   8,   9   };
   char   broiled[SIZE] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'i', 'j', 'k' };
   int k;

   int i = 2; printf("i = %d\n", i );

   i = --i; // everyone agrees with this one!
   printf("After 'i = --i;', i = %d\n", i);

   i = 2; printf("i = %d\n", i);
   i = i++; // this generates some discussion and is a classic JAVA final exam question.
   printf("After 'i = i++;', i = %d\n", i);

   printf("NOTE: 'i++ = i++;'  does not compile - error: expression is not assignable\n");
   // i++ = i++;  //  error: expression is not assignable
   // printf(" << "After 'i++ = i++;', i = " << i << "\n";
  
   printf("same error with 'y++ = i++;'\n");
   // int y;
   // y++ = i++; // error: expression is not assignable

   printf("\nWhat happens with array indicies?\n");
   printf("double trouble:"); for(k=0; k < SIZE; k++) printf("%.0f", trouble[k]); printf("\n");

   i = 2; printf("i = %d\n", i);

   trouble[i] = trouble[i++];
   printf("After 'trouble[i] = trouble[i++];', i = %d\n",  i);
   printf("double trouble:"); for(k =0; k < SIZE; k++) printf("%.0f", trouble[k]); printf("\n");
   printf("It assigned trouble[3]=trouble[2] and i=3\n");

   printf("\nreset trouble.\n");
   for(k = 0; k < SIZE; k++) trouble[k] = k;
   printf("double trouble:"); for(k =0; k < SIZE; k++) printf("%.0f", trouble[k]); printf("\n");

   i = 2; printf("i = %d\n", i);
   trouble[i++] = trouble[i++];
   printf("After 'trouble[i++] = trouble[i++];', i = %d\n", i);
   printf("double trouble:"); for(k =0; k < SIZE; k++) printf("%.0f", trouble[k]); printf("\n");
   printf("It assigned trouble[3]=trouble[2] and i=4\n");

   printf("\nDoes a char array make any difference?\n");
   printf("char broiled:"); for(k =0; k < SIZE; k++) printf("%c", broiled[k]); printf("\n");


   i = 2; printf("i = %d\n", i);

   broiled[i] = broiled[i++];
   printf("After 'broiled[i] = broiled[i++];', i = %d\n", i);
   printf("char broiled:"); for(k =0; k < SIZE; k++) printf("%c", broiled[k]); printf("\n");

   broiled[i++] = broiled[i++];
   printf("After 'broiled[i++] = broiled[i++];', i = %d\n", i);
   printf("char broiled:"); for(k =0; k < SIZE; k++) printf("%c", broiled[k]); printf("\n");

   printf("\nSo there you go.\n");
   printf("'i=i++', 'array[i]=array[i++]', array[i++]=array[i++] behave differently\n");
   printf("  ('i++=i++' does not compile - error: expression is not assignable)\n");

   // i = 3++;  // will this compile?  *** NO! ***
   // i++ = 3;

   // this compiles in c++ but not in c.
   // i = 2; printf("i = 2;\n");
   // (++i)++;  printf("after (++i)++; i=%d\n", i);  // prints 4

   // i = 2; printf("i = 2;\n");
   // (i++)++;  printf(" << "after (i++)++; i=\n", i);
}
