// http://stackoverflow.com/questions/4864341/assigning-c-function-pointers-to-member-functions-of-the-same-object

#include <iostream>

class test {

    int a;
    int b;

    int add      (){ return a + b; }
    int subtract (){ return a - b; }
    int multiply (){ return a * b; }
    int divide   (){ return a / b; }

    public:
    int calculate (char operatr, int operand1, int operand2){
        int (test::*opPtr)() = NULL;

        a = operand1;
        b = operand2;

        if (operatr == '+') opPtr = &test::add;
        if (operatr == '-') opPtr = &test::subtract;
        if (operatr == '*') opPtr = &test::multiply;
        if (operatr == '/') opPtr = &test::divide;

        return (this->*opPtr)();
    }
};

int main(){
    test t;
    std::cout << t.calculate ('+', 2, 3)  << "\n";
    std::cout << t.calculate ('-', 2, 6)  << "\n";
    std::cout << t.calculate ('*', 2, 3)  << "\n";
    std::cout << t.calculate ('/', 20, 3) << "\n";
}

// http://tipsandtricks.runicsoft.com/Cpp/MemberFunctionPointers.html
#if 0

// Function Pointers to Member Functions
// Function pointers are a neat concept, since they basically allow the storage and passing around of functions.
// But when i was first experimenting with them, i had a hard time finding information about how to handle pointers to member functions of classes, instead of ordinary functions (note however, that this was many years ago, and i guess that today's C++ tutorials include them), so here's how to use them:

// First the easy case: pointers to static member functions. Since they don't depend on an object instance, they look like ordinary function pointers.
// Assume we have some class with a static function:

class myclass
{
public:
	static void myfunc(int x) { std::cout << x << "\n"; }
};


// Now we declare a type for a pointer to the static member function:

typedef void ( *STATICFUNC ) ( int );


// Now assigning a function pointer and calling the function with it is easy:

void TestStatic()
{
	STATICFUNC f = myclass::myfunc;	// assign address of memberfunction
	f ( 123 ); // call the function via function pointer
}



// Now to the interesting part: function pointers to non-static member functions.
// First assume we have again our 'myclass', only that myfunc isn't declared static this time.
// The type of a function pointer to it now looks like this:

typedef void ( myclass::*FUNC ) ( int );


// The interesting part of the syntax here is that the asterisk comes after the class name.
// To use the function pointer, we have to instantiate an object of type myclass, then we can create a function pointer and call the function with it:

void TestMemberFunc ()
{
	myclass obj;		// instantiate myclass
	FUNC f = &myclass::myfunc;	// assign address
	( obj.*f ) ( 123 );	// and call it
}


// The important thing in the syntax here are the parentheses around obj.*f, which basically bind the pointer to the object whose member function we are calling. 
#endif
