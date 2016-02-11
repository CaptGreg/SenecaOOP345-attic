 // file "abc-h20.cpp"
 #include <iostream>
 #include <cstring>
 using namespace std;

 class IsDisplayable {
   public:
     virtual void display(ostream& os) const = 0;
 };

 ostream& operator<<(ostream& os, const IsDisplayable& d) {
     d.display(os);
     return os;
 }

 class Quote : public IsDisplayable {
     char* text;
   public:
     Quote(const char* t = "") {
         text = new char[strlen(t) + 1];
         strcpy(text, t);
     }
     Quote(const Quote& q) {
         text = NULL;
         *this = q;
     }
     Quote& operator=(const Quote& q) {
         if (this != &q) {
             if (text) delete [] text;
             text = new char[strlen(q.text) + 1];
             strcpy(text, q.text);
         }
         return *this;
     }
     ~Quote() {
         delete [] text;
     }

     void display(ostream& os) const // our code
     {
        os << "Today's quote is\n" << text << "\n";
     }
     
 };

#include <iostream>
 using namespace std;
 // #include "Quote.h"  // (above code)
 using namespace std;

 int main(int argc, char* argv[]) {

     Quote q;

     q = "early bird gets the worm.";
     cout << q << "\n";

     for (int i = 1; i < argc; i++) {
         Quote quote(argv[i]);
         cout << quote << "\n";
     }

     q = "an apple a day keeps the doctor away.";
     cout << q << "\n";

     return 0;
 }
