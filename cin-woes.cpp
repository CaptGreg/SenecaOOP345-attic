// http://isocpp.org/wiki/faq/input-output#iostream-vs-stdio


// http://www.cplusplus.com/reference/ios/ios/clear/
// std::ios::clear
// 
// void clear (iostate state = goodbit);
// 
// Set error state flags
// Sets a new value for the stream's internal error state flags.
// 
// The current value of the flags is overwritten: All bits are replaced by those in state; If state is goodbit (which is zero) all error flags are cleared.
// 
// In the case that no stream buffer is associated with the stream when this function is called, the badbit flag is automatically set (no matter the value for that bit passed in argument state).
// 
// Note that changing the state may throw an exception, depending on the latest settings passed to member exceptions.
// 
// The current state can be obtained with member function rdstate.
// 
// Parameters
// 
// state
//     An object of type ios_base::iostate that can take as value any combination of the following state flag member constants:
// 
//     iostate value
//     (member constant)	indicates	functions to check state flags
//     							good()	eof()	fail()	bad()	rdstate()
//     goodbit	No errors (zero value iostate)	        true	false	false	false	goodbit
//     eofbit	End-of-File reached on input operation	false	true	false	false	eofbit
//     failbit	Logical error on i/o operation	false	false	true	false	failbit
//     badbit	Read/writing error on i/o operation	false	false	true	true	badbit
//
//     eofbit, failbit and badbit are member constants with implementation-defined values that can be combined (as if with the bitwise OR operator).
//     goodbit is zero, indicating that none of the other bits is set.



// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Why does my program go into an infinite loop when someone enters an invalid input character? +
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// For example, suppose you have the following code that reads integers from std::cin:

#include <iostream>
int main1()
{
  std::cout << "main1\n";
  std::cin.clear(); // GB
  std::cout << "BAD FORM - Enter numbers separated by whitespace (use -1 to quit): (if cin>>i fails, this fails...)";
  int i = 0;
  while (i != -1) {
    std::cin >> i; // BAD FORM — See comments below
    std::cout << "You entered " << i << '\n';
  }
  // ...
  return 0;
}

// The problem with this code is that it lacks any checking to see if someone entered an invalid input character. In particular, if someone enters something that doesn’t look like an integer (such as an ‘x’), the stream std::cin goes into a “failed state,” and all subsequent input attempts return immediately without doing anything. In other words, the program enters an infinite loop; if 42 was the last number that was successfully read, the program will print the message You entered 42 over and over.

// An easy way to check for invalid input is to move the input request from the body of the while loop into the control-expression of the while loop. E.g.,

int main2()
{
  std::cout << "main2\n";
  std::cin.clear(); // GB
  std::cout << "GOOD FORM - Enter a number, or -1 to quit: ";
  int i = 0;
  while (std::cin >> i) { // GOOD FORM
    if (i == -1) break;
    std::cout << "You entered " << i << '\n';
  }
  // ...
  return 0;
}
// This will cause the while loop to exit either when you hit end-of-file, or when you enter a bad integer, or when you enter -1.

// (Naturally you can eliminate the break by changing the while loop expression from while (std::cin >> i) to while ((std::cin >> i) && (i != -1)), but that’s not really the point of this FAQ since this FAQ has to do with iostreams rather than generic structured programming guidelines.)




// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + How does that funky while (std::cin >> foo) syntax work? +
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// See the previous FAQ for an example of the “funky while (std::cin >> foo) syntax.”
// 
// The expression (std::cin >> foo) calls the appropriate operator>> (for example, it calls the operator>> that takes an std::istream on the left and, if foo is of type int, an int& on the right). The std::istream operator>> functions return their left argument by convention, which in this case means it will return std::cin. Next the compiler notices that the returned std::istream is in a boolean context, so it converts that std::istream into a boolean.
// 
// To convert an std::istream into a boolean, the compiler calls a member function called std::istream::operator void*(). This returns a void* pointer, which is in turn converted to a boolean (NULL becomes false, any other pointer becomes true). So in this case the compiler generates a call to std::cin.operator void*(), just as if you had casted it explicitly such as (void*) std::cin.
// 
// The operator void*() cast operator returns some non-NULL pointer if the stream is in a good state, or NULL if it’s in a failed state. For example, if you read one too many times (e.g., if you’re already at end-of-file), or if the actual info on the input stream isn’t valid for the type of foo (e.g., if foo is an int and the data is an ‘x’ character), the stream will go into a failed state and the cast operator will return NULL.
// 
// The reason operator>> doesn’t simply return a bool (or void*) indicating whether it succeeded or failed is to support the “cascading” syntax:
// 
//     std::cin >> foo >> bar;
// 
// The operator>> is left-associative, which means the above is parsed as:
// 
//     (std::cin >> foo) >> bar;
// 
// In other words, if we replace operator>> with a normal function name such as readFrom(), this becomes the expression:
// 
//     readFrom( readFrom(std::cin, foo), bar);
// 
// As always, we begin evaluating at the innermost expression. Because of the left-associativity of operator>>, this happens to be the left-most expression, std::cin >> foo. This expression returns std::cin (more precisely, it returns a reference to its left-hand argument) to the next expression. The next expression also returns (a reference to) std::cin, but this second reference is ignored since it’s the outermost expression in this “expression statement.”

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Why does my input seem to process past the end of file? +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Because the eof state may not get set until after a read is attempted past the end of file. That is, reading the last byte from a file might not set the eof state. E.g., suppose the input stream is mapped to a keyboard — in that case it’s not even theoretically possible for the C++ library to predict whether or not the character that the user just typed will be the last character.

// For example, the following code might have an off-by-one error with the count i:

int main3() 
{
  std::cout << "main3\n";
  {
    std::cin.clear(); // GB
    std::cout << "WRONG - Enter a number: (if cin>>i fails, this fails...)";
    int i = 0;
    while (! std::cin.eof()) { // WRONG! (not reliable)
      std::cin >> i;
      std::cout << "You entered " << i << '\n';
      ++i;
      // Work with i ...
    }
    std::cin.clear();  // GB
  }

// What you really need is:

  {
    std::cin.clear(); // GB
    std::cout << "RIGHT - Enter a number: ";
    int i = 0;
    while (std::cin >> i) { // RIGHT! (reliable)
      std::cout << "You entered " << i << '\n';
      ++i;
      // Work with i ...
    }
  }
  return 0;
}

int main()
{
  main1();
  main2();
  main3();
}
