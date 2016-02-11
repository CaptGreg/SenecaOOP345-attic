#include <stack>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>           // fmod (remainder operator for doubles)
using namespace std;

class Calc {
private:
  stack<double> s; // s for stack

  void op(string& opCode)
  {
    if(s.size() >= 2) {
      double a = s.top(); s.pop();
      double b = s.top(); s.pop();
      double result;
           if(opCode == "+") result = b + a;
      else if(opCode == "-") result = b - a;
      else if(opCode == "*") result = b * a;
      else if(opCode == "/") result = b / a;
      else if(opCode == "%") result = fmod(b, a);
      else {
        cout << "unknown operator " << opCode << "\n";
        return;
      }
      cout << result << "\n";
      s.push(result);
    } else
      cout << "need two numbers\n";
  }
public:
  void calc() {
    string input;
    cout << "q to quit\n";

    while(true) {
      cout << "input>";
      cin >> input;

      if(input == "q")
        break;

      double d;
      if(istringstream(input) >> d)  // use stringstream to convert string to number
        s.push(d);
      else if(input.size() == 1 && string("-+*/%").find(input) != string::npos)
        op(input);
      else cout << input << "<== not understood\n";
    }
  }
  
  void print()
  {
    // no iterators with stack, need to use list or deque

    // for(stack<double>::iterator it = s.begin(); it ; it != s.end()) // c++ 98
      // cout << *it << "\n";

    // for(auto it = s.begin(); it ; it != s.end()) // c++ 98
      // cout << *it << "\n";

    // for(auto d : s) // c++ 11
      // cout << d << "\n";
  }
};

int main(int argc, char **argv)
{
  try {
    Calc c;
    c.calc();
  } catch(const char* err) {
    cout << "caught a char* err=" << err << "\n";
  }
}
