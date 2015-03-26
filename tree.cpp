// GB, what happens when vector grows?
// are our pointers invalidated?

// http://stackoverflow.com/questions/12134970/n-ary-tree-design-with-smart-pointers


#include <iostream>
#include <vector>
#include <memory>   // shared_ptr
using namespace std;

// http://en.cppreference.com/w/cpp/memory/enable_shared_from_this
//
// template< class T > class enable_shared_from_this;
// std::enable_shared_from_this allows an object t that is currently managed by a std::shared_ptr 
// named pt to safely generate additional std::shared_ptr instances pt1, pt2, ... that all share 
// ownership of t with pt.
// 
// Inheriting from std::enable_shared_from_this<T> provides the type T with a member function 
// shared_from_this. If an object t of type T is managed by a std::shared_ptr<T> named pt, then 
// calling T::shared_from_this will return a new std::shared_ptr<T> that shares ownership of t with pt.
// 
// Note that prior to calling shared_from_this on an object t, there must be a std::shared_ptr that owns t.
// 
// Also note that enable_shared_from_this provides an alternative to an expression like 
// std::shared_ptr<T>(this), which is likely to result in this being destructed more than once by 
// multiple owners that are unaware of each other. 
// 
// Notes
//
// A common implementation for enable_shared_from_this is to hold a weak reference (such as std::weak_ptr) 
// to this. The constructors of std::shared_ptr detect the presence of an enable_shared_from_this base and 
// assign the newly created std::shared_ptr to the internally stored weak reference. Constructing a 
// std::shared_ptr for an object that is already managed by another std::shared_ptr will not consult the 
// internally stored weak reference and thus will lead to undefined behavior

struct Good: std::enable_shared_from_this<Good>
{
    std::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};
 
struct Bad
{
    std::shared_ptr<Bad> getptr() {
        return std::shared_ptr<Bad>(this);
    }
    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};
 
int main1()
{
    // Good: the two shared_ptr's share the same object
    std::shared_ptr<Good> gp1(new Good);
    std::shared_ptr<Good> gp2 = gp1->getptr();
    std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';
 
    // Bad, each shared_ptr thinks it's the only owner of the object
    std::shared_ptr<Bad> bp1(new Bad);
    std::shared_ptr<Bad> bp2 = bp1->getptr();
    std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
    return 0;
} // UB: double-delete of Bad



// TREE CODE
// http://stackoverflow.com/questions/12134970/n-ary-tree-design-with-smart-pointers
// GB didn't compile, all fixed

struct node : std::enable_shared_from_this<node>
{
    std::vector<std::shared_ptr<node>> children;
    std::weak_ptr<node> parent;
  
    int payload;  // GB add payload

    node() {}
    node(int p) : payload(p) {}
    void add_child(int p)  // GB add payload parameter
    {
        auto n = std::make_shared<node>();
        n->parent = std::weak_ptr<node>(shared_from_this());
        n->payload = p;  // GB add payload
        children.emplace_back(n);
    }
};

int main()
{
    auto root = std::make_shared<node>(999);

    root->add_child(1);
    root->add_child(2);
    root->add_child(3);

    root->children[0]->add_child(40);
    root->children[0]->add_child(50);
    root->children[0]->add_child(60);

    root->children[1]->add_child(41);
    root->children[1]->add_child(51);
    root->children[1]->add_child(61);

    root->children[2]->add_child(42);
    root->children[2]->add_child(52);
    root->children[2]->add_child(62);


    cout << root->payload << "\n";
    for(auto c : root->children) {
       cout << "  " << c->payload << "\n";
       
       for(auto gc : c->children) {
           cout << "    " << gc->payload << "\n";
       }
    }
}
