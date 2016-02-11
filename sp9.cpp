// DIY shared_ptr

// http://www.codeproject.com/Articles/15351/Implementing-a-simple-smart-pointer-in-c
// a 2006 implentation, long before C++11
// GB - a few bugs
// GB   - no RefCount ctor
// GB   - RefCount::count was never initialized

// GB add C++11 std::shared_ptr use_count() and data() member functions to DIY_shared_ptr

#include <iostream>
using namespace std;

template <typename T>
class DIY_unique_ptr
{
private:
    T*    pData;
public:
    DIY_unique_ptr(T* pValue) : pData(pValue) { }    // constructor
    ~DIY_unique_ptr() { delete  pData; }             // destructor
    T& operator*   () { return *pData; }             // operator*  overload
    T* operator->  () { return  pData; }             // operator-> overload
};

template < typename T > 
class DIY_shared_ptr
{
    class RefCount // RefCount = reference count
    // GB: make class local to DIY_shared_ptr
    {
        int count; // Reference count
    public:
        RefCount() : count(0) {} // GB: no ctor and count was never intialized
    
        void AddRef() // Increment the reference count
            { count++; }
    
        int Release() // Decrement and return the reference count
            { return --count; }
    
        int use_count() const { return count; } // GB
    };

    T*    pData;       // pointer
    RefCount*   reference;   // Reference count

public:
    DIY_shared_ptr() : pData(nullptr), reference(nullptr) // default constructor
    {
        // Create a new reference 
        reference = new RefCount();
        // Increment the reference count
        reference->AddRef();
    }

    DIY_shared_ptr(T* pValue) : pData(pValue), reference(0)  // constructor
    {
        // Create a new reference 
        reference = new RefCount();
        // Increment the reference count
        reference->AddRef();
    }

    DIY_shared_ptr(const DIY_shared_ptr<T>& sp) : pData(sp.pData), reference(sp.reference) // Copy constructor
    {
        // Copy the data and reference pointer <-- GB initialized above ": pData(...), reference(...)"
        // and increment the reference count
            reference->AddRef();
    }

    ~DIY_shared_ptr() // Destructor
    {
        // Decrement the reference count
        // if reference become zero delete the data
        if(reference->Release() == 0)
        {
            delete pData;
            delete reference;
        }
    }

    T& operator*  () { return *pData; }

    T* operator-> () { return  pData; }
    
    DIY_shared_ptr<T>& operator = (const DIY_shared_ptr<T>& sp) // Assignment operator
    {
        if (this != &sp) // Avoid self assignment
        {
            // Decrement the old reference count
            // if reference become zero delete the old data
            if(reference->Release() == 0)
            {
                delete pData;
                delete reference;
            }

            // Copy the data and reference pointer
            // and increment the reference count
            pData = sp.pData;
            reference = sp.reference;
            reference->AddRef();
        }
        return *this;
    }
    int use_count() const { return reference->use_count(); } // GB
    T*  data()  const { return pData; } // GB
};

class X {
  std::string name;
public:
  X(const std::string& n) { name = n; cout << "X ctor " << name << "\n"; }
  ~X() { cout << "X dtor " << name << "\n"; }
  void hello() { cout << "hello from " << name << "\n\n"; }
};


int main(int argc, char**argv)
{
  cout << "main starting...\n";

  try {
    X x("x");
    x.hello();

    X* p = new X("p");
    p->hello();

    DIY_unique_ptr<X> diy_up(new X("diy unique smart pointer"));
    diy_up->hello();

    DIY_shared_ptr<X> ssp(new X("diy shared smart pointer"));
    ssp->hello();
    cout << "ssp.use_count()=" << ssp.use_count() << "\n";

    DIY_shared_ptr<X> ssp2( ssp );    // copy ctor
    ssp2->hello();
    cout << "ssp.use_count ()=" << ssp.use_count()  << "\n";
    cout << "ssp2.use_count()=" << ssp2.use_count() << "\n";

    DIY_shared_ptr<X> ssp3 =  ssp2;   // assignment operator
    ssp3->hello();
    cout << "ssp.use_count ()=" << ssp.use_count()  << "\n";
    cout << "ssp2.use_count()=" << ssp2.use_count() << "\n";
    cout << "ssp3.use_count()=" << ssp3.use_count() << "\n";

    if(argc > 2)  {
       cout << "argc > 2, return\n";
       return 0;
    } else
    if(argc > 1)  {
       cout << "argc > 1, we are throwing\n";
       throw string("we are throwing");
    }

    delete p;
  } catch (const std::exception& e){
    cout << "It threw a std::exception: " << e.what() << "\n";
  } catch (const string& e){
    cout << "It threw a string: " << e << "\n";
  }

  cout << "main terminating...\n";
}
