// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Edsger Wybe Dijkstra (May 11 1930 – August 6 2002) was a Dutch computer scientist, and winner of the 
// 1972 Turing Award. He was an early and influential proponent of "structured programming."

// Since no one agrees how association and aggregation differ, what would Dijkstra think?
// We know what he thinks about Software Engineering Gurus.

// http://en.wikiquote.org/wiki/Edsger_W._Dijkstra
// The required techniques of effective reasoning are pretty formal, but as long as programming is done 
// by people that don't master them, the software crisis will remain with us and will be considered an 
// incurable disease. And you know what incurable diseases do: they invite the quacks and charlatans in, 
// who in this case take the form of Software Engineering gurus. 
// Dijkstra (2000) "Answers to questions from students of Software Engineering" (EWD 1305).
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// http://www.codeproject.com/Articles/330447/Understanding-Association-Aggregation-and-Composit
//               Association                            Aggregation                              Composition
// Owner         No owner                               Single owner                             Single owner
// Life time     Have their own lifetime                Have their own lifetime                  Owner's life time
// Child object  Child objects all are independent      Child objects belong to a single parent  Child objects belong to a single parent
// Relationship  "using"/"uses"                         "using" with a parent                    "has-a", death relationship
// examples      club/members                           pond + ducks - ducks use a pond          car + engine

// dictinary Aggregate definition, formed by the conjunction or collection of particulars into a 
//  whole mass or sum; total; combined: the aggregate amount of indebtedness.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://stackoverflow.com/questions/885937/difference-between-association-aggregation-and-composition

// Association is a relationship where all objects have their own lifecycle and there is no owner. 
// Let’s take an example of Teacher and Student. Multiple students can associate with single teacher 
// and single student can associate with multiple teachers, but there is no ownership between the 
// objects and both have their own lifecycle. Both can create and delete independently.

// Aggregation is a specialised form of Association where all objects have their own lifecycle, 
// but there is ownership and child objects can not belong to another parent object. 
// Let’s take an example of Department and teacher. A single teacher can not belong to multiple 
// departments, but if we delete the department teacher object will not be destroyed. We can think 
// about it as a “has-a” relationship.

// Composition is again specialised form of Aggregation and we can call this as a “death” relationship.
// It is a strong type of Aggregation. Child object does not have its lifecycle and if parent object is 
// deleted, all child objects will also be deleted. 
// Let’s take again an example of relationship between House and Rooms. House can contain multiple 
// rooms - there is no independent life of room and any room can not belong to two different houses. 
// If we delete the house - room will automatically be deleted. Let’s take another example relationship 
// between Questions and Options. Single questions can have multiple options and option can not belong 
// to multiple questions. If we delete questions options will automatically be deleted.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://stackoverflow.com/questions/25779394/c-association-aggregation-and-composition

// Composition: a Car has-an Engine, a Person has-an Address. Basically, must have, controls lifetime.
// Association: A Car has-a Driver, some class instance has-an ErrorLogger. Lifetime not controlled, may be shared.
// Aggregation: A DOM (Document Object Model, that is the objects that make up a tree of HTML elements) Node has-a (an array of) child Nodes. The Node is top (well, higher) level; it "contains" its children, they don't contain it.

// Independent existence.
// An Invoice is composed of line items. 
// What's a line item that's not on an invoice? It's -- well -- it's nothing. It can't exist independently.
// On the other hand, an Invoice is associated with a Customer.
// Customer has an independent existence, with or without an invoice.
// If the two things have independent existence, they may be associated.
// If one thing cannot exist independently, then it is part of a composition.

// Composition is a stricter relationship than aggregation. Composition means that something is so strongly related to something else that they cannot basically exist independently, or if they can, they live in different contexts.
// Real world example: you define a GUI window, and then a text field where to write something. Between the class defining the GUI and the class defining the text field there's composition. Together, they compose a widget which can be seen as an entity on its own. Suppose you delete the window, and you delete the text field as well.

