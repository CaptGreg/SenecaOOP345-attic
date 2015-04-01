#include <iostream>
using namespace std;


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ftp://www.cs.uregina.ca/pub/class/210/C++FAQ/virtual-functions.html
// http://www.parashift.com/c++-faq-lite/virtual-ctors.html
//  [20.8] What is a "virtual constructor"? 

// An idiom that allows you to do something that C++ doesn't directly support.

// You can get the effect of a virtual constructor by a virtual clone() member function 
// (for copy constructing), or a virtual create() member function (for the default constructor). 

class B {
public:
  B() {cout<<"B:ctor\n";}
  virtual ~B() {cout<<"B:dtor\n";} // A virtual destructor
  virtual B* clone()  const = 0;   // Uses the copy constructor
  virtual B* create() const = 0;   // Uses the default constructor
};

class D : public B {
public:
  D() {cout<<"D:ctor\n";}
  ~D() {cout<<"D:dtor\n";}
  D* clone()  const;   // Covariant Return Types; see below - ABC defines B*clone(),  not D*clone()
  D* create() const;   // Covariant Return Types; see below - ABC defines B*create(), not D*create()
};

D* D::clone()  const { cout <<"D* D::clone()\n";   return new D(*this); }
D* D::create() const { cout <<"D* D::create()\n";  return new D();      }

// In the clone() member function, the new D(*this) code calls D's copy constructor to copy 
// the state of this into the newly created D object. (Note: unless D is known to be final 
// (AKA a leaf), you can reduce the chance of slicing by making its copy constructor protected.) In 
// the create() member function, the new D() code calls D's default constructor.

// Users use these as if they were "virtual constructors": 

void userCode(B& s)
{
  cout<<"++++ userCode B* s2 = s.clone();\n";
  B* s2 = s.clone();
  cout<<"++++ userCode B* s3 = s.create();\n";
  B* s3 = s.create();
  cout<<"++++ userCode delete s2 (created by D::clone)\n";
  delete s2;    // You need a virtual destructor here
  cout<<"++++ userCode delete s3 (created by D::create)\n";
  delete s3;    // of not, delete will call ~B() not the ~D() destructor
}


// This function will work correctly regardless of whether the B is a D or some other 
// kind-of class derived from B that doesn't even exist yet.

// Note: The return type of D's clone() member function is intentionally different from the return 
// type of B's clone() member function. This is called Covariant Return Types, a feature that was not 
// originally part of the language. If your compiler complains at the declaration of D* clone() const 
// within class D (e.g., saying "The return type is different" or "The member function's type differs 
// from the base class virtual function by return type alone"), you have an old compiler and you'll have to change the return type to B*.

// Note: If you are using Microsoft Visual C++ 6.0, you need to change the return types in the derived 
// classes to B*. This is because MS VC++ 6.0 does not support this feature of the language. Please 
// do not write me about this; the above code is correct with respect to the C++ Standard (see 10.3p5); 
// the problem is with MS VC++ 6.0. Fortunately covariant return types are properly supported by MS VC++ 7.0. 


