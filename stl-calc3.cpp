#include <list>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

class Calc {
private:
  list<double> s;

  void dualOperands(string& opCode)
  {
    if(s.size() >= 2) {
      double a = s.front(); s.pop_front();
      double b = s.front(); s.pop_front();
      double result;
           if(opCode == "+")  result = b + a;
      else if(opCode == "-")  result = b - a;
      else if(opCode == "*")  result = b * a;
      else if(opCode == "/")  result = b / a;
      else {
        cout << "unknown operator " << opCode << "\n";
        return;
      }
      cout << result << "\n";
      s.push_front(result);
    } else
      cout << "need two numbers\n";
  }

public:
  void calc() {
    cout << "q to quit\n";

    while(true) {
      string input;
      cout << "input>";
      cin >> input;

      if(input == "q")
        break;

      double d;
      if(istringstream(input) >> d)  // use stringstream to convert string to number
        s.push_front(d);

      else if(input.size() == 1 && string("-+*/").find(input) != string::npos)
        dualOperands(input);

      else cout << input << "<== not understood\n";

      cout << "\nstack:\n";
      print();
      cout << "\n";
    }
  }
  
  void print()
  {
    // for(list<double>::iterator it = s.begin(); it != s.end(); ++it) // C++ 98
      // cout << *it << "\n";

    // for(auto it = s.begin(); it != s.end(); ++it) // auto is C++ 11
      // cout << *it << "\n";

    for(auto d : s)  // range-based for is C++ 11
      cout << d << "\n";
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
