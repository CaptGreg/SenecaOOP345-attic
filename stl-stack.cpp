#include <stack>
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
  try {
    cout << endl;
    stack<int> s;
    for(int i = 0 ; i < 5; i++)
      s.push(10*i+1);
    for(int i = 0 ; i < 5; i++) {
      cout << s.top() << endl;
      s.pop();
    }
  } catch(const char* err) {
    cout << "caught a char* err=" << err << endl;
  }

  try {
    cout << endl;
    stack<double> s;
    for(int i = 0 ; i < 5; i++)
      s.push(100.*i+1.1);
    for(int i = 0 ; i < 5; i++) {
      cout << s.top() << endl;
      s.pop();
    }
  } catch(const char* err) {
    cout << "caught a char* err=" << err << endl;
  }

  try {
    cout << endl;
    stack<double> s;
    for(int i = 0 ; i < 15; i++)
      s.push(1000.*i+2.2);
    for(int i = 0 ; i < 5; i++) {
      cout << s.top() << endl;
      s.pop();
    }
  } catch(const char* err) {
    cout << "caught a char* err=" << err << endl;
  }

  try {
    cout << endl;
    stack<string> s;
    s.push( string("one") );
    s.push( string("two") );
    s.push( string("three") );
    s.push( string("four") );
    s.push( string("five") );

    size_t num = s.size();
    for(size_t i = 1 ; i <= num + 1; i++) {
      if(s.empty()) throw "popping an empty stack.\n";
      cout << s.top() << endl;
      s.pop();
    }
  } catch (const char* err) {
    cout << "caught a char* err=" << err << endl;
  }
}