// Aggregation is different, in the sense that the link between the two entities is temporary, unstable, and occasional. 
// A real world example. Suppose you have a database of objects containing multiple data instances. Now you run some filter to collect the data instances obeying a given criterium, and the resulting instances are pushed into a graphical list so that the user can see them. When the graphical widget receives the objects, it can form an aggregation of these entities, and present them. If the user closes the window with the graphical list, and the latter get deleted, the data objects should not be deleted. Maybe they are displayed somewhere else, or you still need them.

// Also, in general, composition is defined at creation time. Aggregation is instead defined later in the object lifetime.

// Composition means a part of the entity state is encapsulated by another type but it is conceptualy part of the entity state. For example you may have a address type and a employee entity type that includes a address.

// Association means that a entity type is assocciated with another entity type but the assocciated entity is conceptualy not part of the entity state. For example a employee may be assocciated with a company.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://www.holub.com/goodies/uml/#composition
// (1) Composition vs. Aggregation: 

// Neither "aggregation" nor "composition" really have direct analogs in many languages (Java, for example). 

// An "aggregate" represents a whole that comprises various parts; so, a Committee is an aggregate of its Members. A Meeting is an aggregate of an Agenda, a Room, and the Attendees. At implementation time, this relationship is not containment. (A meeting does not contain a room.) Similarly, the parts of the aggregate might be doing other things elsewhere in the program, so they might be referenced by several objects. In other words, There's no implementation-level difference between aggregation and a simple "uses" relationship (an "association" line with no diamonds on it at all). In both cases an object has references to other objects. Though there's no implementation difference, it's definitely worth capturing the relationship in the UML, both because it helps you understand the domain model better, and because there are subtle implementation issues. I might allow tighter coupling relationships in an aggregation than I would with a simple "uses," for example. 

// Composition involves even tighter coupling than aggregation, and definitely involves containment. The basic requirement is that, if a class of objects (call it a "container") is composed of other objects (call them the "elements"), then the elements will come into existence and also be destroyed as a side effect of creating or destroying the container. It would be rare for a element not to be declared as private. An example might be an Customer's name and address. A Customer without a name or address is a worthless thing. By the same token, when the Customer is destroyed, there's no point in keeping the name and address around. (Compare this situation with aggregation, where destroying the Committee should not cause the members to be destroyed---they may be members of other Committees). 

// In terms of implementation, the elements in a composition relationship are typically created by the constructor or an initializer in a field declaration, but Java doesn't have a destructor, so there's no way to guarantee that the elements are destroyed along with the container. In C++, the element would be an object (not a reference or pointer) that's declared as a field in another object, so creation and destruction of the element would be automatic. Java has no such mechanism. It's nonetheless important to specify a containment relationship in the UML, because this relationship tells the implementation/testing folks that your intent is for the element to become garbage collectible (i.e. there should be no references to it) when the container is destroyed).

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://stackoverflow.com/questions/731802/what-is-the-difference-between-composition-and-association-relationship/23464244#23464244
//  COMPOSITION
//Imagine a software firm that is composed of different Business Units (or departments) like Storage BU, Networking BU. Automobile BU. The life time of these Business Units is governed by the lifetime of the organization. In other words, these Business Units cannot exist independently without the firm. This is COMPOSITION. (ie the firm is COMPOSED OF business units)
//ASSOCIATION
//The software firm may have external caterers serving food to the employees. These caterers are NOT PART OF the firm. However, they are ASSOCIATED with the firm. The caterers can exist even if our software firm is closed down. They may serve another firm! Thus the lifetime of caterers is not governed by the lifetime of the software firm. This is typical ASSOCIATION
//AGGREGATION
//Consider a Car manufacturing unit. We can think of Car as a whole entity and Car Wheel as part of the Car. (at this point, it may look like composition..hold on) The wheel can be created weeks ahead of time, and it can sit in a warehouse before being placed on a car during assembly. In this example, the Wheel class's instance clearly lives independently of the Car class's instance. Thus, unlike composition, in aggregation, life cycles of the objects involved are not tightly coupled.
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://www.learncpp.com/cpp-tutorial/102-composition/
// 10.2 — Composition
// BY ALEX, ON DECEMBER 4TH, 2007
// In real-life, complex objects are often built from smaller, simpler objects. For example, a car is built using a metal frame, an engine, some tires, a transmission, a steering wheel, and a large number of other parts. A personal computer is built from a CPU, a motherboard, some memory, etc… Even you are built from smaller parts: you have a head, a body, some legs, arms, and so on. This process of building complex objects from simpler ones is called composition (also known as object composition).
// More specifically, composition is used for objects that have a has-a relationship to each other. A car has-a metal frame, has-an engine, and has-a transmission. A personal computer has-a CPU, a motherboard, and other components. You have-a head, a body, some limbs.
// So far, all of the classes we have used in our examples have had member variables that are built-in data types (eg. int, double). While this is generally sufficient for designing and implementing small, simple classes, it quickly becomes burdensome for more complex classes, especially those built from many sub-parts. In order to facilitate the building of complex classes from simpler ones, C++ allows us to do object composition in a very simple way -- by using classes as member variables in other classes.
// Lets take a look at some examples of how this is done. If we were designing a personal computer class, we might do it like this (assuming we’d already written a CPU, Motherboard, and RAM class):
// #include "CPU.h"
// #include "Motherboard.h"
// #include "RAM.h"
 
