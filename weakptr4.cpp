// https://msdn.microsoft.com/en-ca/library/hh279672.aspx

/*
 Sometimes an object must store a way to access the underlying object of a 
 shared_ptr without causing the reference count to be incremented. Typically, 
 this situation occurs when you have cyclic references between shared_ptr instances.

 The best design is to avoid shared ownership of pointers whenever you can. 
 However, if you must have shared ownership of shared_ptr instances, avoid 
 cyclic references between them. When cyclic references are unavoidable, or 
 even preferable for some reason, use weak_ptr to give one or more of the 
 owners a weak reference to another shared_ptr. By using a weak_ptr, you can 
 create a shared_ptr that joins to an existing set of related instances, but 
 only if the underlying memory resource is still valid. A weak_ptr itself does 
 not participate in the reference counting, and therefore, it cannot prevent 
 the reference count from going to zero. However, you can use a weak_ptr to try 
 to obtain a new copy of the shared_ptr with which it was initialized. If the 
 memory has already been deleted, a bad_weak_ptr exception is thrown. If the 
 memory is still valid, the new shared pointer increments the reference count 
 and guarantees that the memory will be valid as long as the shared_ptr 
 variable stays in scope.

 Example

 The following code example shows a case where weak_ptr is used to ensure 
 proper deletion of objects that have circular dependencies. As you examine the 
 example, assume that it was created only after alternative solutions were 
 considered. The Controller objects represent some aspect of a machine process, 
 and they operate independently. Each controller must be able to query the 
 status of the other controllers at any time, and each one contains a private 
 vector<weak_ptr<Controller>> for this purpose. Each vector contains a circular 
 reference, and therefore, weak_ptr instances are used instead of shared_ptr.
*/

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Controller 
{
public:
    int Num;
    wstring Status;
    vector<weak_ptr<Controller>> others;
    explicit Controller(int i) : Num(i) , Status(L"On")
    {
        wcout << L"Creating Controller" << Num << "\n";
    }

    ~Controller()
    {
        wcout << L"Destroying Controller" << Num << "\n";
    }

    // Demonstrates how to test whether the  
    // pointed-to memory still exists or not. 
    void CheckStatuses() const
    {
        for_each(others.begin(), others.end(), [] (weak_ptr<Controller> wp) {
            try {
                auto p = wp.lock();
                wcout << L"Status of " << p->Num << " = " << p->Status << "\n";
            } catch (bad_weak_ptr b) {
                wcout << L"Null object" << "\n";
            }                
        });
    }
};

void RunTest()
{
    vector<shared_ptr<Controller>> v;

    v.push_back(shared_ptr<Controller>(new Controller(0)));
    v.push_back(shared_ptr<Controller>(new Controller(1)));
    v.push_back(shared_ptr<Controller>(new Controller(2)));
    v.push_back(shared_ptr<Controller>(new Controller(3)));
    v.push_back(shared_ptr<Controller>(new Controller(4)));

    // Each controller depends on all others not being deleted. 
    // Give each controller a pointer to all the others. 
    for (int i = 0 ; i < v.size(); ++i) {
        for_each(v.begin(), v.end(), [v,i] (shared_ptr<Controller> p) {
            if(p->Num != i) {
                v[i]->others.push_back(weak_ptr<Controller>(p));
                wcout << L"push_back to v[" << i << "]: " << p->Num << "\n";
            }
        });        
    }

    for_each(v.begin(), v.end(), [](shared_ptr<Controller>& p) {
        wcout << L"use_count = " << p.use_count() << "\n";
        p->CheckStatuses();
    }
    );
}

int main()
{    
    RunTest(); 
    wcout << L"Press any key" << "\n";
    char ch;
    cin.getline(&ch, 1);
}

/*
 As an experiment, modify the vector others to be a vector<shared_ptr<Controller>>, 
 and then in the output, notice that no destructors are invoked when TestRun returns.
*/
