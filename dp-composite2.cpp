// http://patterns.pl/src/composite.cpp

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Component
{
public:
  virtual void print() = 0;
};

class Composite : public Component
{
  string name;
  vector<Component*> elements;

public:
  Composite(const string & s) : name(s) {}

  void add(Component* element) { elements.push_back(element); }

  void remove(Component* element)
  { 
    // removes element from collection
    elements.erase(std::remove(elements.begin(), elements.end(), element), elements.end());
  }
  
  Component* getChild(int index) { return elements[index]; }

  void print() override
  {
    cout << name << endl;
		
    for (auto &e : elements)
      e->print();
  }
};

class Leaf : public Component
{
  string name;
public:

  Leaf(string s) : name(s) {}
  
  void print() override
  {
    cout << name << endl;
  }
};

void printComp(Component & c)
{
  c.print();
}

int main()
{
  // unix directory structure
  Composite root("/");
  Composite usr("usr");
  Composite home("home");
  Composite dev("dev");
  Composite user("user");
  Leaf file("file.txt");

  user.add(&file);
  home.add(&user);

  root.add(&usr);
  root.add(&dev);
  root.add(&home);

  // composition of objects and object are treating in the same way 
  printComp(root);
  printComp(file);

  return 0;
}
