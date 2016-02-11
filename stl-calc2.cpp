#include <vector>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

template <class T>
class StlStackArray {
  vector<T>     s;           // stack
  int           sp;          // stack pointer
  int           _capacity;
public:
  StlStackArray(size_t capacity = 100) {
    s = vector<T>(capacity);
    sp = 0;
    _capacity = capacity; 
  }

  void push(T data) {
    if(sp < _capacity) { 
      s[sp++] = data; 
    } else 
      throw "push: stack overflow (capacity exceeded)"; 
  }

  T top() {
    if(sp > 0) { 
      return s[sp-1];
    } else 
      throw "pop: stack underflow (pop on empty stack empty)";
  }

  void pop() {
    if(sp > 0) { 
      --sp;
    } else 
      throw "pop: stack underflow (pop on empty stack empty)";
  }

  T peek(size_t index) { // used to print stack
    if( index <= sp ) {
      return s[ sp - index - 1 ];
    } else
      throw "peek: stack underflow";
  }

  size_t size() { 
    return sp;
  }

  size_t capacity() {
    return _capacity;
  }
};

class Calc {
private:
  StlStackArray<double> s;

  void op(string& opCode)
  {
    if(s.size() >= 2) {
      double a = s.top(); s.pop();
      double b = s.top(); s.pop();
      double result;
           if(opCode == "+") result = a + b;
      else if(opCode == "-") result = a - b;
      else if(opCode == "*") result = a * b;
      else if(opCode == "/") result = a / b;
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
      else if(input.size() == 1 && string("-+*/").find(input) != string::npos)
        op(input);
      else cout << input << "<== not understood\n";

      cout << "\n";
      cout << "Stack:\n";
      print();
    }
  }
  
  void print()
  {
    for(size_t i = 0; i < s.size(); i++)
      cout << s.peek(i) << "\n";
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
