// add load/save, clear, math functions

#include <list>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

class Calc {
private:
  list<double> s;
  double A;
public:
  Calc() { A = 0.; }

private:
  void clear()
  {
     A = 0.;
     while(!s.empty())
       s.pop_front();
  }

  void noOperand(string& opCode)
  {
    double result;
         if(opCode == "pi") result = M_PI;
    else if(opCode == "e")  result = M_E;
    else {
      cout << "unknown operator " << opCode << "\n";
      return;
    }
    cout << result << "\n";
    s.push_front(result);
  }

  void loadsave(string& opCode)
  {
    if(s.size() >= 1 && opCode == "save/a") { // save top of stack 
      A = s.front();
      cout << A << "\n";
    } else if(opCode == "load/a") {           // load A 
      cout << A << "\n";
      s.push_front(A);
    } else
      cout << "unknown operator " << opCode << "\n";
  }

  void singleOperand(string& opCode)
  {
    if(s.size() >= 1) {
      double a = s.front(); s.pop_front();
      double result;
           if(opCode == "sin")     result = sin(a);
      else if(opCode == "cos")     result = cos(a);
      else if(opCode == "tan")     result = tan(a);
      else if(opCode == "sqrt")    result = sqrt(a);
      else if(opCode == "abs")     result = abs(a);
      else if(opCode == "log")     result = log(a);
      else if(opCode == "exp")     result = exp(a);
      else {
        cout << "unknown operator " << opCode << "\n";
        return;
      }
      cout << result << "\n";
      s.push_front(result);
    } else
      cout << "need at least one number\n";
  }

  void dualOperands(string& opCode)
  {
    if(s.size() >= 2) {
      double a = s.front(); s.pop_front();
      double b = s.front(); s.pop_front();
      double result;
           if(opCode == "+")  result = a + b;
      else if(opCode == "-")  result = a - b;
      else if(opCode == "*")  result = a * b;
      else if(opCode == "/")  result = a / b;
      else if(opCode == "**") result = exp(b*log(a));
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

      else if(input == "clear")   clear();

      else if(input.size() == 1 && string("-+*/").find(input) != string::npos)
        dualOperands(input);

      else if(input == "**")      dualOperands(input);

      else if(input == "e")       noOperand(input);
      else if(input == "pi")      noOperand(input);

      else if(input == "sin")     singleOperand(input);
      else if(input == "cos")     singleOperand(input);
      else if(input == "tan")     singleOperand(input);
      else if(input == "sqrt")    singleOperand(input);
      else if(input == "abs")     singleOperand(input);
      else if(input == "log")     singleOperand(input);
      else if(input == "exp")     singleOperand(input);

      else if(input == "save/a")  loadsave(input);
      else if(input == "load/a")  loadsave(input);

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

    // for(auto it = s.begin(); it != s.end(); ++it) // C++ 98
      // cout << *it << "\n";

    for(auto d : s)  // C++ 11
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
