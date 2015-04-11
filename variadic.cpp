 // see fastforward.cpp
 // see variadic_templates.cpp

 // Variadic Templates
 // variadic.cpp

 #include <iostream>

 template <typename T>
 void print(const T& t) {
     std::cout << t << std::endl;
 }

 template <typename T, typename... etc>
 void print(const T& t,const etc&... pp) {
     std::cout << t << " | ";
     print(pp...);
 }


#if 0
 // it would be great if we could wrap << with a mutex.
 template <typename T>
 std::ostream& operator<<(std::ostream& os, const T& t) {
     os << "my<< " << t;
 }

 template <typename T, typename... etc>
 std::ostream& operator<<(std::ostream& os, const T& t ,const etc&... pp) {
// error: overloaded 'operator<<' must be a binary operator (has 3 parameters)
     os << t;
     os << (pp...);
 }
#endif

 int main() {
     print(100);
     print("abcd", 100, 34.56, 'c');
     std::cout << 100;
     std::cout << "abcd" << 100 <<  34.56 <<  'c' << "\n";
}
