// https://support.microsoft.com/en-us/kb/837697
// GB converted to C++
// GB converted char* to string

#include <iostream>
#include <queue>
#include <vector>
#include <string>
using namespace std;

//Define a custom data type.
class Student {
public:
	// char* chName;
	string chName;
	int nAge;
	Student(): chName(""),nAge(0){}
	Student( char* chNewName, int nNewAge ):chName(chNewName), nAge(nNewAge){}
};

//Overload the < operator.
bool operator< (const Student& structstudent1, const Student &structstudent2)
{
	return structstudent1.nAge > structstudent2.nAge;	
}
//Overload the > operator.
bool operator> (const Student& structstudent1, const Student &structstudent2)
{
	return structstudent1.nAge < structstudent2.nAge;	
}

int main()
{
	//Declare a priority_queue and specify the ORDER as <
	//The priorities will be assigned in the Ascending Order of Age
	priority_queue<Student, vector<Student>,less<vector<Student>::value_type> > pqStudent1;

	//declare a priority_queue and specify the ORDER as >
	//The priorities will be assigned in the Descending Order of Age
	priority_queue<Student, vector<Student>,greater<vector<Student>::value_type> > pqStudent2;
	
	// Add container elements.
	pqStudent1.push( Student( "Mark", 38 ) );
	pqStudent1.push( Student( "Marc", 25 ) );
	pqStudent1.push( Student( "Bill", 47 ) );
	pqStudent1.push( Student( "Andy", 13 ) );
	pqStudent1.push( Student( "Newt", 44 ) );

	//Display container elements.
	while ( !pqStudent1.empty() ) {
		cout << pqStudent1.top().chName << "\n";
		pqStudent1.pop();
	}
	cout << "\n";

	// Add container elements.
	pqStudent2.push( Student( "Mark", 38 ) );
	pqStudent2.push( Student( "Marc", 25 ) );
	pqStudent2.push( Student( "Bill", 47 ) );
	pqStudent2.push( Student( "Andy", 13 ) );
	pqStudent2.push( Student( "Newt", 44 ) );
    
	//Display container elements.
	while ( !pqStudent2.empty() ) {
		cout << pqStudent2.top().chName << "\n";
		pqStudent2.pop();
	}
	cout << "\n";
	
	return 0;
}