// class PersonalComputer
// {
// private:
   // CPU m_cCPU;
   // Motherboard m_cMotherboard;
   // RAM m_cRAM;
// };
// Initializing class member variables
// In the previous lesson on initializer lists, you learned that the preferred way to initialize class members is through initializer lists rather than assignment. So let’s write a constructor for our PersonalComputer class that uses an initialization list to initialize the member variables. This constructor will take 3 parameters: a CPU speed, a motherboard model, and a RAM size, which it will then pass to the respective member variables when they are constructed.
  // PersonalComputer::PersonalComputer(int nCPUSpeed, char *strMotherboardModel, int nRAMSize)
    // : m_cCPU(nCPUSpeed), m_cMotherboard(strMotherboardModel), m_cRAM(nRAMSize)
// {
// }
// Now, when a PersonalComputer object is instantiated using this constructor, that PersonalComputer object will contain a CPU object initialized with nCPUSpeed, a Motherboard object initialized with strMotherboardModel, and a RAM object initialized with nRAMSize.
// It is worth explicitly noting that composition implies ownership between the complex class and any subclasses. When the complex class is created, the subclasses are created. When the complex class is destroyed, the subclasses are similarly destroyed.
// A full example
// While the above example is useful in giving the general idea of how composition works, let’s do a full example that you can compile yourself. Many games and simulations have creatures or objects that move around a board, map, or screen. The one thing that all of these creatures/objects have in common is that they all have-a location. In this example, we are going to create a creature class that uses a point class to hold the creature’s location.
// First, let’s design the point class. Our creature is going to live in a 2d world, so our point class will have 2 dimensions, X and Y. We will assume the world is made up of discrete squares, so these dimensions will always be integers.
#include <iostream>
#include <iostream>
#include <string>
 
    class Point2D
    {
    private:
        int m_nX;
        int m_nY;
     
    public:
        // A default constructor
        Point2D()
            : m_nX(0), m_nY(0)
        {
        }
     
        // A specific constructor
        Point2D(int nX, int nY)
            : m_nX(nX), m_nY(nY)
        {
        }
     
        // An overloaded output operator
        friend std::ostream& operator<<(std::ostream& out, const Point2D &cPoint)
        {
            out << "(" << cPoint.GetX() << ", " << cPoint.GetY() << ")";
            return out;
        }
     
        // Access functions
        void SetPoint(int nX, int nY)
        {
            m_nX = nX;
            m_nY = nY;
        }
     
        int GetX() const { return m_nX; }
        int GetY() const { return m_nY; }
    };
     
    class Creature
    {
    private:
        std::string m_strName;
        Point2D m_cLocation;
     
        // We don't want people to create Creatures with no name or location
        // so our default constructor is private
        Creature() { }
     
    public:
        Creature(std::string strName, const Point2D &cLocation)
            : m_strName(strName), m_cLocation(cLocation)
        {
        }
     
        friend std::ostream& operator<<(std::ostream& out, const Creature &cCreature)
        {
            out << cCreature.m_strName.c_str() << " is at " << cCreature.m_cLocation;
            return out;
        }
     
        void MoveTo(int nX, int nY)
        {
            m_cLocation.SetPoint(nX, nY);
        }
    };

