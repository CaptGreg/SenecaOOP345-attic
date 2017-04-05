
// question: can enum handle different types such as flot and int?

enum { A = 99, B = 99.99};

// answer: NO

// enum.cpp:4:20: error: enumerator value for ‘B’ is not an integer constant
//  enum { A = 99, B = 99.99};


int main()
{
}
