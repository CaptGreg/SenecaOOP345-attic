#include <cstring> // strcpy, strlen
#include <iostream>
using namespace std;

 class Base {
     char string[31];
     int where;
 public:
     Base() {
         strcpy(string, "ace");
         where = 2;
         cout << "Base ctor\n";
     }
     Base(char s[]) {
         strcpy(string, s);
         where = 0;
         cout << "Base* ctor\n";
     }
     ~Base() {
         cout << "~Base dtor\n";
     }
     void show() {
         cout << string << '\n';
     }
     void next() {
         increment(where);
         if (where >= (int) strlen(string))
             where = 0;
     }
     virtual void increment(int &n) { n++; }
     void access(char val) { string[where] = val; }
     char access()         { return string[where]; }
 };

 class Getter: virtual public Base {
 public:
     Getter() {
         cout << "Getter ctor\n";
     }
     Getter(char *s)  : Base(s) {
         cout << "Getter* ctor\n";
     }
     ~Getter() {
         cout << "~Getter dtor\n";
     }
     void increment(int &n) {
         n += 2;
         cout << n << " Getter increment\n";
     }
     char get() {
         char c = access();
         next();
         return c;
     }
 };

 class Setter: virtual public Base {
 public:
     Setter() {
         cout << "Setter ctor\n";
     }
     Setter(char *s) : Base(s) {
         cout << "Setter* ctor\n";
     }
     ~Setter() {
         cout << "~Setter dtor\n";
     }
     void increment(int &n) {
         n += 3;
         cout << n << " Setter increment\n";
     }
     void set(char c) {
         access(c);
         next();
     }
 };

 class Better: public Setter, public Getter {
     int toggle;
 public:
     Better() {
         toggle = 0;
         cout << "Better ctor\n";
     }
     Better(char *s) : Base(s) {
         toggle = 0;
         cout << "Better* ctor\n";
     }
     ~Better() {
         cout << "~Better dtor\n";
     }
     void increment(int &n) {
         if (toggle)
             Setter::increment(n);
         else
             Getter::increment(n);
         toggle = !toggle;
     }
 };

 int main (int argc, char**argv)
{
     cout << "\n" << argv[0] << "\n" << "\n";
     cout << "\nmain creating 'x' - calling Better x(\"012345678901234567890\");\n";
     Better x( (char*) "012345678901234567890");
     cout << "main calling x.show();\n";
     x.show();

     cout << "\nmain calling x.set('f');\n";
     x.set('f');
     cout << "main calling x.show();\n";
     x.show();

     cout << "\nmain calling x.set(x.get());\n";
     x.set(x.get());
     cout << "main calling x.show();\n";
     x.show();

     cout << "\nmain calling x.set('a');\n";
     x.set('a');
     cout << "main calling x.show();\n";
     x.show();

     cout << "\nmain calling x.set('A');\n";
     x.set('A');
     cout << "main calling x.show();\n";
     x.show();

     cout << "\nmain class instance 'x' going out of scope - dtor's will be called ---  NOTE order!\n";
 }
