// see fastforward.cpp
// see variadic.cpp

// http://www.cplusplus.com/articles/EhvU7k9E/
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// GB 2014-10-18, compile issues using g++ 4.8.2

template<typename... Arguments>             // <<-- compiles
class VariadicTemplate;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Any of the following ways to create an instance of this class template is valid:

// VariadicTemplate<double, float> instance1;
// No! error: 
//  aggregate ‘VariadicTemplate<double, float> instance1’ 
//  has incomplete type and cannot be defined
//  VariadicTemplate<double, float> instance1;

// VariadicTemplate<bool, unsigned short int, long> instance2;
// No! error: 
//  aggregate ‘VariadicTemplate<bool, short unsigned int, long int> instance2’ 
//  has incomplete type and cannot be defined
//  VariadicTemplate<bool, unsigned short int, long> instance2;

// VariadicTemplate<char, std::vector<int>, std::string, std::string, std::vector<long long>> instance3;
// No! error: 
//  aggregate ‘VariadicTemplate<char, std::vector<int, std::allocator<int> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >
//  , std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<long long int, std::allocator<long long int> > > instance3’ 
//  has incomplete type and cannot be defined
//  VariadicTemplate<char, std::vector<int>, std::string, std::string, std::vector<long long>> instance3;

// g++ -Wall -std=c++11  -O0 -ggdb variadic_templates.cpp -o variadic_templates -lrt

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// The number of variadic template arguments can also be zero, so the following

// VariadicTemplate<> instance;
// No! variadic_templates.cpp:21:20: error: aggregate ‘VariadicTemplate<> instance’ has incomplete type and cannot be defined

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// However, if you create a template like this:

template<typename T, typename... Arguments>      // <<-- compiles
class VariadicTemplateXYZ;

// You must set at least one type as a template argument (for typename T), unless default type has been initilialized, like in the following declaration:

template<typename T = int, typename... Arguments> // <<-- compiles
class VariadicTemplateXYZ;
	
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// class templates specialization                 // <<-- compiles
template<typename T>
class Template{
public:
    void SampleFunction(T param){
       cout << "Template<T>::SampleFunction param=" << param << "\n";

    }
};

template<>
class Template<int>{
public:
    void SampleFunction(int param){
       cout << "Template<int>::SampleFunction param=" << param << "\n";

    }
};

// variadic class templates specialization       // <<-- compiles
template<typename... Arguments>
class VariadicTemplate{
public:
    void SampleFunction(Arguments... params){
       cout << "Template<typename ... Arguments>::SampleFunction\n";
       // SampleFunction(params ... );
    }
};

template<>
class VariadicTemplate<double, int, long>{
public:
    void SampleFunction(double param1, int param2, long param3){
       cout << "Template<double,int,long>::SampleFunction param=" 
            << param1 << " " 
            << param2 << " " 
            << param3 << " " 
            << "\n";
    }
};


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <iostream>
 
void tprintf(const char* format) // base function
{
std::cout << __PRETTY_FUNCTION__ << "\n";
    std::cout << format;
}
 
template<typename T, typename... Targs>
void tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
{
std::cout << __PRETTY_FUNCTION__ << "\n";
    for ( ; *format != '\0'; format++ ) {
        if ( *format == '%' ) {
           std::cout << value;
           tprintf(format+1, Fargs...); // recursive call
           return;
        }
        std::cout << *format;
    }
}
 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
template<typename T>
T adder(T v) {
std::cout << __PRETTY_FUNCTION__ << "\n";
  return v;
}

template<typename T, typename... Args>
T adder(T first, Args... args) {
std::cout << __PRETTY_FUNCTION__ << "\n";
  return first + adder(args...);
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://msdn.microsoft.com/en-us/library/dn439779.aspx
void print() {
// std::cout << __PRETTY_FUNCTION__ << "\n";
    cout << endl;
}

template <typename T> 
void print(const T& t) {
// std::cout << __PRETTY_FUNCTION__ << "\n";
    cout << t << endl;
}

template <typename First, typename... Rest> 
void print(const First& first, const Rest&... rest) {
// std::cout << __PRETTY_FUNCTION__ << "\n";
    cout << first << ", ";
    print(rest...); // recursive call using pack expansion syntax
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main(int argc, char**argv)
{
// http://msdn.microsoft.com/en-us/library/dn439779.aspx
    print(); // calls first overload, outputting only a newline
    print(1); // calls second overload
    
    // these call the third overload, the variadic template, 
    // which uses recursion as needed.
    print(10, 20);
    print(100, 200, 300);
    print("first", 2, "third", 3.14159);

long sum = adder(1, 2, 3, 8, 7);
cout << sum << "\n";

std::string s1 = "x", s2 = "aa", s3 = "bb", s4 = "yy";
std::string ssum = adder(s1, s2, s3, s4);

cout << ssum << "\n";

    tprintf("% world% %\n","Hello",'!',123);

   // Template<int> t;
   // t.SampleFunction(15);

   // VariadicTemplate<int> vt;
   // vt.SampleFunction(42);
}
