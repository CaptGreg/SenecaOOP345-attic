#include <iostream>
#include <cstring>

using namespace std;

class diyString {
  char* s;
public:
  diyString() : s(nullptr) { }
  diyString(const char*string) : s(nullptr) {
    if(string) 
      s = strcpy(new char[strlen(string)+1], string);
  }
  ~diyString() { if(s) delete [] s; }          // rule of 3: destructor

  diyString& operator= (const diyString& rhs)  // rule of 3: assignment operator
  {
    if(this != &rhs) {
      if(s) delete [] s;
      s = nullptr;
      if(rhs.s) s = strcpy(new char[strlen(rhs.s)+1], rhs.s);
    }
    return *this;
  }
  diyString(const diyString& rhs)              // rule of 3: copy constructor
  {
    s = nullptr;
    *this = rhs;
  }
#if 0
  diyString& operator= (diyString&& rhs)  // rule of 5: move assignment operator
  {
    if(this != &rhs) {
      if(s) delete [] s;
      s = rhs.s;         // move pointer, steal "brains"
      rhs.s = nullptr;   // no "brains"
    }
    return *this;
  }
#else
  diyString&& operator= (diyString&& rhs)  // rule of 5: move assignment operator
  {
    if(this != &rhs) {
      if(s) delete [] s;
      s = rhs.s;         // move pointer, steal "brains"
      rhs.s = nullptr;   // no "brains"
    }
    return std::move(*this);
  }
#endif

  diyString(diyString&& rhs)              // rule of 5: move constructor
  {
    s = nullptr;
    *this = std::move(rhs);
  }

  void print(const char* label = nullptr)
  {
    if(label) cout << label;
    if(s)
      cout << s << "\n";
    else
      cout << "nullptr - no \"brain\", a zombie\n";

  }

  diyString& operator+= (const diyString& rhs)  // append operator
  {
    if(rhs.s) {
      if(s) {
         char* p = new char[strlen(s) + strlen(rhs.s) + 1];
         strcat(strcpy(p,s), rhs.s);
         delete [] s;
         s = p;
      } else { // s is nullptr, rhs.s is not
         s = new char[strlen(rhs.s) + 1];
         strcpy(s,rhs.s);
      }
    }
    return *this;
  }

};

int main(int argc, char**argv, char**env)
{
  diyString s1;               // default constructor
  s1.print("s1=");
  diyString s2;               // default constructor
  s2.print("s2=");

  s1 = s2;                    // assignment operator
  s1.print("s1=");
  s2.print("s2=");

  diyString s3("C++11");      // constructor
  s3.print("s3=");

  s2 = s3;                    // assignmant operator
  s2.print("s2=");
  s3.print("s3=");

  diyString s4(s2);           // copy constructor
  s4.print("s4=");
  s2.print("s2=");

  s4 = std::move(s2);         // move assignmant operator
  s4.print("s4=");
  s2.print("s2=");

  diyString s5(std::move(s4)); // move constructor
  s5.print("s5=");
  s4.print("s4=");

  s4 += "[added something]";
  s4.print("s4+= add to zombie -->");
  s4 += "{added something else}";
  s4.print("s4+= add to non-zombie -->");
}