int main1 ()
{
    using namespace std;
    cout << "Enter a name for your creature: ";
    std::string cName;
    cin >> cName;
    Creature cCreature(cName, Point2D(4, 7));
    
    while (1)
    {
        cout << cCreature << "\n";
        cout << "Enter new X location for creature (-1 to quit): ";
        int nX=0;
        cin >> nX;
        if (nX == -1)
            break;
 
        cout << "Enter new Y location for creature (-1 to quit): ";
        int nY=0;
        cin >> nY;
        if (nY == -1)
            break;
         
        cCreature.MoveTo(nX, nY);
        }
 
    return 0;
}
// Why use composition?
// Instead of using the Point2D class to implement the Creature’s location, we could have instead just added 2 integers to the Creature class and written code in the Creature class to handle the positioning. However, using composition provides a number of useful benefits:
// Each individual class can be kept relatively simple and straightforward, focused on performing one task. This makes those classes easier to write and much easier to understand. For example, Point2D only worries about point-related stuff, which helps keep it simple. 
// Each subobject can be self-contained, which makes them reusable. For example, we could reuse our Point2D class in a completely different application. Or if our creature ever needed another point (for example, a destination it was trying to get to), we can simply add another Point2D member variable. 
// The complex class can have the simple subclasses do most of the hard work, and instead focus on coordinating the data flow between the subclasses. This helps lower the overall complexity of the complex object, because it can delegate tasks to the sub-objects, who already know how to do them. For example, when we move our Creature, it delegates that task to the Point class, which already understands how to set a point. Thus, the Creature class does not have to worry about how such things would be implemented. 
// One question that new programmers often ask is “When should I use composition instead of direct implementation of a feature?”. There’s no 100% answer to that question. However, a good rule of thumb is that each class should be built to accomplish a single task. That task should either be the storage and manipulation of some kind of data (eg. Point2D), OR the coordination of subclasses (eg. Creature). Not both. 
// In this case of our example, it makes sense that Creature shouldn’t have to worry about how Points are implemented, or how the name is being stored. Creature’s job isn’t to know those intimate details. Creature’s job is to worry about how to coordinate the data flow and ensure that each of the subclasses knows what it is supposed to do. It’s up to the individual subclasses to worry about how they will do it.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////  http://www.learncpp.com/cpp-tutorial/103-aggregation/
// 10.3 — Aggregation
// BY ALEX, ON DECEMBER 7TH, 2007
// In the previous lesson on composition, you learned that compositions are complex classes that contain other subclasses as member variables. In addition, in a composition, the complex object “owns” all of the subobjects it is composed of. When a composition is destroyed, all of the subobjects are destroyed as well. For example, if you destroy a car, it’s frame, engine, and other parts should be destroyed as well. If you destroy a PC, you would expect it’s RAM and CPU to be destroyed as well.
// Aggregration
// An aggregation is a specific type of composition where no ownership between the complex object and the subobjects is implied. When an aggregate is destroyed, the subobjects are not destroyed.
// For example, consider the math department of a school, which is made up of one or more teachers. Because the department does not own the teachers (they merely work there), the department should be an aggregate. When the department is destroyed, the teachers should still exist independently (they can go get jobs in other departments).
// Because aggregations are just a special type of compositions, they are implemented almost identically, and the difference between them is mostly semantic. In a composition, we typically add our subclasses to the composition using either normal variables or pointers where the allocation and deallocation process is handled by the composition class.
// In an aggregation, we also add other subclasses to our complex aggregate class as member variables. However, these member variables are typically either references or pointers that are used to point at objects that have been created outside the scope of the class. Consequently, an aggregate class usually either takes the objects it is going to point to as constructor parameters, or it begins empty and the subobjects are added later via access functions or operators.
// Because these subclass objects live outside of the scope of the class, when the class is destroyed, the pointer or reference member variable will be destroyed, but the subclass objects themselves will still exist.
// Let’s take a look at our Teacher and Department example in more detail.
#include <string>
using namespace std;
 
