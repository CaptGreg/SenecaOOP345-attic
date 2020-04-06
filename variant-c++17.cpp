// Bartek's variant blog: https://www.bfilipek.com/2018/06/variant.html

// This example, also from Bartek, is simple, easy to follow, and concise.

// We have more options on how to implement such polymorphism!
// For example, since C++17 we can use std::variant and std::visit.
// 
// In short std::variant allows you to store different types in a single object - it's a typesafe union. 
// And then with std::visit you can call some operation and then it will dispatch it according to the currently active type in a variant.

#include <iostream>
#include <variant>
#include <vector>
using namespace std;

struct Triangle {
    void Render() { cout << "Drawing a triangle!\n"; }
};

struct Polygon {
    void Render() { cout << "Drawing a polygon!\n"; }
};

struct Sphere {
    void Render() { cout << "Drawing a sphere!\n"; }
};

using Renderable = std::variant<Triangle, Polygon, Sphere>;

std::vector<Renderable> vecRender {
        Polygon(), Triangle(), Sphere(), Triangle()
    };

int main ()
{
    // auto CallRender = (auto& obj) { obj.Render(); };   // FACTORY

    auto CallRender = [] (auto& obj) { obj.Render(); };   // fixed FACTORY

    for (auto& obj : vecRender )
        std::visit(CallRender, obj);
}


/*
  Here are some observations we can make:

    There's no base class, types inside a variant might be unrelated.

    We can easily add a new visitor, without changing the implementation of classes. 
    For example, we can implement a visitor that scans the objects and sorts them according to Z-order. 
    In regular polymorphism, adding a new virtual method is more problematic, as you'd have to update the whole class hierarchy.

    It's harder to add new types: you'd have to extend the Renderable type and also each visitor to support the new type. 
    That's much easier with regular polymorphism.

    Since std::variant is the size of the largest type, then we might waste some space when types differ a lot. 
    For example, when one type holds 128 bytes, while the other is only 16 bytes.

    We can use value semantics rather than pointer semantics, so we can have benefits of a contiguous memory block and possible cache friendliness.
*/
