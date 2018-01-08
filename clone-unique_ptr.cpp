// https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/

#include <iostream>
#include <memory> // unique_ptr
using namespace std;

class Interface1
{
public:
    Interface1()
    {
      cout << "Interface1()     **CTOR** this=" << dynamic_cast<void*> (this) << "\n";
    }
    virtual std::unique_ptr<Interface1> cloneInterface1() const = 0;
    virtual void doSomething() const = 0;
    virtual ~Interface1() = default;
};
 
class Interface2
{
public:
    Interface2()
    {
      cout << "Interface2()     **CTOR** this=" << dynamic_cast<void*> (this) << "\n";
    }
    virtual std::unique_ptr<Interface2> cloneInterface2() const = 0;
    virtual void doSomethingElse() const = 0;
    virtual ~Interface2() = default;
};
 
class Implementation : public Interface1, public Interface2
{
public:
    Implementation()
    {
      cout << "Implementation() **CTOR** this=" << dynamic_cast<void*> (this) << "\n";
    }
    virtual std::unique_ptr<Interface1> cloneInterface1() const override
    {
        return make_unique<Implementation>(*this);
    }
    virtual std::unique_ptr<Interface2> cloneInterface2() const override
    {
        return make_unique<Implementation>(*this);
    }
    virtual void doSomething() const override
    {
      cout << "Implementation::doSomething\n";
    }
    virtual void doSomethingElse() const override
    {
      cout << "Implementation::doSomethingElse\n";
    }
    ~Implementation()
    {
      cout << "~Implementation() **DTOR** this=" << dynamic_cast<void*> (this) << "\n";
    }

};

Implementation implementation;

int main()
{
  cout << "main starting\n";
  atexit([]{cout << "atexit\n";});

  cout << "implementation: " << dynamic_cast<void*> (&implementation) << "\n";

  implementation.doSomething();
  implementation.doSomethingElse();

  std::unique_ptr<Interface1> clone1 = implementation.cloneInterface1();
  // cout << "clone1 = "       << dynamic_cast<void*> (&clone1)      << "\n";
  // error: cannot dynamic_cast ‘& clone1’ (of type ‘class std::unique_ptr<Interface2>*’) to type ‘void*’ (source type is not polymorphic)

  cout << "clone1       = " << reinterpret_cast<void*> (&clone1)      << "\n";
  cout << "clone1.get() = " << dynamic_cast    <void*> (clone1.get()) << "\n";
  cout << "\n";
  cout << "calling doSomething from clone1()     --> ";
  clone1->doSomething();
  cout << "\n";

  std::unique_ptr<Interface2> clone2 = implementation.cloneInterface2();
  cout << "clone2       = " << reinterpret_cast<void*> (&clone2)      << "\n";
  cout << "clone2.get() = " << dynamic_cast    <void*> (clone2.get()) << "\n";
  cout << "\n";
  cout << "calling doSomethingElse from clone2() --> ";
  clone2->doSomethingElse();
  cout << "\n";

  cout << "main over\n";
}