int main1()
{
  std::cout << "\nmain1\n";
  cout<<"++++ main1 create d\n";
  D d;
  userCode(d);
  cout<<"++++ main1 d going out of scope\n";

  return 0;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// https://katyscode.wordpress.com/2013/08/22/c-polymorphic-cloning-and-the-crtp-curiously-recurring-template-pattern/

int main2()
{
    class Vehicle
    {
    public:
        virtual ~Vehicle() {}
        virtual Vehicle *clone() const = 0;
        virtual void describe() const = 0;
    };
 
    class Car : public Vehicle
    {
    public:
        virtual Car *clone() const { return new Car(*this); } // Covariant Return Types
        virtual void describe() const { std::cout << "I am a car" << "\n";
        }
    };
 
    class Plane : public Vehicle
    {
    public:
        virtual Plane *clone() const { return new Plane(*this); } // Covariant Return Types
        virtual void describe() const { std::cout << "I am a plane" << "\n"; }
    };
 
    class FighterPlane : public Plane
    {
    public:
        virtual FighterPlane *clone() const { return new FighterPlane(*this); } // Covariant Return Types
        virtual void describe() const { std::cout << "I am a fighter plane" << "\n"; }
    };

    std::cout << "\nmain2\n";
 
    Vehicle *vehicleUnknown;
    Vehicle *car = new Car();
    Vehicle *plane = new Plane();
    Vehicle *fighterplane = new FighterPlane();
 
    car->describe();
    plane->describe();
    fighterplane->describe();
 
    cout << "+++ vehicleUnknown = cloned plane\n";
    vehicleUnknown = plane->clone();
    vehicleUnknown->describe();

    cout << "+++ vehicleUnknown = cloned car\n";
    vehicleUnknown = car->clone();
    vehicleUnknown->describe();

    cout << "+++ vehicleUnknown = cloned fighter plane\n";
    vehicleUnknown = fighterplane->clone();
    vehicleUnknown->describe();

    return 0;
}

#if 0

// This pattern takes advantage of the fact that when a virtual function in a base class 
// which returns an object of the class’s type is overridden by a derived class, the return 
// type can be changed to that of the derived object’s class type. This is called co-variant 
// return types. Eg. when you define the virtual clone() function in Vehicle as returning an 
// object of type Vehicle, you can override the function in the derived class Plane to return 
// an object of type Plane, and this is not a violation of C++’s strong typing rules. As you 
// can see, the implementation of clone() is essentially the same in each derived class. This 
// is slightly awkward, and – if you have many derived classes – makes the code messier and 
// more error-prone if you forget to implement clone() or copy-paste without changing the object 
// type. What if there was a way to provide the clone() function automatically in each derived type?

// The Curiously Recurring Template Pattern (CRTP)

// The CRTP is a pattern which has many applications, but its main feature is that you can 
// define a class which derives from a template class where the class being defined is itself 
// a template type parameter. For example, if you have a template class MyTemplateClass and 
// you want to define a class called SomeClass, the following code is valid:
class SomeClass : public MyTemplateClass<SomeClass>

// What does this do for us? Well, it means that any function we include in the template class 
// which uses the template type parameter will be duplicated in all the derived classes which 
// use it, with the type name replaced by the name of the class we are defining. Consider the 
// following code:
class Vehicle
{
public:
    virtual ~Vehicle() {}
 
    virtual Vehicle *clone() const = 0;
 
    virtual void describe() const = 0;
};
 
template <typename Derived>
class VehicleCloneable : public Vehicle
{
public:
    virtual Vehicle *clone() const
    {
        return new Derived(static_cast<Derived const &>(*this));
    }
};

// In Vehicle, we define the interface that each derived type must implement, namely clone() a
// nd describe() in this case, as in the original polymorphic cloning example. In VehicleCloneable, 
// we drive from Vehicle and implement the clone() function using the template type parameter 
// Derived to create an object of the type specified when we define each class which derives 
// from VehicleCloneable. The static_cast is required because *this will reference an object 
// of type VehicleCloneable by default, so we need to down-cast to the correct derived type. 
// Now we can re-implement the other vehicles as follows:

class Car : public VehicleCloneable<Car>
{
public:
    virtual void describe() const
    {
        std::cout << "I am a car" << "\n";
    }
};
 
class Plane : public VehicleCloneable<Plane>
{
public:
    virtual void describe() const
    {
        std::cout << "I am a plane" << "\n";
    }
};

// As you can see, each class definition derives from VehicleCloneable using its own type name as the template type parameter. This will cause VehicleCloneable<DerivedVehicleType>::clone() to be inherited by each new derived class. The code in main() remains the same, as does the output.

// This works fine as long as we are only using one level of derivation, but what if we want to derive from an already-derived class like this:
class FighterPlane : public Plane

// How do we convert this definition to use the CRTP?
// class FighterPlane : public VehicleCloneable<????>

// Now we have a problem. If we derive from VehicleCloneable<Plane>, the implementation of clone() will return the wrong type – Plane instead of FighterPlane. If we derive from VehicleCloneable<FighterPlane>, we won’t inherit the fields and methods from Plane.


#endif





// sigh, templates need to be global
template <typename Base, typename Derived>
class VehicleCloneable : public Base
{
public:
	using Base::Base;
	// The using line causes all constructors to be automatically inherited from 
	// whichever base class is specified in the Base template type parameter, 
	// bringing them into whatever specialization of VehicleCloneable we are 
	// currently deriving from.
	virtual Base *clone() const { return new Derived(static_cast<Derived const &>(*this)); }
};

int main3()
{
    class Vehicle
    {
    protected:
        int fuelCapacity;
    public:
        Vehicle() {}
        Vehicle(int fuelCapacity) : fuelCapacity(fuelCapacity) {}
        virtual ~Vehicle() {}
        virtual Vehicle *clone() const = 0;
     
        virtual void describe() const = 0;
    };
     
    class Car : public VehicleCloneable<Vehicle, Car>
    {
    private:
     typedef VehicleCloneable<Vehicle, Car> BaseClass;
    public:
     Car() {}
     Car(int fuelCapacity) : BaseClass(fuelCapacity) {}

     virtual void describe() const { std::cout << "I am a car, fuel=" << fuelCapacity << "\n"; }
    };
     
    class Plane : public VehicleCloneable<Vehicle, Plane>
    {
    private:
     typedef VehicleCloneable<Vehicle, Plane> BaseClass;
    protected:
     int wingSpan;
    public:
     Plane() {}
     Plane(int fuelCapacity, int wingSpan) : BaseClass(fuelCapacity), wingSpan(wingSpan) {}

     virtual void describe() const { std::cout << "I am a plane, fuel=" << fuelCapacity << ", wingspan=" << wingSpan << "\n"; }
    };
     
    class FighterPlane : public VehicleCloneable<Plane, FighterPlane>
    {
    private:
     typedef VehicleCloneable<Plane, FighterPlane> BaseClass;
    protected:
     int numberOfBombs;
    public:
     FighterPlane() {}
     FighterPlane(int fuelCapacity, int wingSpan, int numberOfBombs)
     : BaseClass(fuelCapacity, wingSpan), numberOfBombs(numberOfBombs) {}

     virtual void describe() const { std::cout << "I am a fighter plane, fuel=" << fuelCapacity << ", wingspan=" << wingSpan << ", bombs=" << numberOfBombs << "\n"; }
    };

    // We have given the Vehicle class a fuelCapacity, the Plane class a wingSpan (and the 
    // constructor also passes the fuel capacity back to Vehicle), and the FighterPlane class 
    // (which is itself derived indirectly from Plane via the CRTP middle-man class 
    // VehicleCloneable) a numberOfBombs, and again this classes passes back the fuel capacity 
    // and wing span to Plane.

    // To simplify calling of the base constructor, we define a private typedef in each derived 
    // class specifying the templated base class to refer to (called BaseClass).

    // You might at first glance think this should work nicely, but unfortunately it won’t compile. 
    // The problem comes with the fact that the base class constructors being called are all 
    // expected to be in VehicleCloneable<…>, not the ‘conceptual’ base classes we want. Adding 
    // in the middle-man class distorts the inheritance hierarchy such that the constructor calls 
    // can’t be forwarded to the conceptual base classes directly, only via VehicleCloneable.

    // What if we try to just reference the conceptual base classes directly in our base constructor 
    // calls, eg. Vehicle from Car and Plane from FighterPlane? Unfortunately, since we haven’t derived 
    // directly from these – rather from VehicleCloneable – the compiler will emit an error saying that 
    // the class being referenced in the base constructor call is not a base class.

    // The C++11 solution

    // If you are using a C++11 compiler which supports constructor inheritance (that includes gcc and 
    // clang, but not Visual Studio 2012 or 2013), the solution is trivial and one line long:

    // template <typename Base, typename Derived>
    // class VehicleCloneable : public Base
    // {
    // public:
    // using Base::Base; <-- add this line!
    //  
    // virtual Base *clone() const { return new Derived(static_cast<Derived const &>(*this)); }
    // };
 
    std::cout << "\nmain3\n";

    Vehicle *vehicleUnknown;
    Vehicle *car          = new Car(50);
    Vehicle *plane        = new Plane(10000,100);
    Vehicle *fighterplane = new FighterPlane(1000,60,5);
 
    car->describe();
    plane->describe();
    fighterplane->describe();
 
    cout << "+++ vehicleUnknown = cloned plane\n";
    vehicleUnknown = plane->clone();
    vehicleUnknown->describe();

    cout << "+++ vehicleUnknown = cloned car\n";
    vehicleUnknown = car->clone();
    vehicleUnknown->describe();

    cout << "+++ vehicleUnknown = cloned fighter plane\n";
    vehicleUnknown = fighterplane->clone();
    vehicleUnknown->describe();

    return 0;
}

int main(int argc, char** argv)
{
  main1();
  main2();
  main3();
}
