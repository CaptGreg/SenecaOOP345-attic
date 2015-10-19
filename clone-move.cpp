#include <iostream>
using namespace std;

class Fruit {
public:
    virtual ~Fruit() {}
    virtual Fruit* clone() const = 0;
    virtual Fruit* move_clone()  = 0;
    virtual void print()  = 0;
};
 
class Apple : public Fruit
{
  string name;
public:
    Apple() : name("unknown") { cout << "Apple() " << (void*) this << "\n"; }
    Apple(string n) : name(n) {cout << "Apple(" << name << ") " << (void*) this << "\n"; }
    ~Apple() { cout << "~Apple() name=" << name << " " <<  (void*) this << "\n"; }

    virtual Apple* clone() const { return new Apple(*this); }
 
    virtual Apple* move_clone()  { return new Apple( move(*this) ); }

    void print() { cout << "Apple::print " << name <<  " " << (void*) this << "\n"; }
};

class Grape : public Fruit
{
  string name;
public:
    Grape() : name("unknown") { cout << "Grape() " << (void*) this << "\n"; }
    Grape(string n) : name(n) {cout << "Grape(" << name << ") " << (void*) this << "\n"; }
    ~Grape() { cout << "~Grape() name=" << name << " " <<  (void*) this << "\n"; }

    virtual Grape* clone() const { return new Grape(*this); }
 
    virtual Grape* move_clone()  { return new Grape( move(*this) ); }

    void print() { cout << "Grape::print " << name <<  " " << (void*) this << "\n"; }
};


void App()
{
   Apple* rottenApple = new Apple("rottenApple");

   Grape* sourGrapes  = new Grape("sourGrapes");

   Fruit* fruit;

   fruit = rottenApple->clone();
   fruit->print();
   delete fruit;

   fruit = sourGrapes->move_clone();
   fruit->print();
   delete fruit;

   fruit = sourGrapes->clone();
   fruit->print();
   delete fruit;

   delete rottenApple;
   delete sourGrapes;
}

int main()
{
   App();
   return 0;
}