int main2()
{
    class Teacher
    {
    private:
        string m_strName;
    public:
        Teacher(string strName)
            : m_strName(strName)
        {
        }
     
        string GetName() { return m_strName; }
    };
 
    class Department
    {
    private:
        Teacher *m_pcTeacher; // This dept holds only one teacher
     
    public:
        Department(Teacher *pcTeacher=NULL)
            : m_pcTeacher(pcTeacher)
        {
        }
    };
 
    // Create a teacher outside the scope of the Department
    Teacher *pTeacher = new Teacher("Bob"); // create a teacher
    {
        // Create a department and use the constructor parameter to pass
        // the teacher to it.
        Department cDept(pTeacher);
 
    } // cDept goes out of scope here and is destroyed
 
    // pTeacher still exists here because cDept did not destroy it
    delete pTeacher;

    return 0;
}
// In this case, pTeacher is created independetly of cDept, and then passed into cDept’s constructor. Note that the department class uses an initialization list to set the value of m_pcTeacher to the pTeacher value we passed in. When cDept is destroyed, the m_pcTeacher pointer destroyed, but pTeacher is not deallocated, so it still exists until it is independently destroyed.
// To summarize the differences between composition and aggregation:
// Compositions:
// Typically use normal member variables 
// Can use pointer values if the composition class automatically handles allocation/deallocation 
// Responsible for creation/destruction of subclasses 
// Aggregations:
// Typically use pointer variables that point to an object that lives outside the scope of the aggregate class 
// Can use reference values that point to an object that lives outside the scope of the aggregate class 
// Not responsible for creating/destroying subclasses 
// It is worth noting that the concepts of composition and aggregation are not mutually exclusive, and can be mixed freely within the same class. It is entirely possible to write a class that is responsible for the creation/destruction of some subclasses but not others. For example, our Department class could have a name and a teacher. The name would probably be added to the department by composition, and would be created and destroyed with the department. On the other hand, the teacher would be added to the department by aggregate, and created/destroyed independently.
// It is also possible to create other hybrid aggregate/composition schemes, such as where a class holds independent subobjects like an aggregate, but will destroy them when the class goes out of scope like a composition.
// While aggregates can be extremely useful (which we will see more of in the next lesson on container classes), they are also potentially dangerous. As noted several times, aggregates are not responsible for deallocating their subobjects when they are destroyed. Consequently, if there are no other pointers or references to those subobjects when the aggregate is destroyed, those subobjects will cause a memory leak. It is up to the programmer to ensure that this does not happen. This is generally handled by ensuring other pointers or references to those subobjects exist when the aggregate is destroyed.

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://en.wikipedia.org/wiki/Object_composition
// Composition is a kind of association where the composite object has sole responsibility for the disposition of the component parts. The relationship between the composite and the component is a strong “has a” relationship, as the composite object takes ownership of the component. This means the composite is responsible for the creation and destruction of the component parts. An object may only be part of one composite. If the composite object is destroyed, all the component parts must be destroyed. The part has no life of itself and cannot be transferred to another object. Composition enforces encapsulation as the component parts usually are members of the composite object.
 
 // UML notation for an aggregation and a composition (Note that the examples show abstracted data models, disregarding the fact that in a real world carburetor ⇒ car association, there is a market for spare parts.)
// The more general form, aggregation, is depicted as an unfilled diamond and a solid line.
// Aggregation is a kind of association that specifies a whole/part relationship between the aggregate (whole) and component part. This relationship between the aggregate and component is a weak “has a” relationship, as the component may survive the aggregate object. The component object may be accessed through other objects without going through the aggregate object. The aggregate object does not take part in the lifecycle of the component object, meaning the component object may outlive the aggregate object. The state of the component object still forms part of the aggregate object.
// The C++ code below shows what the source code is likely to look like.
// Composition
class Carburetor {};
class Car
{
private:
 
  // Car is the owner of carburetor.
  // Carburetor is created when Car is created,
  // it is destroyed when Car is destroyed.
  Carburetor carb;
 
};

