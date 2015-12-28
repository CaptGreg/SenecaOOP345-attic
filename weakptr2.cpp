// http://en.cppreference.com/w/cpp/memory/weak_ptr

/*
std::weak_ptr is a smart pointer that holds a non-owning ("weak") reference to 
an object that is managed by std::shared_ptr. It must be converted to 
std::shared_ptr in order to access the referenced object.

std::weak_ptr models temporary ownership: when an object needs to be accessed 
only if it exists, and it may be deleted at any time by someone else, 
std::weak_ptr is used to track the object, and it is converted to 
std::shared_ptr to assume temporary ownership. If the original 
std::shared_ptr is destroyed at this time, the object's lifetime is extended 
until the temporary std::shared_ptr is destroyed as well.

In addition, std::weak_ptr is used to break circular references of 
std::shared_ptr.
*/

#include <iostream>
#include <memory>
 
std::weak_ptr<int> gw;
 
void f()
{
    if (auto spt = gw.lock()) { // Has to be copied into a shared_ptr before usage
	std::cout << *spt << "\n";
    }
    else {
        std::cout << "gw is expired\n";
    }
}
 
int main()
{
    {
        auto sp = std::make_shared<int>(42);
	gw = sp;
 
	f();
    }
 
    f();
}

/*
 output
 42
 gw is expired
*/
