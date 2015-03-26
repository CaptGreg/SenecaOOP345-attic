// http://www.codeproject.com/Articles/185797/Composite-Design-Pattern

#if 0 // java
class Program
{
    static void Main(string[] args)
    {
        Worker a = new Worker("Worker Tom", 5);
        Supervisor b = new Supervisor("Supervisor Mary", 6);
        Supervisor c = new Supervisor("Supervisor Jerry", 7);
        Supervisor d = new Supervisor("Supervisor Bob", 9);
        Worker e = new Worker("Worker Jimmy", 8);

        //set up the relationships
        b.AddSubordinate(a); //Tom works for Mary
        c.AddSubordinate(b); //Mary works for Jerry
        c.AddSubordinate(d); //Bob works for Jerry
        d.AddSubordinate(e); //Jimmy works for Bob

        //Jerry shows his happiness and asks everyone else to do the same
        if (c is IEmployee)
            (c as IEmployee).ShowHappiness();
    }
}

public interface IEmployee
{
    void ShowHappiness();
}

public class Worker : IEmployee
{
    private string name;
    private int happiness;

    public Worker(string name, int happiness)
    {
        this.name = name;
        this.happiness = happiness;
    }

    void IEmployee.ShowHappiness()
    {
        Console.WriteLine(name + " showed happiness level of " + happiness);
    }
}

public class Supervisor : IEmployee
{
    private string name;
    private int happiness;

    private List<iemployee> subordinate = new List<iemployee>();

    public Supervisor(string name, int happiness)
    {
        this.name = name;
        this.happiness = happiness;
    }

    void IEmployee.ShowHappiness()
    {
        Console.WriteLine(name + " showed happiness level of " + happiness);
        //show all the subordinate's happiness level
        foreach (IEmployee i in subordinate)
            i.ShowHappiness();
    }

    public void AddSubordinate(IEmployee employee)
    {
        subordinate.Add(employee);
    }
}
#endif // java

#include <iostream>
#include <string>
#include <list>
// using namespace std;

class IEmployee   // abstract base class (Java interface)
{
protected:
    std::string name;
    int happiness;
public:
    virtual void ShowHappiness() = 0;
};

class Worker : public IEmployee
{
public: 
    Worker(std::string name, int happiness)
    {
        IEmployee::name      = name;
        IEmployee::happiness = happiness;
    }

    void ShowHappiness()
    {
        std::cout << name << " showed happiness level of " << happiness << "\n";
    }
};

class Supervisor : public IEmployee
{
    std::list<IEmployee*> subordinate;

public:
    Supervisor(std::string name, int happiness)
    {
        IEmployee::name      = name;
        IEmployee::happiness = happiness;
    }

    void ShowHappiness()
    {
        std::cout << name << " showed happiness level of " << happiness << "\n";
	//show all the subordinate's happiness level
        for (auto i : subordinate)
            i->ShowHappiness();
    }

    void AddSubordinate(IEmployee* employee)
    {
        subordinate.push_back(employee);
    }
};

int main(int argc, char**argv)
{
    Worker*     a = new Worker(std::string("Worker Tom"), 5);
    Supervisor* b = new Supervisor(std::string("Supervisor Mary"), 6);
    Supervisor* c = new Supervisor(std::string("Supervisor Jerry"), 7);
    Supervisor* d = new Supervisor(std::string("Supervisor Bob"), 9);
    Worker*     e = new Worker(std::string("Worker Jimmy"), 8);

    //set up the relationships
    b->AddSubordinate(a); //Tom works for Mary
    c->AddSubordinate(b); //Mary works for Jerry
    c->AddSubordinate(d); //Bob works for Jerry
    d->AddSubordinate(e); //Jimmy works for Bob

    //Jerry shows his happiness and asks everyone else to do the same
    c->ShowHappiness();
}
