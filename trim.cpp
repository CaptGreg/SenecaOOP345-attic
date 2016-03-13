// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
std::string &trim(std::string &s) // GB works!
{
  return ltrim(rtrim(s));
}


// http://www.toptip.ca/2010/03/trim-leading-or-trailing-white-spaces.html

// Note LTrim() and RTrim() return references. They are destructive. 
std::string &LTrim(std::string &s)
{
  // s.erase(s.begin(),find_if_not(s.begin(),s.end(),[](int c){return isspace(c);}));
  auto f = [] (int c) { return isspace(c); }; // GB break out lambda
  s.erase(s.begin(), find_if_not(s.begin(),s.end(), f) ); // GB forward iterators
  return s;
}

std::string &RTrim(std::string &s)
{ 
  // s.erase(find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base(), s.end());
  auto f = [] (int c) { return isspace(c); }; // GB break out lambda
  s.erase( find_if_not( s.rbegin(), s.rend(), f ).base(), s.end()); // GB reverse iterators
  return s;
}

std::string TRim(const std::string &s) // GB works!
{
  std::string t=s; // did trim inline, GB convert to returning trimmed result
  return LTrim(RTrim(t));
}


std::string simpleTrim( std::string& s)
{
  while(!s.empty() && isspace(s[0]) ) // trim leading spaces
    s.erase( 0, 1 );

  while(!s.empty() && isspace(s[s.size()-1]) ) // trim trailing spaces
    s.erase( s.size()-1, 1 );

  return s;
}

#include <iostream> 
#include <vector> 
int main() // GB wrote this test code for the above pair of trim routines.
{
   std::vector<std::string> tests = {
     std::string ( "   a   " ),
     std::string ( "" ),
     std::string ( "   " ),
     std::string ( "\t\t\t\t\t" ),
     std::string ( "B      " ),
     std::string ( "   \t C" ),
   };

   std::cout << "\nDATA->"; for(auto e : tests) std::cout << ":" << e;  std::cout << ":\n";

   std::cout << "\nTRim->"; for(auto e : tests) std::cout << ":" << TRim(e);  std::cout << ":\n";

   std::cout << "\ntrim->"; for(auto e : tests) std::cout << ":" << trim(e);  std::cout << ":\n";

   std::cout << "\nsimpleTrim->"; for(auto e : tests) std::cout << ":" << simpleTrim(e);  std::cout << ":\n";
    
   return 0;
}
