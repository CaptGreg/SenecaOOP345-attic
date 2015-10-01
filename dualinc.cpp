#include <vector>
#include <iostream>
#include <cstdint>  // intptr_t

using namespace std;

int main(int argc,char**)
{
    int i;

    std::vector<int> v = { 1, 2, 3, 4 };
    for(auto e : v) cout << e; cout << "\n";

    i = 0; 
    i = i++;
    cout << "after 'i = 0; i = i++;'\n";
    cout << "'i = " << i << "\n";

    i = 0; v[i++] = i++;
    cout << "after 'i = 0; v[i++] = i++;'\n";
    for(auto e : v) cout << e; cout << ", i=" << i << "\n";

    for(int i=0; i < v.size(); i++) v[i] = i+1;
    for(auto e : v) cout << e; cout << "\n";

    i = 0; v[++i] = ++i;
    cout << "after 'i = 0; v[++i] = ++i;'\n";
    for(auto e : v) cout << e; cout << ", i=" << i << "\n";

    cout << "============================ more pointers =========================================\n";

     char chars[27];
     for(int i= 0; i < 26; i++) chars[i] =  'A' + i; chars[26] = '\0';
     cout << chars << "\n";
     char *p;
     char c;

     p = chars +1;

     c = *p;   // go to the memory addressed by p, treat the value there as a char
     p++;      // increment the address

     auto print = [] (char*chars,char*p,char c) {
       unsigned index = p - chars;
       
       cout << "c=" << c << ", chars=" << chars << ", p->chars[" << intptr_t(p-chars) << "], *p="<< *p << "\n";
       for(int i = index + 11; i ; i--) cout << " "; 
       cout << "^p\n";
     };
     print(chars,p,c);

     // or the same thing in one line:
     c = *p++; // go to the memory addressed by p, treat the value there as a char, increment the address
     print(chars,p,c);

     // increment the address first:
     p++;      // increment the address
     c = *p;   // go to the memory addressed by p, treat the value there as a char
     print(chars,p,c);

     // or the same thing in one line:
     c = *(++p);   // increment the address, go to the memory addressed by p, treat the value there as a char
     print(chars,p,c);

     // now let's increment the value pointed at by p:
     c = (*p)++;
     print(chars,p,c);

     // and again using the prefix ++ operator:
     c = ++(*p);
     print(chars,p,c);

     // what happens without brackets?
     c = ++*p;
     print(chars,p,c);
}