// Aggregation
#include <vector>
class Duck {};
class Pond
{
private:
  std::vector<Duck> ducks;
};

// Aggregation[edit]
// Aggregation differs from ordinary composition in that it does not imply ownership. In composition, when the owning object is destroyed, so are the contained objects. In aggregation, this is not necessarily true. For example, a university owns various departments (e.g., chemistry), and each department has a number of professors. If the university closes, the departments will no longer exist, but the professors in those departments will continue to exist. Therefore, a University can be seen as a composition of departments, whereas departments have an aggregation of professors. In addition, a Professor could work in more than one department, but a department could not be part of more than one university.
// Composition is usually implemented such that an object contains another object. For example, in C++:
#include <vector>
void moreAggregation()
{
class Professor;
 
class Department
{
  string name;
  public:
    Department(string n) :name(n) {}
  // Aggregation: vector of pointers to Professor objects living outside the Department
  std::vector<Professor*> members[5];
};
 
class University
{
  public:
  std::vector<Department> faculty;
 
  University()  // constructor
  {
    // Composition: Departments exist as long as the University exists
    faculty.push_back(Department("chemistry"));
    faculty.push_back(Department("physics"));
    faculty.push_back(Department("arts"));
  }
};
}

// In aggregation, the object may only contain a reference or pointer to the object (and not have lifetime responsibility for it).
// Sometimes aggregation is referred to as composition when the distinction between ordinary composition and aggregation is unimportant.

// Composition is about expressing relationships between objects. Think about the chair example. 
// A chair has a Seat. A chair has a back. And a chair has a set of legs. The phrase "has a" implies 
// a relationship where the chair owns, or at minimum, uses, another object. It is this "has a" 
// relationship that is the basis for composition.
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// http://programmers.stackexchange.com/questions/255973/c-association-aggregation-and-composition

// My understanding is that in:
// Composition: Foo has a Bar object as data member.
// Association: Foo has a pointer to Bar object as a data member 
// Aggregation: Foo has a pointer to Bar object and data of Bar is deep copied in that pointer. 

// There are multiple ways to map the OO concepts of association, aggregation and composition to C++ code. 
// This is especially true for aggregation, because there is not even a consensus what it exactly means.

// Your mapping suggests the following semantics, which are not wrong:
// Composition: Foo has a Bar object as data member -> Foo contains a Bar. It can't exist without it.
// Association: Foo has a pointer to Bar object as a data member, without managing the Bar object -> Foo knows about Bar
// Aggregation: Foo has a pointer to Bar object and manages the lifetime of that object -> Foo contains a Bar, but can also exist without it.

// As stated, aggregation is the difficult one here because, even within UML, the meaning of aggregation 
// is not crystal clear. Another possible meaning of aggregation is "Foo contains a Bar object that is 
// shared with other objects." This would typically be represented in C++ code by means of a 
// std::shared_pointer or boost::shared_pointer.  Which meaning your instructor attaches 
// to aggregation must be discussed with him.



// GB added main, Composition, Association, Aggregation
// GB sprinkled 'public:'
// GB added class Baz

class Baz {
};

void Compostion()
{
  class Bar
  {
  public:       // GB error without public, otherwise cannot access baz
      Baz baz;
  };
  class Foo
  {
      Bar bar;
  public:
      Foo(Baz baz)
      {
        bar.baz=baz;
      }
  };
}

void Association()
{
  class Bar
  {
  public:       // GB error without public, otherwise cannot access baz
      Baz baz;
  };
  class Foo
  {
      Bar* bar;
  public:
      void setBar(Bar* _bar)
      {
          bar=_bar;
      }
  };
}

void Aggregation()
{
  class Bar
  {
  public:       // GB error without public, otherwise cannot access baz
      Baz baz;
  };
  class Foo
  {
      Bar* bar;
  public:
      void setBar(Bar* _bar)
      {
          bar = new Bar;
          bar->baz=_bar->baz;
      }
  };
}

int main()
{
  Compostion();
  Association();
  Aggregation();

  main1();
  main2();
}
