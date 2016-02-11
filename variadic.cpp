 // see fastforward.cpp
 // see variadic_templates.cpp

 // Variadic Templates
 // variadic.cpp

 #include <iostream>

 template <typename T>
 void print(const T& t) {
     std::cout << t << "\n";
 }

 template <typename T, typename... etc>
 void print(const T& t,const etc&... pp) {
     std::cout << t << " | ";
     print(pp...);
 }


#if 0
 // Can we use a variadic templete with an operator overload?
 // NO!   - It doesn't compile :-( 
 template <typename T>
 std::ostream& operator<< (std::ostream& os, const T& t) 
 {
     os << "my<< " << t;
     return os;
 }

#if 0
 // isolate error message by coding an operator different  from '<<'
 // try with '!='
 template <typename T, typename... etc>
 std::ostream& operator<< (std::ostream& os, const T& t ,const etc&... pp) 
 // error: overloaded 'operator<<' must be a binary operator (has 3 parameters)
 // std::ostream& operator!= (std::ostream& os, const T& t ,const etc&... pp) 
 // error: overloaded 'operator!= must be a binary operator (has 3 parameters)

 {
// error: overloaded 'operator<<' must be a binary operator (has 3 parameters)
     os << t;
     os << (pp...);
     return os;
 }
#endif
#endif

 int main() {
     print(100);
     print("abcd", 100, 34.56, 'c');
     std::cout << 100;
     std::cout << "abcd" << 100 <<  34.56 <<  'c' << "\n";
}
