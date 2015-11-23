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
         
        ~Parent() { cout << "Bye Parent" << endl; }
         
        void hi() const { cout << "Hello" << endl; }
    };
     
    struct Child
    {
        shared_ptr<Parent> parent;
         
        ~Child() { cout << "Bye Child" << endl; }
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
         
        ~Parent() { cout << "Bye Parent" << endl; }
         
        void hi() const { cout << "Hello" << endl; }
    };
     
    struct Child
    {
        weak_ptr<Parent> parent;
         
        ~Child() { cout << "Bye Child" << endl; }
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
        A(int x)  : x(x) { cout << "HI" << endl; }
        ~A() { cout << "Bye" << endl; }
    };
     
    // cannot define function 'm' local to function 'main'
    // weak_ptr<A> m()
    // coerce it to be a lambda
    auto m = [] () -> weak_ptr<A>
    {
        auto a = make_shared<A>(12);
        cout << a->x << endl;
         
        return a;
    };
     
    auto a = m();
     
    cout << "After m()" << endl;
     
    if (a.expired())
        cout << "Expired" << endl;
    else
        cout << a.lock()->x << endl;

    return 0;
}

int main()
{
    main1();

    main2();

    main3();

    return 0;
}
