// GB select ignore empty variables if desired

#include <iostream>
#include <vector>
#include <string>
using namespace std;

const vector<string> splitChar(const string& s, const char& delimiter, const bool bIgnoreEmpty = false)
{
  string token;
	vector<string> v;
	
	for(auto n:s) {
		if(n != delimiter) {
      token += n; 
    } else if(not token.empty()) {  // not empty, add token
      v.push_back( move(token) );
    } else if(not bIgnoreEmpty) {   // empty, add empty token anyway
      v.push_back( move(token) );
    }
	}
	if(not token.empty()) 
    v.push_back( move(token) );
	
	return v;
}

vector<string> splitString(string& s, const string delimiter, const bool bIgnoreEmpty = false) // destroys s
{
  string token;
  vector<string> v;

  size_t pos = 0;
  while ((pos = s.find(delimiter)) != string::npos) {
      token = s.substr(0, pos);
      if(not token.empty()) {          // not empty, add token
        v.push_back( move(token) );
      } else if(not bIgnoreEmpty) {    // empty, add empty token anyway
        v.push_back( move(token) );
      }
      s.erase(0, pos + delimiter.length());
  }
  v.push_back( move(s) );
  return v;
}

int main()
{
	vector<string> v;
	string str;

  {
    char delimiter = ' ';
    str = "the quick brown fox jumps over the lazy   dog";
    cout << '"' << str << '"' << "\n";
    v = splitChar(str, delimiter, true);
    for(auto n:v) cout << "ignoring empty fields <" << n << ">\n";
	
    cout << '"' << str << '"' << "\n";
    v = splitChar(str, delimiter, false);
    for(auto n:v) cout << "storing empty fields <" << n << ">\n";
  }

  {
    string delimiter = string(" ");
    str = "the quick brown fox jumps over the lazy   dog";
    cout << '"' << str << '"' << "\n";
    v = splitString(str, delimiter, true);                  // destroys str
    for(auto n:v) cout << "ignoring empty fields <" << n << ">\n";
	
    str = "the quick brown fox jumps over the lazy   dog";  // reinitialize str
    cout << '"' << str << '"' << "\n";
    v = splitString(str, delimiter, false);
    for(auto n:v) cout << "storing empty fields <" << n << ">\n";
  }
	return 0;
}
