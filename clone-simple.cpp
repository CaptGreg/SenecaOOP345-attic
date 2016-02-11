// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// https://katyscode.wordpress.com/2013/08/22/c-polymorphic-cloning-and-the-crtp-curiously-recurring-template-pattern/

#include <iostream>
#include <typeinfo>   // typeid(...), typeid(...).name()
using namespace std;

int main()
{
    class Vehicle  // ABC: abstract base class
    {
    public:
        virtual ~Vehicle() {}
        virtual Vehicle *clone()    const = 0;
        virtual void     describe() const = 0;
    };
 
    class Car : public Vehicle
    {
    public:
        virtual Car *clone()    const { return new Car(*this); } // Covariant Return Types
        virtual void describe() const { cout << typeid(*this).name() << " -> I am a car\n"; }    };
 
    class Plane : public Vehicle
    {
    public:
        virtual Plane *clone()  const { return new Plane(*this); } // Covariant Return Types
        virtual void describe() const { cout << typeid(*this).name() << " -> I am a plane\n"; }
    };
 
    class PassengerPlane : public Plane
    {
    public:
        virtual PassengerPlane *clone() const { return new PassengerPlane(*this); } // Covariant Return Types
        virtual void describe() const { cout << typeid(*this).name() << " -> I am a PassengerPlane I was derived from (my base class) -->";  Plane::describe(); }
    };

    Vehicle *vehicleUnknown;
    Vehicle *car            = new Car();
    Vehicle *plane          = new Plane();
    Vehicle *passengerplane = new PassengerPlane();
 
    car->describe();
    plane->describe();
    passengerplane->describe();
 
    cout << "+++ vehicleUnknown = cloned plane\n";
    vehicleUnknown = plane->clone();
    vehicleUnknown->describe();

    cout << "+++ vehicleUnknown = cloned car\n";
    vehicleUnknown = car->clone();
    vehicleUnknown->describe();

    cout << "+++ vehicleUnknown = cloned passenger plane\n";
    vehicleUnknown = passengerplane->clone();
    vehicleUnknown->describe();

    return 0;
}
