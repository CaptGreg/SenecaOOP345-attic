#include <string>
#include <iostream>
using namespace std;
#include "stack-array.h"

int main(int argc, char **argv)
{
  try {
    cout << "\n";
    StackArray<int> saI(5);
    for(int i = 0 ; i < 5; i++)
      saI.push(10*i+1);
    for(int i = 0 ; i < 5; i++)
      cout << saI.pop() << "\n";
  } catch(const char* err) {
    cout << "caught a char* err=" << err << "\n";
  }

  try {
    cout << "\n";
    StackArray<double> saD(5);
    for(int i = 0 ; i < 5; i++)
      saD.push(100.*i+1.1);
    for(int i = 0 ; i < 5; i++)
      cout << saD.pop() << "\n";
  } catch(const char* err) {
    cout << "caught a char* err=" << err << "\n";
  }

  try {
    cout << "\n";
    StackArray<double> saD(5);
    for(int i = 0 ; i < 15; i++)
      saD.push(100.*i+1.1);
    for(int i = 0 ; i < 5; i++)
      cout << saD.pop() << "\n";
  } catch(const char* err) {
    cout << "caught a char* err=" << err << "\n";
  }

  try {
    cout << "\n";
    StackArray<string> saS(5);
    saS.push( string("one") );
    saS.push( string("two") );
    saS.push( string("three") );
    saS.push( string("four") );
    saS.push( string("five") );

    size_t num = saS.size();
    for(size_t i = 1 ; i <= num + 1; i++) {
      if(saS.size() == 0) cout << "popping an empty stack empty. expect a throw:\n";
      cout << saS.pop() << "\n";
    }
  } catch(const char* err) {
    cout << "caught a char* err=" << err << "\n";
  }
}
