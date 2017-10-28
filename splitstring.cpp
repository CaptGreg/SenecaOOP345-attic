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
      // add token if 1. not empty or 2. empty and don't ignore empty, 
    } else if(not token.empty() or (token.empty() and not bIgnoreEmpty)) 
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
      // add token if 1. not empty or 2. empty and don't ignore empty, 
      if(not token.empty() or (token.empty() and not bIgnoreEmpty)) 
        v.push_back( move(token) );
      s.erase(0, pos + delimiter.length());
  }
  if(not s.empty() or (s.empty() and not bIgnoreEmpty)) 
    v.push_back( move(s) );
  return v;
}

vector<string> splitGB ( const string& s , char delimiter = ',' , bool bIgnoreEmpty = true) {
  string token;
	vector<string> v;
	
    for(auto c:s)
      if(n != delimiter) 
        token += c; 
      else 
      // add token if 1. not empty or 2. empty and don't ignore empty, 
      if(not token.empty() or (token.empty() and not bIgnoreEmpty)) 
        v.push_back( move(token) );  // add token.
    if(not s.empty() or (s.empty() and not bIgnoreEmpty)) 
      v.push_back( move(token) );
    return v;
  };

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
