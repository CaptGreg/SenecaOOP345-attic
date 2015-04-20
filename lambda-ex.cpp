#include <iostream>
#include <string>
using namespace std;

void f()
{
}

void g()
{
}

int main()
{
  auto q = []  () { cout << "q\n"; };
  auto r = [q] () { cout << "r\n"; q(); };
  auto s = [r] () { cout << "s\n"; r(); };
  auto t = [s] () { cout << "t\n"; s(); };

  auto letterMethod1 = [] (float grade) { // -> string {
    return
            grade >= 90? string("A+") :
            grade >= 80? string("A")  :
            grade >= 75? string("B+") :
            grade >= 70? string("B")  :
            grade >= 65? string("C+") :
            grade >= 60? string("C")  :
            grade >= 55? string("D+") :
            grade >= 50? string("D")  :
                         string("F");
  };

  auto letterMethod2 = [] (float grade) { // -> string {
    struct {
        float       lowerLimit;
        const char* letterGrade;
    } table[] = {
            {90, "A+"},
            {80, "A"},
            {75, "B+"},
            {70, "B"},
            {65, "C+"},
            {60, "C"},
            {55, "D+"},
            {50, "D"},
            {45, "F but you nearly made it"},
            {0 , "F - take the course again.  next time do some work!"}
     }, *pTable = table;

     if(grade < 0) grade = 0;

     while(true) {
	if(grade >= pTable->lowerLimit) 
           return string(pTable->letterGrade);
        else 
	   pTable++;
     }
  };

  t();

  float grade = 74.5;
  cout << "grade = " << grade << ", letter grade = " << letterMethod1(grade) << "\n";
  grade = 0;
  cout << "grade = " << grade << ", letter grade = " << letterMethod2(grade) << "\n";
}
