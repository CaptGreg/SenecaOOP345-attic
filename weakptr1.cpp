// http://oopscenities.net/2014/08/03/c-smart-pointers-part-5-weak_ptr/

#include <memory>
#include <iostream>

using namespace std;

int main1()
{
    cout << "\nmain1 - shared_ptr\n";
    struct Child;
     
    struct Parent
    {
        shared_ptr<Child> child;
         
        ~Parent() { cout << "Bye Parent" << "\n"; }
         
        void hi() const { cout << "Hello" << "\n"; }
    };
     
    struct Child
    {
        shared_ptr<Parent> parent;
         
        ~Child() { cout << "Bye Child" << "\n"; }
    };
     
    auto parent = make_shared<Parent>();
    auto child = make_shared<Child>();
    parent->child = child;
    child->parent = parent;
    child->parent->hi();

    return 0;
}

int main2()
{
    cout << "\nmain2 - weak_ptr\n";

    struct Child;
     
    struct Parent
    {
        shared_ptr<Child> child;
         
        ~Parent() { cout << "Bye Parent" << "\n"; }
         
        void hi() const { cout << "Hello" << "\n"; }
    };
     
    struct Child
    {
        weak_ptr<Parent> parent;
         
        ~Child() { cout << "Bye Child" << "\n"; }
    };
     
    auto parent = make_shared<Parent>();
    auto child = make_shared<Child>();
    parent->child = child;
    child->parent = parent;
    child->parent.lock()->hi();

    return 0;
}

int main3()
{
    cout << "\nmain3 - expired weak_ptr\n";

    struct A
    {
        int x;  
        A(int x)  : x(x) { cout << "HI" << "\n"; }
        ~A() { cout << "Bye" << "\n"; }
    };
     
    // cannot define function 'm' local to function 'main'
    // weak_ptr<A> m()
    // coerce it to be a lambda
    auto m = [] () -> weak_ptr<A>
    {
        auto a = make_shared<A>(12);
        cout << a->x << "\n";
         
        return a;
    };
     
    auto a = m();
     
    cout << "After m()" << "\n";
     
    if (a.expired())
        cout << "Expired" << "\n";
    else
        cout << a.lock()->x << "\n";

    return 0;
}

int main()
{
    main1();

    main2();

    main3();

    return 0;
}
