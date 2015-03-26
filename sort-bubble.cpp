#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

 // ascending order comparison
 bool ascending(double a, double b) {
     return a > b;
 }

 // descending order comparison
 bool descending(double a, double b) {
     return a < b;
 }

 // bubble sort
 void sortBubble(vector<double> a, int n, bool (*comp)(double, double)) { 
     for (int i = n - 1; i > 0; i--) {
         for (int j = 0; j < i; j++) {
             if (comp(a[j], a[j+1])) {
                 double temp = a[j];
                 a[j] = a[j+1];
                 a[j+1] = temp;
             }
         }
     }
 }

int main(int argc, char**argv)
{
  int NUM = 1000;
  if(argc > 1) NUM = atoi(argv[1]);

  vector<double> v(NUM);

  for(int i = 0; i < v.size(); i++)
    v[i] = 1 + random();

  sortBubble(v, NUM, ascending);
  sortBubble(v, NUM, descending);
}
