 #include <iostream>
 #include <cstring>      // GB strcpy, strlen

 using namespace std;
 class Bass {
     char string[31];
     int where;
 public:
     Bass() {
         strcpy(string, "ace");
         where = 2;
         cout << "B\n";
     }
     Bass(char s[]) {
         strcpy(string, s);
         where = 0;
         cout << "B*\n";
     }
     ~Bass() {
         cout << "~B\n";
     }
     void show() {
         cout << string << '\n';
     }
     void next() {
         increment(where);
         if (where >= strlen(string))
             where = 0;
     }
     virtual void increment(int &n) { n++; }
     void access(char val) { string[where] = val; }
     char access()         { return string[where]; }
 };

 class Getter: virtual public Bass {
 public:
     Getter() {
         cout << "G\n";
     }
     Getter(char *s)  : Bass(s) {
         cout << "G*\n";
     }
     ~Getter() {
         cout << "~G\n";
     }
     void increment(int &n) {
         n += 2;
         cout << n << " in getter\n";
     }
     char get() {
         char c = access();
         next();
         return c;
     }
 };

 class Setter: virtual public Bass {
 public:
     Setter() {
         cout << "S\n";
     }
     Setter(char *s) : Bass(s) {
         cout << "S*\n";
     }
     ~Setter() {
         cout << "~S\n";
     }
     void increment(int &n) {
         n += 3;
         cout << n << " in setter\n";
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
         cout << "M\n";
     }
     Better(char *s) : Bass(s) {
         toggle = 0;
         cout << "M*\n";
     }
     ~Better() {
         cout << "~M\n";
     }
     void increment(int &n) {
         if (toggle)
             Setter::increment(n);
         else
             Getter::increment(n);
         toggle = !toggle;
     }
 };

 int main() {
     Better x("oBandyns");

     x.set('f');
     x.set(x.get());
     x.set('a');
     x.set('A');
     x.show();
 }
