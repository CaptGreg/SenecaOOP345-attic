#include<iostream>
#include<list> //Library for List Container.
#include <typeinfo>     // typeid
#define TYPEID(X) std::cout << "typeid(" #X ").name()=" << std::typeid(X).name() << "\n";

using namespace std;

template <typename L>
void display (const L& input) {
  for (auto i = input.cbegin(); i != input.cend(); ++i){
    cout << *i << " ";
  }
  cout << "\n";
}

int main() {
  list <int> intNum1 = {22, 33};
  cout << "Data at inNum1: ";
  display(intNum1);

  list <int> intNum2 = {66, 77};
  cout << "Data at intNum2: ";
  display(intNum2);

  cout << "Now we will enter intNum1 at the end of intNum2" << "\n";; 
  intNum2.insert(intNum2.end(), intNum1.begin(), intNum1.end());
  display(intNum2);
  cout << "======================================"<< "\n";
  list <int> intNum3 = {101, 122};
  cout << "Data at inNum2: ";
  display(intNum3);

  cout << "Now we will enter intNum3 at the 3rd position of intNum2" << "\n";
  // intNum2.insert(intNum2.begin()+3, intNum3.begin(), intNum3.end());
  intNum2.insert(intNum2.begin(), intNum3.begin(), intNum3.end());
  // TYPEID(intNum2.begin());
  // auto l = intNum2.begin()+3;
  auto l = intNum2.begin();
  // TYPEID(l);
  // l+=3; // error
  // l = l + 3; // error
  // l++;  // works
  // l++;  // works
  // l++;  // works
  intNum2.insert(l, intNum3.begin(), intNum3.end());
  
  list<int>::iterator ll = intNum2.begin();
  // TYPEID(ll);
  ll++;
  ll++;
  ll++;
  intNum2.insert(ll, intNum3.begin(), intNum3.end());
  // list<int>::iterator lll = intNum2.begin() + 3;
  cout << "Data at intNum2: ";
  display(intNum2);
}
