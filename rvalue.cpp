// http://www.cprogramming.com/c++11/rvalue-references-and-move-semantics-in-c++11.html

#include <iostream>
#include <string>
#include <utility>   // std::move, std::move is just a cast
using namespace std;

  int x;
  int& getRef ()
  {
    return x;
  }

  int getInt ()
  {
    return x;
  }
 
  int && getRvalueInt ()
  {
    // notice that it's fine to move a primitive type--remember, std::move is just a cast
    return std::move( x );
  }

  string getName ()
  {
      return "Alex";
  }

  void printReference (const string& str)
  {
          cout << str;
  }
 
  void printReference (string&& str)
  {
        cout << str;
  }

  void printAddress (const int& v) // const ref to allow binding to rvalues
  {
    cout << reinterpret_cast<const void*>( & v ) << "\n";
  }

int main(int argc, char**arv)
{
  int a;
  a = 1; // here, a is an lvalue

//  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  int x;
  getRef() = 4;

//  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  getName();
  string name = getName();

  const string&& cname = getName(); // ok
  string&& csname = getName(); // also ok - praise be!

//  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  string me( "alex" );
  printReference(  me ); // calls the first printReference function, taking an lvalue reference
 
  printReference( getName() ); // calls the second printReference function, taking a mutable rvalue referen
//  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Move constructor and move assignment operator

  // The most common pattern you'll see when working with rvalue references is to create a move constructor and move assignment operator (which follows the same principles). A move constructor, like a copy constructor, takes an instance of an object as its argument and creates a new instance based on the original object. However, the move constructor can avoid memory reallocation because we know it has been provided a temporary object, so rather than copy the fields of the object, we will move them.

  // What does it mean to move a field of the object? If the field is a primitive type, like int, we just copy it. It gets more interesting if the field is a pointer: here, rather than allocate and initialize new memory, we can simply steal the pointer and null out the pointer in the temporary object! We know the temporary object will no longer be needed, so we can take its pointer out from under it.

  // Imagine that we have a simple ArrayWrapperB class, like this:


  class ArrayWrapperA
  {
    public:
        ArrayWrapperA (int n)
            : _p_vals( new int[ n ] )
            , _size( n )
        {}
        // copy constructor
        ArrayWrapperA (const ArrayWrapperA& other)
            : _p_vals( new int[ other._size  ] )
            , _size( other._size )
        {
            for ( int i = 0; i < _size; ++i )
            {
                _p_vals[ i ] = other._p_vals[ i ];
            }
        }
        ~ArrayWrapperA ()
        {
            delete [] _p_vals;
        }
    private:
    int *_p_vals;
    int _size;
  };

  // Notice that the copy constructor has to both allocate memory and copy every value from the array, one at a time! That's a lot of work for a copy. Let's add a move constructor and gain some massive efficiency.
  class ArrayWrapperB
  {
  public:
    // default constructor produces a moderately sized array
    ArrayWrapperB ()
        : _p_vals( new int[ 64 ] )
        , _size( 64 )
    {}
 
    ArrayWrapperB (int n)
        : _p_vals( new int[ n ] )
        , _size( n )
    {}
 
    // move constructor
    ArrayWrapperB (ArrayWrapperB&& other)
        : _p_vals( other._p_vals  )
        , _size( other._size )
    {
        other._p_vals = NULL;
        other._size = 0;
    }
 
    // copy constructor
    ArrayWrapperB (const ArrayWrapperB& other)
        : _p_vals( new int[ other._size  ] )
        , _size( other._size )
    {
        for ( int i = 0; i < _size; ++i )
        {
            _p_vals[ i ] = other._p_vals[ i ];
        }
    }
    ~ArrayWrapperB ()
    {
        delete [] _p_vals;
    }
 
  private:
    int *_p_vals;
    int _size;
  };

  const ArrayWrapperB getArrayWrapperB (); // makes the move constructor useless, the temporary is const!

 // There's still one more situation we haven't discussed how to handle in a move constructor--when we have a field that is an object. For example, imagine that instead of having a size field, we had a metadata field that looked like this:

class MetaData
{
public:
    MetaData (int size, const std::string& name)
        : _name( name )
        , _size( size )
    {}
 
    // copy constructor
    MetaData (const MetaData& other)
        : _name( other._name )
        , _size( other._size )
    {}
 
    // move constructor
    MetaData (MetaData&& other)
        : _name( other._name )
        , _size( other._size )
    {}
 
    std::string getName () const { return _name; }
    int getSize () const { return _size; }
    private:
    std::string _name;
    int _size;
};

class ArrayWrapperC
{
public:
    // default constructor produces a moderately sized array
    ArrayWrapperC ()
        : _p_vals( new int[ 64 ] )
        , _metadata( 64, "ArrayWrapperC" )
    {}
 
    ArrayWrapperC (int n)
        : _p_vals( new int[ n ] )
        , _metadata( n, "ArrayWrapperC" )
    {}
 
    // move constructor
    ArrayWrapperC (ArrayWrapperC&& other)
        : _p_vals( other._p_vals  )
        , _metadata( other._metadata )
    {
        other._p_vals = NULL;
    }
 
    // copy constructor
    ArrayWrapperC (const ArrayWrapperC& other)
        : _p_vals( new int[ other._metadata.getSize() ] )
        , _metadata( other._metadata )
    {
        for ( int i = 0; i < _metadata.getSize(); ++i )
        {
            _p_vals[ i ] = other._p_vals[ i ];
        }
    }
    ~ArrayWrapperC ()
    {
        delete [] _p_vals;
    }
private:
    int *_p_vals;
    MetaData _metadata;
};


  cout << "\n";
  cout << "printAddress( getInt() );\n";
  printAddress( getInt() );
  cout << "printAddress( x );\n";
  printAddress( x );

  // When you run this program, you'll see that there are two separate values printed.
  // On the other hand,
	
  cout << "printAddress( getRvalueInt() );\n";
  printAddress( getRvalueInt() );
  cout << "printAddress( x );\n";
  printAddress( x );
  // prints the same value because we are explicitly returning an rvalue here. 
}
